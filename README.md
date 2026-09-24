# AnalysisTreeQA
![Workflow status](https://github.com/HeavyIonAnalysis/AnalysisTreeQA/workflows/CMake/badge.svg)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/HeavyIonAnalysis/AnalysisTreeQA?sort=semver)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/HeavyIonAnalysis/AnalysisTreeQA)

## Doxygen documentation:

https://heavyionanalysis.github.io/AnalysisTreeQA/

## General information

This is a package for Quality Assurance and some basic analysis of AnalysisTree data.

## Installation

ROOT6 is needed for installation. Version compiled with c++17 flag is preferred, otherwise CMAKE_CXX_STANDARD flag needs to be implicitly specified (see below).

    git clone git@github.com:HeavyIonAnalysis/AnalysisTreeQA.git
    cd AnalysisTreeQA
    mkdir build install
    cd build
    source /path/to/your/thisroot.sh
    cmake -DCMAKE_INSTALL_PREFIX=../install ../
    make -j install

### List of CMake options:

To apply the flag use -D{Name}={value}, for example, if you want to compile using c++11:

    cmake -DCMAKE_CXX_STANDARD=11 ../

| Name  | Default value | Possible values |
| ------------- | ------------- | ---------- |
| CMAKE_BUILD_TYPE  | RELEASE  | RELEASE/DEBUG |
| CMAKE_CXX_STANDARD  | 17  | 11/14/17 |
| AnalysisTreeQA_BUILD_TESTS  | ON  | ON/OFF |
| AnalysisTreeQA_BUILD_SERVICES | OFF | ON/OFF |
| AnalysisTreeQA_BUILD_TASKS  | OFF  | ON/OFF |
| AnalysisTreeQA_BUNDLED_AT  | ON  | ON/OFF |
| AnalysisTreeQA_BUNDLED_AT_VERSION  | v2.2.8  | master/v2.0.1/... |
| AnalysisTreeQA_BUNDLED_AT_GIT_SHALLOW  | ON  | ON/OFF |

If AnalysisTreeQA_BUNDLED_AT_VERSION variable shows not on the tag (but branch name or commit hash) then AnalysisTreeQA_BUNDLED_AT_GIT_SHALLOW variable must be set OFF.

### Optional: CLI + YAML task framework (services/)

The classic macro-based workflow below (`examples/example.cpp`, `src/BasicQA.hpp`) only ever needs ROOT and AnalysisTree, exactly as described above. On top of it, AnalysisTreeQA optionally provides a compiled CLI binary (`analysistreeqa`) that runs a configurable set of QA tasks purely from a YAML config file - no macro to write or compile. This is opt-in via `-DAnalysisTreeQA_BUILD_SERVICES=ON`, and additionally needs `yaml-cpp` and Boost's `program_options` component (`AnalysisTreeQA_BUILD_TASKS=ON`, for writing your own private task classes, needs the same two and requires `AnalysisTreeQA_BUILD_SERVICES=ON`). See [`services/README.md`](services/README.md) for the full tutorial.

#### Where should your own QA tasks live?

Two supported options, pick whichever fits:
- **Fork this repository, add to its own [`tasks/`](tasks/README.md)**: simplest for a quick, local, one-off analysis; `analysistreeqa` is built right there in the same repo/branch (`-DAnalysisTreeQA_BUILD_SERVICES=ON -DAnalysisTreeQA_BUILD_TASKS=ON`).
- **Your own, separate repository, with AnalysisTreeQA as a dependency**: better for an analysis that shouldn't live inside/track AnalysisTreeQA's own git history, and can be versioned independently. See [`services/README.md`](services/README.md#using-analysistreeqa-from-your-own-repository), "Using AnalysisTreeQA from your own repository."

## Usage

Simple program: 

    // Manager instance: filelist1, filelist2, ...; treename1, treename2, ...
    QA::Manager man({filelist1, filelist2}, {"aTree", "bTree"}); 
    man.SetOutFileName("test_qa.root");

    auto* task = QA::Task();

    // 1D histo: {branch_name, field_name}, {nbins, min, max}
    task->AddH1({"N_{hits}", {"VtxTracks", "nhits"}, {15, 0, 15}}); 

    // AnalysisTree::Variable in case of more complicated plot
    Variable chi2_over_ndf("chi2/ndf", {{"VtxTracks", "chi2"}, {"VtxTracks", "ndf"}}, []( std::vector<double>& var ) { return var.at(0)/var.at(1); });
    task->AddH1({"#chi^{2}/NDF", chi2_over_ndf, {100, 0, 100}}); 

    // 2D histo:
    task->AddH2({"DCA_{x}", {"VtxTracks", "dcax"}, {100, -1, 1}}, {"DCA_{y}", {"VtxTracks", "dcay"}, {100, -1, 1}}); 

    // Histogramm with additional cut:
    Cuts* mc_protons = new Cuts("McProtons", {{"SimTracks","pid"}, 2212});
    task->AddH1({"N_{hits}", {"VtxTracks", "nhits"}, {15, 0, 15}}, mc_protons); 

    man.Run();

## Known problems

 - Matching between 3 branches is not available (i. e. TofHits.m2 vs. VtxTracks.p with cut on MC-true protons). This is a limitation of the AnalysisTree dependency itself (`AnalysisTree::Matching` only ever correlates two independent multi-object branches) - it also applies to the YAML-declarative `HistogramQA` task in `services/` (see `services/README.md`), not just the C++ macro API.
