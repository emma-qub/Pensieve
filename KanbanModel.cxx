#include "KanbanModel.hxx"

#include <TaskItem.hxx>

KanbanModel::KanbanModel(QObject* p_parent):
  QStandardItemModel(p_parent)
{
  setColumnCount(TaskItem::eNone);
}

KanbanModel::~KanbanModel() = default;

void KanbanModel::AppendTaskItem(TaskItem *p_taskItem)
{
  QList<QStandardItem*> row;
  auto status = p_taskItem->GetStatus();
  auto epic = p_taskItem->GetEpic();
  QStandardItem* epicItem = nullptr;
  if (m_itemByEpicMap.contains(epic)) {
    epicItem = m_itemByEpicMap[epic];
  } else {
    epicItem = new QStandardItem(epic);
    appendRow({epicItem, new QStandardItem, new QStandardItem, new QStandardItem});
  }

  for (int col = 0; col < columnCount(); ++ col) {
    col == status? row << p_taskItem: row << new QStandardItem;
  }
  epicItem->appendRow(row);
}

TaskItem* KanbanModel::GetTaskItemFromIndex(const QModelIndex& p_index) const
{
  Q_ASSERT_X(p_index.isValid(), "KanbanModel::GetTaskItemFromIndex", "Invalid index.");

  return dynamic_cast<TaskItem*>(itemFromIndex(p_index));
}
