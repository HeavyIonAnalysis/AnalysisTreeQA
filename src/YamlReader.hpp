#ifndef ANALYSISTREEQA_YAMLREADER_HPP
#define ANALYSISTREEQA_YAMLREADER_HPP

#include <yaml-cpp/yaml.h>
#include <regex>
#include <utility>
#include <fstream>
#include <iostream>
#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

class YamlReader {
 public:
  explicit YamlReader(std::string filename) : filename_(std::move(filename)) {}

  static Axis ConstructAxis(const YAML::Node& yaml_axis, Task* task){
    auto field = yaml_axis["field"].as<std::string>();
    auto title = yaml_axis["title"] ? yaml_axis["title"].as<std::string>() : field;
    if(yaml_axis["bins"]){
      auto bins = yaml_axis["bins"].as<std::vector<double>>();
      return {title, task->GetVariable(field),
              {static_cast<Int_t>(bins.at(0)), bins.at(1), bins.at(2)}};
    }
    else if(yaml_axis["bin_edges"]){
      auto bins = yaml_axis["bin_edges"].as<std::vector<double>>();
      return {title, task->GetVariable(field),
              TAxis(bins.size()-1, &(bins[0]))};
    }
    throw std::runtime_error("No axis bins are specified: " + title);
  }

  void AddPlotsFromYaml(Task* task){
    assert(!filename_.empty());
    YAML::Node config = YAML::LoadFile(filename_);

    Add1DPlot(config["1D"], task);
    Add1DPlot(config["Integral"], task, true);

    Add2DPlot(config["2D"], task, EntryConfig::PlotType::kHisto2D);
    Add2DPlot(config["Profile"], task, EntryConfig::PlotType::kProfile);
    Add2DPlot(config["Integral2D"], task, EntryConfig::PlotType::kIntegral2D);

  }

  static void Add1DPlot(const YAML::Node& node, Task* task, bool is_integral = false) {
    assert(task && node);

    for(auto plot : node){
      auto name = plot["name"] ? plot["name"].as<std::string>() : "";
      auto cuts = plot["cuts"] ? plot["cuts"].as<std::string>() : "";
      auto a = ConstructAxis(plot["x_axis"], task);
      Cuts* c = !cuts.empty() ? task->GetCut(cuts) : nullptr;
      is_integral ? task->AddIntegral(a, c, name) : task->AddH1(a, c, name);
    }
  }

  static void Add2DPlot(const YAML::Node& node, Task* task, EntryConfig::PlotType type) {
    assert(task && node);

    for(auto plot : node) {
      auto name = plot["name"] ? plot["name"].as<std::string>() : "";
      auto x = ConstructAxis(plot["x_axis"], task);
      auto y = ConstructAxis(plot["y_axis"], task);

      auto cuts = plot["cuts"] ? plot["cuts"].as<std::string>() : "";
      auto cuts_x = plot["x_cuts"] ? plot["x_cuts"].as<std::string>() : "";
      auto cuts_y = plot["y_cuts"] ? plot["y_cuts"].as<std::string>() : "";

      Cuts* c = !cuts.empty() ? task->GetCut(cuts) : nullptr;
      Cuts* c_x = !cuts_x.empty() ? task->GetCut(cuts_x) : nullptr;
      Cuts* c_y = !cuts_y.empty() ? task->GetCut(cuts_y) : nullptr;

      switch (type) {
        case EntryConfig::PlotType::kHisto2D: task->AddH2(x, y, c, name); break;
        case EntryConfig::PlotType::kIntegral2D: task->AddIntegral(x, y, c_x, c_y, name); break;
        case EntryConfig::PlotType::kProfile: task->AddProfile(x, y, c, name); break;
        default: throw std::runtime_error("Wrong plot type");
      }
    }
  }


 protected:
  std::string filename_;
};

}
}
#endif//ANALYSISTREEQA_YAMLREADER_HPP
