# AnalysisTreeQA services: CLI + YAML task framework

This is the optional, compiled-binary alternative to writing and compiling a
macro like `examples/example.cpp`: a YAML config file lists which QA tasks
to run, on which branches, with which cuts; the binary
(`analysistreeqa`) does the rest. It needs no changes to the core
`AnalysisTreeQA` library and does not affect the classic macro workflow at
all (see the main [`README.md`](../README.md)).

## Building

```
cmake -DAnalysisTreeQA_BUILD_SERVICES=ON [-DAnalysisTreeQA_BUILD_TASKS=ON] ..
make -j install
```
Needs `yaml-cpp` and Boost's `program_options` component in addition to the
usual ROOT + AnalysisTree requirements (see the main README). Both are only
looked up when `AnalysisTreeQA_BUILD_SERVICES=ON` is passed.
`AnalysisTreeQA_BUILD_TASKS=ON` additionally compiles any private task
classes you've dropped into the top-level `tasks/` directo,, see
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
  HistogramQA
  TrackQA
```
With `-DAnalysisTreeQA_BUILD_TASKS=ON`, the same command also lists the
example tasks from `../tasks/` (`EventHeaderQA`, `ParticleQA`,
`ParticlesFlowQA`, `TracksMatchQA`, `ExampleUserTask`, ...) plus whatever
private task types you've added there yourself.

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
{type: or, clauses: [...], title: "optional"}
```
All entries in one `simple_cuts:` list are combined with logical **AND**.
`range`/`equals`/`not_equals` cover "one field vs. a threshold/value" - e.g.
an MC-truth primary/secondary selection is just
`{variable: "SimParticles.mother_id", type: equals, value: -1}` (primary) or
`type: not_equals` (secondary), no custom code needed.

**Logical OR**: `AnalysisTree::Cuts` itself only ever ANDs its clauses, so OR
is provided as its own declarative type, `or`, combining several
`range`/`equals`/`not_equals` clauses (each on its own field) into a single
AND-able entry:
```yaml
simple_cuts:
  - type: or
    clauses:
      - {variable: "SimParticles.pid", type: equals, value: 211}
      - {variable: "SimParticles.pid", type: equals, value: 2212}
  - {variable: "VtxTracks.pT", type: range, min: 0.2, max: 2.0}
```
which reads as `(pid == 211 OR pid == 2212) AND (0.2 <= pT <= 2.0)`. Nested
`or`/`custom` clauses *inside* an `or` aren't supported (each clause must be
`range`/`equals`/`not_equals`) - write a custom cut (below) if you need more
than that.

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

Only two task types ship as built-in (part of `services/steer/tasks/`,
always available whenever `AnalysisTreeQA_BUILD_SERVICES=ON`) - kept
deliberately minimal, since a canned histogram set for e.g. "particle QA" is
an analysis-specific choice, not a framework default:

| `type:` | Wraps | Required fields | Optional fields |
| --- | --- | --- | --- |
| `TrackQA` | `BasicQA::AddTrackQA` | `branch` | `cuts` |
| `HistogramQA` | direct `AddH1`/`AddH2`/`AddProfile` calls | `plots: [...]` (see below) | - |

`EventHeaderQA`, `ParticleQA`, `TracksMatchQA` and `ParticlesFlowQA` (the same
`BasicQA.hpp`-wrapping pattern as `TrackQA`) live as ready-to-use *examples*
in `../tasks/` instead - use them directly (`-DAnalysisTreeQA_BUILD_TASKS=ON`)
or as templates for your own; see `../tasks/README.md`.

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

Two options, see the main [`README.md`](../README.md#where-should-your-own-qa-tasks-live)
for how to choose between them:
- **Inside a fork of AnalysisTreeQA itself**: see [`../tasks/README.md`](../tasks/README.md).
- **From your own, separate repository**: see the next section.

## Using AnalysisTreeQA from your own repository

Depend on AnalysisTreeQA as an external package (the same way the sibling
repository `Centrality` depends on the base `AnalysisTree` library) instead
of forking it, if your analysis should be versioned independently and
shouldn't live inside/track AnalysisTreeQA's own git history. You still get
the full `services/` infrastructure - the self-registering
`TaskFactory`/`CutFactory`, the built-in task types, and the CLI/config
plumbing (`AnalysisTreeQAAppCore`) - without copying any of it.

### 1. Point your CMake at an installed AnalysisTreeQA

`find_package(AnalysisTreeQA)` needs an install built with
`-DAnalysisTreeQA_BUILD_SERVICES=ON` (add `-DAnalysisTreeQA_BUILD_TASKS=ON`
too if you also want the `../tasks/` examples available). A dual-mode
`cmake/AnalysisTreeQA.cmake` module (find a pre-installed copy, or fetch and
build one), modeled on `Centrality`'s own `cmake/AnalysisTree.cmake`:
```cmake
# cmake/AnalysisTreeQA.cmake
if(MyRepo_BUNDLED_ATQA)
    include(FetchContent)
    FetchContent_Declare(AnalysisTreeQA
            GIT_REPOSITORY https://github.com/HeavyIonAnalysis/AnalysisTreeQA.git
            GIT_TAG <tag>)
    set(AnalysisTreeQA_BUILD_SERVICES ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(AnalysisTreeQA)
else()
    find_package(AnalysisTreeQA REQUIRED)
endif()
```
`find_package(AnalysisTreeQA)` also resolves ROOT/AnalysisTree/yaml-cpp/Boost
for you (see `AnalysisTreeQAConfig.cmake.in`) - you don't need to separately
`find_package()` each of them yourself.

### 2. Your own private task classes - same rules as `../tasks/`

Write your task classes exactly as described in
[`../tasks/README.md`](../tasks/README.md) (derive from
`AnalysisTree::QA::Task`, `explicit YourTask(const YAML::Node&)` constructor,
book in `Init()`, `ATQA_REGISTER_TASK(YourTask)`). Build them as an `OBJECT`
library, not `STATIC` - the same self-registration/linker-pruning reasoning
from `../tasks/README.md` applies identically here, just across a package
boundary instead of within one repo:
```cmake
add_library(MyTasks OBJECT tasks/MyDetectorQaTask.cpp)
target_link_libraries(MyTasks PUBLIC AnalysisTreeQASteerCore)
```

### 3. A trivial `main.cpp`

Copy `services/app/main.cpp` from this repository (~20 lines) - it only
parses options and drives `Application`, nothing analysis-specific:
```cmake
add_executable(my_analysis_qa app/main.cpp)
target_link_libraries(my_analysis_qa PRIVATE AnalysisTreeQAAppCore AnalysisTreeQABuiltinTasks MyTasks)
```

### 4. Your own YAML configs

Reference AnalysisTreeQA's built-in types (`TrackQA`, `HistogramQA`) and your
own private ones by name, exactly like any config in `../services/config/` -
the self-registering `TaskFactory` doesn't distinguish where a task type came
from.
