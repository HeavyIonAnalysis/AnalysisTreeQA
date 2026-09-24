# Private QA tasks

Drop your own QA task class here (a `.hpp` + `.cpp` pair) to make it selectable from a `services/` YAML config by name, without editing any central file. This only works when the project is built with `-DAnalysisTreeQA_BUILD_SERVICES=ON -DAnalysisTreeQA_BUILD_TASKS=ON`. See main `README.md` and `services/README.md` for the full picture.

`tasks/ExampleUserTask.hpp`/`.cpp` is a minimal worked example. You can copy it, rename the class, and adjust the `Init()` function.

## How to setup the tasks

1. Your class derives from `AnalysisTree::QA::Task` (`src/Task.hpp`), the same class the classic macro API (`examples/example.cpp`) uses directly.
2. It has a constructor `explicit YourTask(const YAML::Node& node)`. `node` is the *entire* YAML entry for this task (its `type`, `name`, and whatever custom fields you define).
   Cuts: build them with `AnalysisTree::QA::CutFactory::Instance().BuildCuts(node["cuts"], "some_default_name")` (see `services/steer/CutFactory.hpp`) to get the same declarative range/equals/named/custom-lambda cut support every built-intask has.
3. It overrides `void Init() override`, where it books its histograms/cuts (`AddH1`/`AddH2`/`AddProfile`/`AddIntegral`, or any `BasicQA.hpp` helper), and finishes by calling `AnalysisTask::Init();`. Booking must happen here, not in the constructor. The shared output file and top-level directory are only attached to the task after construction but before `Init()` runs.
4. At the very bottom of the `.cpp` file, **inside** the `namespace AnalysisTree { namespace QA { ... } }` block, add one line: 
   ```cpp
   ATQA_REGISTER_TASK(YourTask)
   ```
   using the plain, unqualified class name (not `AnalysisTree::QA::YourTask`). This registers `"YourTask"` as a `type:` name any YAML config can use, the moment this `.cpp` is linked into `analysistreeqa` - no other file needs to change.

## Why it has to be `.cpp`, not header-only

`ATQA_REGISTER_TASK` relies on a static object constructor running before `main()`. If your task type is never actually referenced anywhere else in the program (its found by name at runtime only), a plain static archive would let the linker drop that translation unit as "unused". This directory is built as a CMake `OBJECT` library specifically to avoid that, just add your files here and rebuild; you don't need to touch `tasks/CMakeLists.txt`.

## Custom cuts

If a cut can't be expressed as a declarative range/equals check, register a named C++ predicate once (typically in its own small `.cpp` here, following `services/steer/tasks/BuiltinCuts.cpp` as a template):
```cpp
ATQA_REGISTER_CUT(MyCut, [](const YAML::Node& params) {
  return AnalysisTree::SimpleCut({"Branch.field"}, [](std::vector<double> v) { return v[0] > 0; }, "MyCut");
});
```
and reference it from YAML as `{type: custom, name: MyCut}` inside any `simple_cuts:` list.
