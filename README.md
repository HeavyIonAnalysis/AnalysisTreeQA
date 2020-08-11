# AnalysisTreeQA

## Installation

    git clone git@git.cbm.gsi.de:pwg-c2f/data/analysis_tree_qa.git
    (https://git.cbm.gsi.de/pwg-c2f/data/analysis_tree_qa.git)
    cd analysis_tree_qa
    mkdir build install
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ../
    make -j install
    
## Usage

CBM and NA61/SHINE examples one can find in tasks directory.
Simple program: 

    // Manager instance: filelist, treename
    QA::Manager man(filelist, "aTree"); 

    // 1D histo: {branch_name, field_name}, {nbins, min, max}
    man.AddEntry({"VtxTracks", "nhits"}, {15, 0, 15}); 

    // AnalysisTree::Variable in case of more complicated plot
    Variable chi2_over_ndf("chi2/ndf", "VtxTracks", {"chi2", "ndf"}, []( std::vector<double>& var ) { return var.at(0)/var.at(1); });
    man.AddEntry(chi2_over_ndf, {100, 0, 100});

    // 2D histo:
    man.AddEntry2D({{"VtxTracks", "dcax"}, {"VtxTracks", "dcay"}}, {{nbins, -1, 1}});

    // Histogramm with additional cut:
    Cuts* mc_protons = new Cuts("McProtons", "SimTracks");
    mc_protons->AddCut( {"pid", 2212-0.5, 2212+0.5} );
    man.AddEntry({"VtxTracks", "dcax"}, {nbins, -1, 1}, mc_protons);

    man.Run();

## Known problems

 - AnalysisTree::Variable can be constructed using fields from only 1 branch
 - Matching between 3 branches is not available (i. e. TofHits.m2 vs. VtxTracks.p with cut on MC-true protons) 
