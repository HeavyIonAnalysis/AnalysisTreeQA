#include "Manager.hpp"
#include "Task.hpp"
#include <AnalysisTree/Cuts.hpp>
#include <AnalysisTree/SimpleCut.hpp>

int main(int argc, char** argv) {
  using namespace AnalysisTree;
  using QA::Manager;
  using QA::Task;

  const std::string event_header_branch{"RecEventHeader"};

  assert(argc == 2);

  std::string filelist{argv[1]};
  std::string tree_name{"aTree"};

  Manager manager({filelist}, {tree_name});
  manager.SetOutFileName("qa.root");

  auto cuts_t2 = new Cuts("t2",{AnalysisTree::SimpleCut(Variable(event_header_branch,"t2"), 1.)});
  auto cuts_t4 = new Cuts("t4",{AnalysisTree::SimpleCut(Variable(event_header_branch,"t4"), 1.)});

  auto cuts_t4_wfa = new Cuts("t4_wfa",{
                                            AnalysisTree::SimpleCut(Variable(event_header_branch,"t4"), 1.),
                                            AnalysisTree::SimpleCut(Variable(event_header_branch,"wfa_s1"), 4000., 1e9),
                                            AnalysisTree::SimpleCut(Variable(event_header_branch,"wfa_t4"), 4000., 1e9),
                                        });
  auto cuts_t4_vtx_z_def = new Cuts("t4_vtx_z_def",{
      AnalysisTree::SimpleCut(Variable(event_header_branch,"t4"), 1.),
      AnalysisTree::SimpleCut(Variable(event_header_branch,"vtx_z"), -594., -590.),
  });


  auto task = new Task;
  task->AddH1({"vertex fit OK", {event_header_branch, "fitted_vtx"}, {11, -1, 10}});
  task->AddH1({"x_{vertex} (cm)", {event_header_branch, "vtx_x"}, {200, -1., 1.}});
  task->AddH1({"y_{vertex} (cm)", {event_header_branch, "vtx_y"}, {200, -1., 1.}});
  task->AddH1({"z_{vertex} (cm)", {event_header_branch, "vtx_z"}, {2000, -600., -580.}});
  task->AddH2(
      {"x_{vertex} (cm)", {event_header_branch, "vtx_x"}, {200, -1., 1.}},
      {"y_{vertex} (cm)", {event_header_branch, "vtx_y"}, {200, -1., 1.}});
  task->AddH2(
      {"z_{vertex} (cm)", {event_header_branch, "vtx_z"}, {2000, -600., -580.}},
      {"x_{vertex} (cm)", {event_header_branch, "vtx_x"}, {200, -1., 1.}});
  task->AddH1({"x_{BPD3} (cm)", {event_header_branch, "bpd3_x"}, {200, -1., 1.}});
  task->AddH1({"y_{BPD3} (cm)", {event_header_branch, "bpd3_y"}, {200, -1., 1.}});
  task->AddH2(
      {"x_{BPD3} (cm)", {event_header_branch, "bpd3_x"}, {200, -1., 1.}},
      {"x_{vertex} (cm)", {event_header_branch, "vtx_x"}, {200, -1., 1.}});
  task->AddH2(
      {"y_{BPD3} (cm)", {event_header_branch, "bpd3_y"}, {200, -1., 1.}},
      {"y_{vertex} (cm)", {event_header_branch, "vtx_y"}, {200, -1., 1.}});
  task->AddH1({"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}});
  task->AddH1({"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}}, cuts_t2);
  task->AddH1({"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}}, cuts_t4);
  task->AddH1({"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}}, cuts_t4_wfa);
  task->AddH1({"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}}, cuts_t4_vtx_z_def);
  task->AddH2(
      {"z_{vertex} (cm)", {event_header_branch, "vtx_z"}, {2000, -600., -580.}},
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}});
  task->AddH1({"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}});
  task->AddH1({"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}}, cuts_t2);
  task->AddH1({"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}}, cuts_t4);
  task->AddH1({"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}}, cuts_t4_wfa);
  task->AddH1({"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}}, cuts_t4_vtx_z_def);
  task->AddH2(
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}},
      {"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}});
  task->AddH2(
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}},
      {"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}},cuts_t2);
  task->AddH2(
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}},
      {"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}},cuts_t4);
  task->AddH2(
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}},
      {"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}},cuts_t4_wfa);
  task->AddH2(
      {"TPC multiplicity", {event_header_branch, "M"}, {1000, 0, 1000}},
      {"E_{PSD} (GeV/c)", {event_header_branch, "Epsd"}, {1000, 0, 10000}},cuts_t4_vtx_z_def);
  task->AddH1({"WFA(S1)", {event_header_branch, "wfa_s1"}, {100, 0, 30000}});
  task->AddH1({"WFA(T4)", {event_header_branch, "wfa_t4"}, {100, 0, 100 * 1e6}});
  task->AddH1({"T2", {event_header_branch, "t2"}, {6, -1, 5}});
  task->AddH1({"T4", {event_header_branch, "t4"}, {6, -1, 5}});
  manager.AddTask(task);

  manager.Init();
  manager.Run(-1);
  manager.Finish();
  return 0;
}