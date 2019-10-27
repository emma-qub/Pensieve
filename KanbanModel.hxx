#ifndef KANBANMODEL_HXX
#define KANBANMODEL_HXX

#include <QStandardItemModel>

class TaskItem;

class KanbanModel: public QStandardItemModel
{
  Q_OBJECT

public:
  KanbanModel(QObject* p_parent = nullptr);
  ~KanbanModel() override;

  void AppendTaskItem(TaskItem* p_taskItem);

  TaskItem* GetTaskItemFromIndex(QModelIndex const& p_index) const;

private:
  QMap<QString, QStandardItem*> m_itemByEpicMap;
};

#endif
