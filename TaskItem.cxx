#include "TaskItem.hxx"


BackLogItem::BackLogItem(const QString& p_taskName):
  QListWidgetItem(p_taskName) {
}

QString BackLogItem::GetName() const {
  return text();
}

void BackLogItem::SetName(QString const& p_name) {
  setText(p_name);
}

QStringList BackLogItem::GetTags() const {
  return data(eTagsRole).toStringList();
}

void BackLogItem::SetTags(const QStringList& p_category) {
  setData(eTagsRole, p_category);
}

TaskStatus BackLogItem::GetStatus() const {
  return data(eStatusRole).value<TaskStatus>();
}

void BackLogItem::SetStatus(TaskStatus p_status) {
  setData(eStatusRole, QVariant::fromValue<TaskStatus>(p_status));
}

QString BackLogItem::GetEpic() const {
  return data(eEpicRole).toString();
}

void BackLogItem::SetEpic(const QString& p_epic) {
  setData(eEpicRole, p_epic);
}

int BackLogItem::GetPriority() const {
  return data(ePriorityRole).toInt();
}

void BackLogItem::SetPriority(int p_priority) {
  setData(ePriorityRole, p_priority);
}



KanbanEpicItem::KanbanEpicItem(QString const& p_taskName):
  QTableWidgetItem(p_taskName) {
}


KanbanTaskItem::KanbanTaskItem(QString const& p_taskName):
  QTableWidgetItem(p_taskName) {

  setData(eTaskRole, true);
}

KanbanTaskItem::~KanbanTaskItem() = default;

QString KanbanTaskItem::GetName() const {
  return text();
}

void KanbanTaskItem::SetName(QString const& p_name) {
  setText(p_name);
}

QString KanbanTaskItem::GetDescription() const {
  return data(eDescriptionRole).toString();
}

void KanbanTaskItem::SetDescription(QString const& p_description) {
  setData(eDescriptionRole, p_description);
}

QStringList KanbanTaskItem::GetTags() const {
  return data(eTagsRole).toStringList();
}

void KanbanTaskItem::SetTags(const QStringList& p_category) {
  setData(eTagsRole, p_category);
}

TaskStatus KanbanTaskItem::GetStatus() const {
  return data(eStatusRole).value<TaskStatus>();
}

void KanbanTaskItem::SetStatus(TaskStatus p_status) {
  setData(eStatusRole, QVariant::fromValue<TaskStatus>(p_status));
}

QString KanbanTaskItem::GetEpic() const {
  return data(eEpicRole).toString();
}

void KanbanTaskItem::SetEpic(const QString& p_epic) {
  setData(eEpicRole, p_epic);
}

int KanbanTaskItem::GetPriority() const {
  return data(ePriorityRole).toInt();
}

void KanbanTaskItem::SetPriority(int p_priority) {
  setData(ePriorityRole, p_priority);
}

void KanbanTaskItem::MoveToToDo() {
  SetStatus(TaskStatus::eToDo);
}

void KanbanTaskItem::MoveToInProgress() {
  SetStatus(TaskStatus::eInProgress);
}

void KanbanTaskItem::MoveToPause() {
  SetStatus(TaskStatus::ePause);
}

void KanbanTaskItem::MoveToDone() {
  SetStatus(TaskStatus::eDone);
}



KanbanTaskItemDelegate::KanbanTaskItemDelegate(QObject* p_parent):
  QStyledItemDelegate(p_parent) {
}

QSize KanbanTaskItemDelegate::sizeHint(QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const {
  auto normalSizeHint = QStyledItemDelegate::sizeHint(p_option, p_index);

  if (p_index.data(KanbanTaskItem::eTaskRole).toBool())
    return QSize(normalSizeHint.width(), 300);

  return normalSizeHint;
}
