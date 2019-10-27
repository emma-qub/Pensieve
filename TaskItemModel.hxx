#ifndef TASKITEMMODEL_HXX
#define TASKITEMMODEL_HXX

#include <QStandardItemModel>

class TaskItem;

class TaskItemModel: public QStandardItemModel
{
  Q_OBJECT

public:
  TaskItemModel(QObject* p_parent = nullptr);
  ~TaskItemModel() override;

  void AppendTaskItem(TaskItem* p_taskItem);

  TaskItem* GetTaskItemFromIndex(QModelIndex const& p_index) const;

private:
  QMap<QString, QStandardItem*> m_itemByEpicMap;
};

#endif
