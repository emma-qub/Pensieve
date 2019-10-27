#ifndef BACKLOGMODEL_HXX
#define BACKLOGMODEL_HXX

#include <QStandardItemModel>

class TaskItem;

class BackLogModel: public QStandardItemModel
{
  Q_OBJECT

public:
  BackLogModel(QObject* p_parent = nullptr);

  void AppendTaskItem(TaskItem* p_taskItem);

  TaskItem* GetTaskItemFromIndex(QModelIndex const& p_index) const;

};

#endif
