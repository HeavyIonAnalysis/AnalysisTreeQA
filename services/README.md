# AnalysisTreeQA services: CLI + YAML task framework

This is the optional, compiled-binary alternative to writing and compiling a macro like `examples/example.cpp`: a YAML config file lists which QA tasks to run, on which branches, with which cuts; the binary
(`analysistreeqa`) does the rest. It needs no changes to the core `AnalysisTreeQA` library and does not affect the Macro API at all (see the main [`README.md`](../README.md)).

## Building

```
cmake -DAnalysisTreeQA_BUILD_SERVICES=ON ..
make -j install
```
Needs `yaml-cpp` and Boost's `program_options` component in addition to the usual ROOT + AnalysisTree requirements (see the main README). Both are only looked up when `AnalysisTreeQA_BUILD_SERVICES=ON` is passed.
With `AnalysisTreeQA_BUILD_EXAMPLES=ON` (the default) also passed, this additionally compiles two worked-example task classes under `examples/`, see [`../examples/README.md`](../examples/README.md) for the full tutorial, including how to write your own in a separate repository.

## Running

```
analysistreeqa -c my_config.yaml
analysistreeqa -c my_config.yaml -i other_filelist.txt -o other_output.root -w
analysistreeqa --print-registered-tasks
```
| Flag | Required | Meaning |
| --- | --- | --- |
| `-c`, `--config` | yes (1) | path to the YAML config |
| `-i`, `--input` | no | overrides the config's `input.files` (multiple `-i` allowed) |
| `-o`, `--output` | no | overrides the config's `output.file` |
| `-w`, `--overwrite` | no | overrides the config's `output.overwrite` to `true` |
| `--print-registered-tasks` | no | list every task type currently linked into this binary (built-in + the two worked examples from `examples/`, if built) and exit, see below |
| `-h`, `--help` | no | print usage |

(1) not required together with `--print-registered-tasks` (same as `--help`) - useful for discovering which `task:` names are available before writing a config at all:
```
$ analysistreeqa --print-registered-tasks
Registered task types (usable as 'task:' in YAML):
  HistogramQaTask
  TrackQaTask
```
With `-DAnalysisTreeQA_BUILD_EXAMPLES=ON` (the default), the same command also lists `ExampleEventHeaderTask` and `ExampleParticleTask` from `../examples/`, see [`../examples/README.md`](../examples/README.md).

## YAML schema

See [`config/default.yaml`](config/default.yaml) for a minimal working example and [`config/example_full.yaml`](config/example_full.yaml) for one exercising every cut style and the `HistogramQaTask` task type.

```yaml
input:
  files: ["data.root", "filelist.txt"]   # required, non-empty
  tree: "rTree"                          # required, AnalysisTree tree name

output:
  file: "qa.root"     # required
  overwrite: false    # optional, default false
```
Each `input.files` entry (and `-i`) accepts **either** a ROOT file directly **or** a "filelist" text file (one AnalysisTree ROOT file path per line, `AnalysisTree::Chain::InitChain()`'s own convention, also what
`examples/example.cpp`'s macro API takes as its "filelist" argument).

```yaml

nevents: -1            # optional, default -1 (all events)

cuts:                  # optional: shared cuts, reusable by name across tasks
  pT_cut:
    simple_cuts:
      - {variable: "VtxTracks.pT", type: range, min: 1.0, max: 1.5}

tasks:                 # required, non-empty list
  - task: TrackQaTask       # one of the built-in types below, or your own (see ../examples/README.md)
    name: VtxTracksQA    # optional (defaults to `task`) - also the output subdirectory name
    branch: VtxTracks
    cuts: pT_cut          # named reference into the shared `cuts:` map above
```

Every task `cuts:` field accepts three forms: 
- a **name** (scalar string) referencing the shared `cuts:` map,
- an **inline** mapping: `cuts: {simple_cuts: [...]}`,
- omitted entirely (no cut applied).

Each entry in a `simple_cuts:` list is one of:
```yaml
{variable: "Branch.field", type: range,      min: <num>, max: <num>, title: "optional"}
{variable: "Branch.field", type: equals,     value: <int>,           title: "optional"}
{variable: "Branch.field", type: not_equals, value: <int>,           title: "optional"}
{type: custom, name: "RegisteredCutName", ...any params your cut reads...}
{type: or, clauses: [...], title: "optional"}
```
All entries in one `simple_cuts:` list are combined with logical **AND**. `range`/`equals`/`not_equals` cover "one field vs. a threshold/value", e.g. a MC-truth primary/secondary selection is just
`{variable: "SimParticles.mother_id", type: equals, value: -1}` (primary) or `type: not_equals` (secondary), no custom code needed.

**Logical OR**: `AnalysisTree::Cuts` itself only ever ANDs its clauses, so OR is provided as its own declarative type, `or`, combining several `range`/`equals`/`not_equals` clauses (each on its own field)
into a single AND-able entry:

```yaml
simple_cuts:
  - type: or
    clauses:
      - {variable: "SimParticles.pid", type: equals, value: 211}
      - {variable: "SimParticles.pid", type: equals, value: 2212}
  - {variable: "VtxTracks.pT", type: range, min: 0.2, max: 2.0}
```
which reads as `(pid == 211 OR pid == 2212) AND (0.2 <= pT <= 2.0)`. Nested `or`/`custom` clauses *inside* an `or` aren't supported (each clause must be `range`/`equals`/`not_equals`). Write a custom cut (below)
if you need more than that.

The `custom` type dispatches to a cut registered in C++ via `ATQA_REGISTER_CUT` (see `CutFactory.hpp`). Only needed for something the three declarative types truly can't express, since each only ever compares
ONE field against a fixed value. Registering a custom cut is a core `services/` capability, not something tied to private tasks. A generic, built-in one ships in `steer/tasks/BuiltinCuts.cpp`:

| `name:` | Meaning | Params |
| --- | --- | --- |
| `RatioInRange` | ratio of two fields (from the same branch) within a range, e.g. chi2/ndf | `branch`, `numerator_field`, `denominator_field`, `min`, `max` (all required) |

```yaml
{type: custom, name: RatioInRange, branch: VtxTracks, numerator_field: chi2, denominator_field: ndf, min: 0, max: 10}
```

AnalysisTreeQA does *not* ship analysis-specific combo cuts by default (which quantities define e.g. "a good track" is an analysis choice, not a framework default). Register your own named C++ predicate once via
`ATQA_REGISTER_CUT` (see `CutFactory.hpp`), typically right in your own task's `.cpp` file:
```cpp
ATQA_REGISTER_CUT(MyCut, [](const YAML::Node& params) {
  return AnalysisTree::SimpleCut({"Branch.field"}, [](std::vector<double> v) { return v[0] > 0; }, "MyCut");
});
```
and reference it from YAML as `{type: custom, name: MyCut}` inside any `simple_cuts:` list.

## Built-in task types

Only two task types ship as built-in (part of `services/steer/tasks/`, always available whenever `AnalysisTreeQA_BUILD_SERVICES=ON`): kept deliberately minimal, since a canned histogram set for e.g. "particle QA" is
an analysis-specific choice, not a framework default:

| `task:` | Wraps | Required fields | Optional fields |
| --- | --- | --- | --- |
| `TrackQaTask` | `BasicQA::AddTrackQA` | `branch` | `cuts` |
| `HistogramQaTask` | direct `AddH1`/`AddH2`/`AddProfile` calls | `plots: [...]` (see below) | - |

`ExampleEventHeaderTask` and `ExampleParticleTask` live as worked examples in `../examples/` instead (`-DAnalysisTreeQA_BUILD_EXAMPLES=ON`, the default): each books one H1, one H2 and one TProfile directly in
`Init()`, without going through a `BasicQA.hpp` helper, specifically so the code is easy to read and copy from. Use them directly or as templates for your own; see [`../examples/README.md`](../examples/README.md).

### `HistogramQaTask`: arbitrary branch/field combinations, no new C++ needed

```yaml
- task: HistogramQaTask
  name: TofPidCheck
  plots:
    - type: h2                # h1, h2, or profile
      name: p_vs_mass2         # optional, plot name
      x: {branch: VtxTracks, field: p,     bins: {nbins: 200, min: 0,    max: 4}}
      y: {branch: TofHits,   field: mass2, bins: {nbins: 200, min: -0.2, max: 1.2}}
      weight: {branch: ..., field: ...}   # optional
      cuts: {simple_cuts: [...]}          # optional
```
`type: h1` needs only `x`; `type: h2`/`profile` need both `x` and `y`. Integral plots aren't exposed here yet, write your own task (see `../examples/README.md`) if you need one.

#### Cross-branch limits (inherited from AnalysisTree, not an AnalysisTreeQA restriction)

`AnalysisTree::Matching` only ever correlates **two** independent multi-object branches (e.g. `VtxTracks` <-> `TofHits`). A plot's x/y/weight axes and its cuts may together reference at most two such branches, plus any
number of *EventHeader*-type branches (a single value broadcast to every entry, needing no matching, e.g. a reaction-plane angle from `SimEventHeader`). Referencing a **third**, independent multi-object branch, e.g. `VtxTracks.p` vs
`TofHits.mass2` cut on `SimParticles.pid`, throws a `runtime_error` from AnalysisTree itself. This is the same limitation the main [`README.md`](../README.md)'s "Known problems" section documents; lifting it
would mean adding an N-way matching mechanism to the AnalysisTree library itself, out of scope here.

## Writing your own task

See [`../examples/README.md`](../examples/README.md) for the full, step-by-step tutorial: it covers both the Macro API and setting up your own separate repository that depends on AnalysisTreeQA
as an installed package (using `ExampleEventHeaderTask`/`ExampleParticleTask` as copyable starting points for the latter).
