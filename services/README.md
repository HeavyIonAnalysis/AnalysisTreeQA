# AnalysisTreeQA services: CLI + YAML task framework

This is the optional, compiled-binary alternative to writing and compiling a
macro like `examples/example.cpp`: a YAML config file lists which QA tasks
to run, on which branches, with which cuts - the binary
(`analysistreeqa`) does the rest. It needs no changes to the core
`AnalysisTreeQA` library and does not affect the classic macro workflow at
all (see the main [`README.md`](../README.md)).

## Building

```
cmake -DAnalysisTreeQA_BUILD_SERVICES=ON [-DAnalysisTreeQA_BUILD_TASKS=ON] ..
make -j install
```
Needs `yaml-cpp` and Boost's `program_options` component in addition to the
usual ROOT + AnalysisTree requirements (see the main README) - both are only
looked up when `AnalysisTreeQA_BUILD_SERVICES=ON` is passed.
`AnalysisTreeQA_BUILD_TASKS=ON` additionally compiles any private task
classes you've dropped into the top-level `tasks/` directory - see
[`../tasks/README.md`](../tasks/README.md).

## Running

```
analysistreeqa -c my_config.yaml
analysistreeqa -c my_config.yaml -i other_input.root -o other_output.root -w
analysistreeqa --print-registered-tasks
```
| Flag | Required | Meaning |
| --- | --- | --- |
| `-c`, `--config` | yes* | path to the YAML config |
| `-i`, `--input` | no | overrides the config's `input.files` (multiple `-i` allowed) |
| `-o`, `--output` | no | overrides the config's `output.file` |
| `-w`, `--overwrite` | no | overrides the config's `output.overwrite` to `true` |
| `--print-registered-tasks` | no | list every task type currently linked into this binary (built-in + any private ones from `tasks/`) and exit - see below |
| `-h`, `--help` | no | print usage |

\* not required together with `--print-registered-tasks` (same as `--help`) - useful for discovering which `type:` names are available before writing a config at all:
```
$ analysistreeqa --print-registered-tasks
Registered task types (usable as 'type:' in YAML):
  EventHeaderQA
  HistogramQA
  ParticleQA
  ParticlesFlowQA
  TrackQA
  TracksMatchQA
```
(plus any private task types from `tasks/` if the binary was built with `AnalysisTreeQA_BUILD_TASKS=ON`).

## YAML schema

See [`config/default.yaml`](config/default.yaml) for a minimal working
example and [`config/example_full.yaml`](config/example_full.yaml) for one
exercising every cut style and the `HistogramQA` task type.

```yaml
input:
  files: ["file1.root", "file2.root"]   # required, non-empty
  tree: "rTree"                          # required, AnalysisTree tree name

output:
  file: "qa.root"     # required
  overwrite: false    # optional, default false

nevents: -1            # optional, default -1 (all events)

cuts:                  # optional: shared cuts, reusable by name across tasks
  pT_cut:
    simple_cuts:
      - {variable: "VtxTracks.pT", type: range, min: 1.0, max: 1.5}

tasks:                 # required, non-empty list
  - type: TrackQA       # one of the built-in types below, or a private one (tasks/)
    name: VtxTracksQA    # optional (defaults to `type`) - also the output subdirectory name
    branch: VtxTracks
    cuts: pT_cut          # named reference into the shared `cuts:` map above
```

Every task's `cuts:` field accepts three forms:
- a **name** (scalar string) referencing the shared `cuts:` map,
- an **inline** mapping: `cuts: {simple_cuts: [...]}`,
- omitted entirely (no cut applied).

Each entry in a `simple_cuts:` list is one of:
```yaml
{variable: "Branch.field", type: range,      min: <num>, max: <num>, title: "optional"}
{variable: "Branch.field", type: equals,     value: <int>,           title: "optional"}
{variable: "Branch.field", type: not_equals, value: <int>,           title: "optional"}
{type: custom, name: "RegisteredCutName", ...any params your cut reads...}
```
All entries in one `simple_cuts:` list are combined with logical AND. These
three declarative types cover "one field vs. a threshold/value" - e.g. an
MC-truth primary/secondary selection is just
`{variable: "SimParticles.mother_id", type: equals, value: -1}` (primary) or
`type: not_equals` (secondary), no custom code needed.

The `custom` type dispatches to a cut registered in C++ via
`ATQA_REGISTER_CUT` (see `CutFactory.hpp`) - only needed for something the
three declarative types truly can't express, since each only ever compares
ONE field against a fixed value. Registering a custom cut is a core `services/`
capability, not something tied to private tasks - a generic, built-in one
ships in `steer/tasks/BuiltinCuts.cpp`:

| `name:` | Meaning | Params |
| --- | --- | --- |
| `RatioInRange` | ratio of two fields (from the same branch) within a range, e.g. chi2/ndf | `branch`, `numerator_field`, `denominator_field`, `min`, `max` (all required) |

```yaml
{type: custom, name: RatioInRange, branch: VtxTracks, numerator_field: chi2, denominator_field: ndf, min: 0, max: 10}
```

AnalysisTreeQA does *not* ship analysis-specific combo cuts by default
(which quantities define e.g. "a good track" is an analysis choice, not a
framework default) - see `../tasks/README.md` and
`../tasks/ExampleCustomCut.cpp` for a worked example of that kind, and how to
add your own.

## Built-in task types

| `type:` | Wraps | Required fields | Optional fields |
| --- | --- | --- | --- |
| `EventHeaderQA` | `BasicQA::AddEventHeaderQA` | `branch` | `cuts` |
| `TrackQA` | `BasicQA::AddTrackQA` | `branch` | `cuts` |
| `ParticleQA` | `BasicQA::AddParticleQA` | `branch` | `cuts` |
| `TracksMatchQA` | `BasicQA::AddTracksMatchQA` | `rec_branch`, `sim_branch` | `cuts` |
| `ParticlesFlowQA` | `BasicQA::AddParticlesFlowQA` | `particles`, `psi_rp: {branch, field}`, `pdg_codes: [...]` | `cuts` (ANDed into each per-PDG selection) |
| `HistogramQA` | direct `AddH1`/`AddH2`/`AddProfile` calls | `plots: [...]` (see below) | - |

### `HistogramQA`: arbitrary branch/field combinations, no new C++ needed

```yaml
- type: HistogramQA
  name: TofPidCheck
  plots:
    - kind: h2                # h1, h2, or profile
      name: p_vs_mass2         # optional, plot name
      x: {branch: VtxTracks, field: p,     bins: {nbins: 200, min: 0,    max: 4}}
      y: {branch: TofHits,   field: mass2, bins: {nbins: 200, min: -0.2, max: 1.2}}
      weight: {branch: ..., field: ...}   # optional
      cuts: {simple_cuts: [...]}          # optional
```
`kind: h1` needs only `x`; `kind: h2`/`profile` need both `x` and `y`.
Integral plots aren't exposed here yet - write a private task (see
`../tasks/README.md`) if you need one.

#### Cross-branch limits (inherited from AnalysisTree, not an AnalysisTreeQA restriction)

`AnalysisTree::Matching` only ever correlates **two** independent
multi-object branches (e.g. `VtxTracks` <-> `TofHits`). A plot's x/y/weight
axes and its cuts may together reference at most two such branches, plus any
number of *EventHeader*-type branches (a single value broadcast to every
entry, needing no matching - e.g. `ParticlesFlowQA`'s `psi_rp`). Referencing
a **third**, independent multi-object branch - e.g. `VtxTracks.p` vs
`TofHits.mass2` cut on `SimParticles.pid` - throws a `runtime_error` from
AnalysisTree itself. This is the same limitation the main
[`README.md`](../README.md)'s "Known problems" section documents; lifting it
would mean adding an N-way matching mechanism to the AnalysisTree library
itself, out of scope here.

## Writing your own task

See [`../tasks/README.md`](../tasks/README.md).
