#include "TaskItem.hxx"

TaskItem::TaskItem(QString const& p_taskName):
  QStandardItem(p_taskName)
{
}

TaskItem::~TaskItem() = default;

QString TaskItem::GetName() const
{
  return text();
}

void TaskItem::SetName(QString const& p_name)
{
  setText(p_name);
}

QString TaskItem::GetDescription() const
{
  return data(eDescriptionRole).toString();
}

void TaskItem::SetDescription(QString const& p_description)
{
  setData(p_description, eDescriptionRole);
}

QStringList TaskItem::GetTags() const
{
  return data(eTagsRole).toStringList();
}

void TaskItem::SetTags(const QStringList& p_category)
{
  setData(p_category, eTagsRole);
}

TaskItem::Status TaskItem::GetStatus() const
{
  return data(eStatusRole).value<Status>();
}

void TaskItem::SetStatus(TaskItem::Status p_status)
{
  setData(QVariant::fromValue<Status>(p_status), eStatusRole);
}

QString TaskItem::GetEpic() const
{
  return data(eEpicRole).toString();
}

void TaskItem::SetEpic(const QString& p_epic)
{
  setData(p_epic, eEpicRole);
}

int TaskItem::GetPriority() const
{
  return data(ePriorityRole).toInt();
}

void TaskItem::SetPriority(int p_priority)
{
  setData(p_priority, ePriorityRole);
}

void TaskItem::MoveToToDo()
{
  SetStatus(eToDo);
}

void TaskItem::MoveToInProgress()
{
  SetStatus(eInProgress);
}

void TaskItem::MoveToPause()
{
  SetStatus(ePause);
}

void TaskItem::MoveToDone()
{
  SetStatus(eDone);
}

TaskItem* TaskItem::clone() const
{
  return static_cast<TaskItem*>(QStandardItem::clone());
}
