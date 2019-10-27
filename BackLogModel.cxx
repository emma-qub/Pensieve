#include "BackLogModel.hxx"

#include "TaskItem.hxx"

BackLogModel::BackLogModel(QObject* p_parent):
  QStandardItemModel(p_parent) {
}

void BackLogModel::AppendTaskItem(TaskItem* p_taskItem) {
  appendRow(p_taskItem);
}

TaskItem* BackLogModel::GetTaskItemFromIndex(const QModelIndex& p_index) const {
  return static_cast<TaskItem*>(itemFromIndex(p_index));
}
