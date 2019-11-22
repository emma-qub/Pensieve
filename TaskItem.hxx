#ifndef TASKITEM_HXX
#define TASKITEM_HXX

#include <QTableWidgetItem>
#include <QListWidget>
#include <QStyledItemDelegate>

enum class TaskStatus {
  eToDo,
  eInProgress,
  ePause,
  eDone,
  eNone
};

class BackLogItem: public QListWidgetItem {
public:
  enum BackLogTaskDataRoles {
    eNameRole = Qt::DisplayRole,
    eDescriptionRole = Qt::UserRole,
    eTagsRole,
    eStatusRole,
    eEpicRole,
    ePriorityRole
  };

  BackLogItem(QString const& p_taskName);

  QString GetName() const;
  void SetName(QString const& p_name);

  QString GetDescription() const;
  void SetDescription(QString const& p_description);

  QStringList GetTags() const;
  void SetTags(QStringList const& p_category);

  TaskStatus GetStatus() const;
  void SetStatus(TaskStatus p_status);

  QString GetEpic() const;
  void SetEpic(QString const& p_epic);

  int GetPriority() const;
  void SetPriority(int p_priority);

};

class KanbanEpicItem: public QTableWidgetItem {
public:
  enum KanbanTaskDataRoles {
    eNameRole = Qt::DisplayRole,
    eDescriptionRole = Qt::UserRole,
    eTagsRole,
    eStatusRole,
    eEpicRole,
    ePriorityRole
  };

  enum EpicDataRoles {
    eToDo
  };

  KanbanEpicItem(QString const& p_taskName);
};

class KanbanTaskItem: public QTableWidgetItem
{
public:
  enum KanbanTaskDataRoles {
    eNameRole = Qt::DisplayRole,
    eDescriptionRole = Qt::UserRole,
    eTagsRole,
    eStatusRole,
    eEpicRole,
    ePriorityRole,

    eTaskRole = 0b10000000000
  };

  KanbanTaskItem(QString const& p_taskName);
  ~KanbanTaskItem() override;

  QString GetName() const;
  void SetName(QString const& p_name);

  QString GetDescription() const;
  void SetDescription(QString const& p_description);

  QStringList GetTags() const;
  void SetTags(QStringList const& p_category);

  TaskStatus GetStatus() const;
  void SetStatus(TaskStatus p_status);

  QString GetEpic() const;
  void SetEpic(QString const& p_epic);

  int GetPriority() const;
  void SetPriority(int p_priority);

  void MoveToToDo();
  void MoveToInProgress();
  void MoveToPause();
  void MoveToDone();
};

Q_DECLARE_METATYPE(TaskStatus)

#endif
