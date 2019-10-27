#ifndef TASKITEM_HXX
#define TASKITEM_HXX

#include <QStandardItem>

class TaskItem: public QStandardItem
{
public:
  enum TaskDataRoles {
    eNameRole = Qt::DisplayRole,
    eDescriptionRole = Qt::UserRole,
    eTagsRole,
    eStatusRole,
    eEpicRole,
    ePriorityRole
  };

  enum Status {
    eToDo,
    eInProgress,
    ePause,
    eDone,
    eNone
  };

  TaskItem(QString const& p_taskName);
  ~TaskItem() override;

  QString GetName() const;
  void SetName(QString const& p_name);

  QString GetDescription() const;
  void SetDescription(QString const& p_description);

  QStringList GetTags() const;
  void SetTags(QStringList const& p_category);

  Status GetStatus() const;
  void SetStatus(Status p_status);

  QString GetEpic() const;
  void SetEpic(QString const& p_epic);

  int GetPriority() const;
  void SetPriority(int p_priority);

  void MoveToToDo();
  void MoveToInProgress();
  void MoveToPause();
  void MoveToDone();

  TaskItem* clone() const override;
};

Q_DECLARE_METATYPE(TaskItem::Status)

#endif
