#include <gtest/gtest.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <vector>

#include "Manager.h"

TEST(Test_AnalysisTreeCore, Test_SimpleTest) {
  int a=1;
  ASSERT_EQ(a, 1);
}

TEST(Test_AnalysisTreeCore, Test_TH1_QA) {

  AnalysisTree::QA::Manager man({"../../input/filelist.txt"}, {"aTree"});
  man.SetOutFileName("test_th1.root");
  man.AddEntry({"VtxTracks", "px"}, {100, -3, 3});
  man.Run(10);

//  std::unique_ptr<TFile> qa_file{ TFile::Open("test_th1.root", "read")};
//  auto* histo = (TH1F*) qa_file->Get("VtxTracks/VtxTracks_px");
//  EXPECT_GT(histo->GetEntries(), 0);
}

TEST(Test_AnalysisTreeCore, Test_TH2_QA) {
  AnalysisTree::QA::Manager man({"../../input/filelist.txt"}, {"aTree"});
  man.SetOutFileName("test_th2.root");
  man.AddEntry2D({{"VtxTracks", "px"}, {"VtxTracks", "py"}}, {{100,-3,3}});
  man.Run(10);

  std::unique_ptr<TFile> qa_file{ TFile::Open("test_th2.root", "read")};

  auto* histo = (TH2F*) qa_file->Get("VtxTracks/VtxTracks_px_py");
  EXPECT_GT(histo->GetEntries(), 0);
}

TEST(Test_AnalysisTreeCore, Test_Cuts_QA) {

  AnalysisTree::Cuts p_cut("p_cut", "VtxTracks");
  AnalysisTree::SimpleCut px_cut("px", -0.5, 0.5);
  AnalysisTree::SimpleCut py_cut("py", -0.5, 0.5);
  p_cut.AddCuts( {px_cut, py_cut} );

  AnalysisTree::QA::Manager man("../../input/filelist.txt", "aTree");
  man.SetOutFileName("test_cuts.root");
  man.AddBranchCut(p_cut);
  man.AddEntry2D({{"VtxTracks", "px"}, {"VtxTracks", "py"}}, {{100,-3,3}});
  man.Run(10);

  std::unique_ptr<TFile> qa_file{ TFile::Open("test_cuts.root", "read")};

  auto* histo = (TH2F*) qa_file->Get("VtxTracks/VtxTracks_px_py");
  EXPECT_GT(histo->GetEntries(), 0);
}


