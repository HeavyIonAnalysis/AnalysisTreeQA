#ifndef ANALYSISTREE_QA_MANAGER_H
#define ANALYSISTREE_QA_MANAGER_H

#include "AnalysisTree/TaskManager.hpp"

namespace AnalysisTree {
namespace QA {

class Task;

/**
 * Manager class for a QA
 */
class Manager : public TaskManager {

 public:
  Manager(const std::vector<std::string>& filelists,
          const std::vector<std::string>& in_trees) : TaskManager(filelists, in_trees) {}

  void AddTask(QA::Task* task);
};

}// namespace QA
}// namespace AnalysisTree
#endif//ANALYSISTREE_QA_MANAGER_H
