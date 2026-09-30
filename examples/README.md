# Examples

## Contents
- [1. Macro API](#1-macro-api)
- [2. Task-based binary](#2-task-based-binary)
  - [2.1 Fork AnalysisTreeQA and add your task here](#21-fork-analysistreeqa-and-add-your-task-here)
  - [2.2 As an independent external package](#22-as-an-independent-external-package)
  - [2.3 Writing your own tasks, plots and cuts](#23-writing-your-own-tasks-plots-and-cuts)

Pick whichever fits: the Macro API needs nothing but ROOT + AnalysisTree and is the quickest for a one-off, throwaway check; the task-based binary needs more setup but gives you a reusable, YAML-configurable executable, without writing or compiling a macro for every change.

## 1. Macro API

`example.cpp` is a self-contained C++ macro: it instantiates `AnalysisTree::QA::Task` directly, books histograms/cuts inline in C++, and drives the whole `Init()`/`Run()`/`Finish()` sequence itself: no YAML, no `services/`, and critically no `yaml-cpp`/Boost dependency. This is the same `AnalysisTree::QA::Task` class the task-based binary (section 2) uses under the hood; that binary is an alternative *driver* for it, not a different histogram-booking API.

Build (default, no extra flags needed):
```bash
cmake -DAnalysisTreeQA_BUILD_EXAMPLES=ON ..   # default is already ON
make -j install
```
Run:
```bash
./example my_filelist.txt
```
Read `example.cpp` itself for the full picture: it walks through `AddH1`/`AddH2`/`AddProfile`/`AddIntegral`, cuts built directly as `AnalysisTree::Cuts`/`SimpleCut`/`RangeCut`/`EqualsCut`, and a custom `AnalysisTree::Variable` for a derived quantity (e.g. chi2/ndf) that the declarative YAML cut types (section 2.3) can't express.

## 2. Task-based binary

`analysistreeqa` is a compiled CLI binary (built via `-DAnalysisTreeQA_BUILD_SERVICES=ON`, see [`../services/README.md`](../services/README.md) for its YAML schema and CLI flags) that runs a configurable set of QA tasks purely from a YAML config file. No macro to write or compile for every change. Your own task *types* still need C++ + a rebuild (section 2.3), but the binary itself, and which tasks/branches/cuts actually run on a given dataset, is chosen entirely from YAML.

Two ways to get your own task type into that binary, pick whichever fits:

### 2.1 Fork AnalysisTreeQA and add your task here

Simplest for a quick, local, one-off analysis: `analysistreeqa` is built right there in the same repo/branch, no separate repository to set up.

1. Fork/clone this repository.
2. Add your `.hpp`/`.cpp` pair right here in `examples/`, following `ExampleEventHeaderTask`/`ExampleParticleTask` as a template (see section 2.3 for what goes in them).
3. Add the new `.cpp` to `AnalysisTreeQAExampleTasks`'s source list in [`CMakeLists.txt`](CMakeLists.txt) (it's an explicit list, so this one line is required).
4. Rebuild with `-DAnalysisTreeQA_BUILD_SERVICES=ON -DAnalysisTreeQA_BUILD_EXAMPLES=ON` (the latter is already the default).

Your class is now selectable via `task: YourTaskName` in any YAML config `analysistreeqa` reads. Bare `#include "Task.hpp"` works fine here, inside this repo (see the note in 2.3 for why that's different in 2.2).

### 2.2 As an independent external package

Better for an analysis that should be versioned independently and shouldn't live inside/track AnalysisTreeQA's own git history. Depend on AnalysisTreeQA as an external package instead of forking it: the same way the sibling repository `Centrality` depends on the base `AnalysisTree` library. You still get the full `services/` infrastructure (the self-registering `TaskFactory`/`CutFactory`, the built-in task types, the CLI/config plumbing in `AnalysisTreeQAAppCore`) without copying any of it.

**Step 1: point your CMake at an installed AnalysisTreeQA.** `find_package(AnalysisTreeQA)` needs an install built with `-DAnalysisTreeQA_BUILD_SERVICES=ON`. A dual-mode `cmake/AnalysisTreeQA.cmake` module (find a pre-installed copy, or fetch and build one), modeled on `Centrality`'s own `cmake/AnalysisTree.cmake`:

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
`find_package(AnalysisTreeQA)` also resolves ROOT/AnalysisTree/yaml-cpp/Boost for you (see `AnalysisTreeQAConfig.cmake.in`), you don't need to separately `find_package()` each of them yourself.

**Step 2: your own task classes as an `OBJECT` library.** Write them as described in section 2.3, then:

```cmake
add_library(MyTasks OBJECT tasks/MyDetectorQaTask.cpp)
target_link_libraries(MyTasks PUBLIC AnalysisTreeQASteerCore)
```
`OBJECT`, not `STATIC`, see the note in section 2.3.

**Step 3: a trivial `main.cpp`.** Copy `../services/app/main.cpp` from this repository (~20 lines), it only parses options and drives `Application`, nothing analysis-specific:

```cmake
add_executable(my_analysis_qa app/main.cpp)
target_link_libraries(my_analysis_qa PRIVATE AnalysisTreeQAAppCore AnalysisTreeQABuiltinTasks MyTasks)
```

**Step 4: your own YAML configs.** Reference AnalysisTreeQA's built-in types (`TrackQaTask`, `HistogramQaTask`) and your own private ones by name, exactly like any config in `../services/config/`, the self-registering `TaskFactory` doesn't distinguish where a task type came from.

### 2.3 Writing your own tasks, plots and cuts

Whichever of 2.1/2.2 you picked, a task class itself looks the same. `ExampleEventHeaderTask.hpp`/`.cpp` and `ExampleParticleTask.hpp`/`.cpp` right here are worked examples, each books one `AddH1`, one `AddH2` and one `AddProfile` directly in `Init()` (no `BasicQA.hpp` helper in between, unlike the built-in `TrackQaTask`), specifically so the code is simple to read and copy from.

1. Derives from `AnalysisTree::QA::Task`. **`#include "AnalysisTreeQA/Task.hpp"` (qualified) if you're in your own repository (2.2)**, or the bare `#include "Task.hpp"` if you're inside this repo (2.1, as `ExampleEventHeaderTask.hpp`/`ExampleParticleTask.hpp` do). 
2. Has a constructor `explicit YourTask(const YAML::Node& node)`. `node` is the *entire* YAML entry for this task (its `task`, `name`, and whatever custom fields you define). Build cuts with `AnalysisTree::QA::CutFactory::Instance().BuildCuts(node["cuts"], "some_default_name")` to get the same declarative range/equals/not_equals/or/named/custom-cut support every built-in task has.
3. Overrides `void Init() override`, where it books histograms/cuts (`AddH1`/`AddH2`/`AddProfile`/`AddIntegral`), finishing with `AnalysisTask::Init();`. Booking must happen here, not in the constructor: the shared output file and top-level directory are only attached to the task after construction but before `Init()` runs.
4. Registers itself at the bottom of the `.cpp` file, **inside** the `namespace AnalysisTree { namespace QA { ... } }` block, using the plain unqualified class name:

   ```cpp
   ATQA_REGISTER_TASK(YourTask)
   ```
   This registers `"YourTask"` as a `task:` name any YAML config can use, the moment this `.cpp` is linked into the binary.

Build it as an `OBJECT` library, not `STATIC`: `ATQA_REGISTER_TASK` relies on a static object constructor running before `main()`; since the class is only ever found by name at runtime, a plain static archive would let the linker drop the translation unit as "unused" (this is why `examples/CMakeLists.txt`'s `AnalysisTreeQAExampleTasks` is an `OBJECT` library too, not just a plain source list on the `analysistreeqa` executable itself).

**Plots without writing a task at all**: for an arbitrary branch/field combination, `HistogramQaTask` lets you declare `h1`/`h2`/`profile` plots directly in YAML, no C++ needed, see [`../services/README.md`](../services/README.md#histogramqatask-arbitrary-branchfield-combinations-no-new-c-needed). Write a task class only for something that needs actual C++ logic beyond picking branch/field/cuts.

**Cuts**: the declarative `range`/`equals`/`not_equals`/`or` types cover most conditions on one field, see [`../services/README.md`](../services/README.md) for the full YAML schema. For something they can't express (e.g. a ratio of two fields), register a named C++ predicate once via `ATQA_REGISTER_CUT` (see `CutFactory.hpp`), typically right in your own task's `.cpp` file:

```cpp
ATQA_REGISTER_CUT(MyCut, [](const YAML::Node& params) {
  return AnalysisTree::SimpleCut({"Branch.field"}, [](std::vector<double> v) { return v[0] > 0; }, "MyCut");
});
```
and reference it from YAML as `{type: custom, name: MyCut}` inside any `simple_cuts:` list.
