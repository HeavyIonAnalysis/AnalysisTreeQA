#ifndef ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_
#define ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_

#include <gtest/gtest.h>
#include <random>

#include "AnalysisTree/ToyMC.hpp"

#include "Manager.hpp"
#include "Task.hpp"
#include "Utils.hpp"

namespace {

using namespace AnalysisTree;

TEST(Test_AnalysisTreeQA, Test_Test) {
  int a=1;
  ASSERT_EQ(a,1);
}

TEST(Test_AnalysisTreeQA, Test_Manager) {

  const int n_events = 1000;

  ToyMC<std::default_random_engine> toy_mc;
  toy_mc.GenerateEvents(n_events);
  toy_mc.WriteToFile("toy_mc.root", "fl_toy_mc.txt");

  QA::Manager man({"fl_toy_mc.txt"}, {"tTree"});
  man.SetOutFileName("test_qa.root");

  auto* task = new QA::Task;
  Cuts eta_cut("eta_cut", {SimpleCut({"SimParticles", "eta"}, -1, 1)});

  AddParticleQA(task, "SimParticles");
  AddTrackQA(task, "RecTracks");
  AddTracksMatchQA(task, "RecTracks", "SimParticles");
  AddParticlesFlowQA(task, "SimParticles", {"SimEventHeader", "psi_RP"}, {211});

  man.AddTask(task);

  man.Init();
  man.Run(-1);
  man.Finish();

  TFile f_qa("test_qa.root", "read");

  auto* h_sim_y = f_qa.Get<TH1>("SimParticles/SimParticles_rapidity");

  EXPECT_NEAR(h_sim_y->GetEntries(), 100000, 100);
  EXPECT_NEAR(h_sim_y->GetMean(), 0, 0.01);
//  EXPECT_NEAR(h_sim_y->GetStdDev(), 1., 0.05);

  auto* h_rec_px = f_qa.Get<TH1>("RecTracks/RecTracks_px");

  EXPECT_NEAR(h_rec_px->GetEntries(), 100000, 100);
  EXPECT_NEAR(h_rec_px->GetMean(), 0, 0.01);
//  EXPECT_NEAR(h_rec_px->GetStdDev(), 1., 0.5);

}

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
