#ifndef ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_
#define ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_

#include <gtest/gtest.h>
#include <random>

#include "AnalysisTree/ToyMC.hpp"

#include "AnalysisTree/TaskManager.hpp"
#include "Task.hpp"
#include "BasicQA.hpp"

namespace {

using namespace AnalysisTree;

TEST(Test_AnalysisTreeQA, Test_Manager) {

  const int n_events = 1000;  // TODO propagate somehow
  std::string filename = "toymc_analysis_task.root";
  std::string treename = "tTree";
  std::string filelist = "fl_toy_mc.txt";

  auto* man = TaskManager::GetInstance();

  auto* toy_mc = new ToyMC<std::default_random_engine>;
  man->AddTask(toy_mc);
  man->SetOutputName(filename, treename);

  man->Init();
  man->Run(n_events);
  man->Finish();

  std::ofstream fl(filelist);
  fl << filename << "\n";
  fl.close();

  auto* task = new QA::Task;
  Cuts eta_cut("eta_cut", {RangeCut("SimParticles.eta", -1, 1)});

  AddParticleQA(task, "SimParticles");
  AddTrackQA(task, "RecTracks");
  AddTracksMatchQA(task, "RecTracks", "SimParticles");
  AddParticlesFlowQA(task, "SimParticles", {"SimEventHeader", "psi_RP"}, {211});

  man->AddTask(task);

  man->Init({filelist}, {treename});
  man->Run(-1);
  man->Finish();

  TFile f_qa("QA.root", "read");

  auto* h_sim_y = (TH1*)f_qa.Get("SimParticles/SimParticles_rapidity");

  EXPECT_NEAR(h_sim_y->GetEntries(), 100*n_events, 100);
//  EXPECT_NEAR(h_sim_y->GetMean(), 0, 0.01);
//  EXPECT_NEAR(h_sim_y->GetStdDev(), 1., 0.05);

  auto* h_rec_px = (TH1*)f_qa.Get("RecTracks/RecTracks_px");

  EXPECT_NEAR(h_rec_px->GetEntries(), 100*n_events, 100);
  EXPECT_NEAR(h_rec_px->GetMean(), 0, 0.01);
//  EXPECT_NEAR(h_rec_px->GetStdDev(), 1., 0.5);

}

}

#endif //ANALYSISTREEQA_SRC_MANAGER_TEST_HPP_
