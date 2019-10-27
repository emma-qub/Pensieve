#include "BackLogProxyModel.hxx"

#include "TaskItem.hxx"

BackLogProxyModel::BackLogProxyModel(QObject* p_parent):
  QSortFilterProxyModel(p_parent)
{
  setSortRole(TaskItem::ePriorityRole);
}
