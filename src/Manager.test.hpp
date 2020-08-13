#ifndef ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_
#define ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_

#include <gtest/gtest.h>

// Coming soon...

TEST(Test_AnalysisTreeQACore, Test_Test) {
  int a=1;
  ASSERT_EQ(a,1);
}

//TEST(Test_AnalysisTreeCore, Test_TH1_QA) {
//
//  AnalysisTree::QA::Manager man({"../../input/filelist.txt"}, {"aTree"});
//  man.SetOutFileName("test_th1.root");
//
//  auto *task = new AnalysisTree::QA::Task;
//
//  task->AddH1({"p_{x}", {"VtxTracks", "px"}, {100, -3, 3}});
//  man.AddTask(task);
//
//  man.Run(10);
//}
//
//TEST(Test_AnalysisTreeCore, Test_TH2_QA) {
//  AnalysisTree::QA::Manager man({"../../input/filelist.txt"}, {"aTree"});
//  man.SetOutFileName("test_th2.root");
//
//  auto *task = new AnalysisTree::QA::Task;
//  task->AddH2({"p_{x}", {"VtxTracks", "px"}, {100, -3, 3}}, {"p_{y}", {"VtxTracks", "py"}, {100, -3, 3}});
//  man.AddTask(task);
//
//  man.Run(10);
//
//  std::unique_ptr<TFile> qa_file{TFile::Open("test_th2.root", "read")};
//
//  auto *histo = (TH2F *) qa_file->Get("VtxTracks/VtxTracks_px_py");
//  EXPECT_GT(histo->GetEntries(), 0);
//}
//
//TEST(Test_AnalysisTreeCore, Test_Cuts_QA) {
//
//  AnalysisTree::SimpleCut px_cut({"VtxTracks", "px"}, -0.5, 0.5);
//  AnalysisTree::SimpleCut py_cut({"VtxTracks", "py"}, -0.5, 0.5);
//
//  auto *p_cut = new AnalysisTree::Cuts("p_cut", {px_cut, py_cut});
//
//  AnalysisTree::QA::Manager man({"../../input/filelist.txt"}, {"aTree"});
//  man.SetOutFileName("test_cuts.root");
//  man.AddBranchCut(p_cut);
//
//  auto *task = new AnalysisTree::QA::Task;
//  task->AddH2({"p_{x}", {"VtxTracks", "px"}, {100, -3, 3}}, {"p_{y}", {"VtxTracks", "py"}, {100, -3, 3}});
//  man.AddTask(task);
//
//  man.Run(10);
//
//  std::unique_ptr<TFile> qa_file{TFile::Open("test_cuts.root", "read")};
//
//  auto *histo = (TH2F *) qa_file->Get("VtxTracks/VtxTracks_px_py");
//  EXPECT_GT(histo->GetEntries(), 0);
//}

#endif //ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_
