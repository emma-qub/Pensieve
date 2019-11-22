#include "TaskItem.hxx"

#include <iomanip>


BackLogItem::BackLogItem(const QString& p_taskName):
  QListWidgetItem(p_taskName) {
}

QString BackLogItem::GetName() const {
  return text();
}

void BackLogItem::SetName(QString const& p_name) {
  setText(p_name);
}

QString BackLogItem::GetDescription() const {
  return data(eDescriptionRole).toString();
}

void BackLogItem::SetDescription(QString const& p_description) {
  setData(eDescriptionRole, p_description);
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
  setData(eStatusRole, static_cast<int>(p_status));
}

QString BackLogItem::GetEpic() const {
  return data(eEpicRole).toString();
}

void BackLogItem::SetEpic(const QString& p_epic) {
  setData(eEpicRole, p_epic);
  setData(Qt::DecorationRole, GetEpicColor(p_epic));
}

int BackLogItem::GetPriority() const {
  return data(ePriorityRole).toInt();
}

void BackLogItem::SetPriority(int p_priority) {
  setData(ePriorityRole, p_priority);
}

bool BackLogItem::operator<(const QListWidgetItem& p_other) const {
  auto const& backLogItem = dynamic_cast<BackLogItem const*>(&p_other);
  Q_ASSERT_X(backLogItem, "BackLogItem::operator<", "Comparizon between a BackLogItem and another QListWidgetItem.");

  return GetPriority() < backLogItem->GetPriority();
}



KanbanEpicItem::KanbanEpicItem(QString const& p_epicName):
  QTableWidgetItem(p_epicName) {
  setData(Qt::DecorationRole, GetEpicColor(p_epicName));
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
  setData(eStatusRole, static_cast<int>(p_status));
}

QString KanbanTaskItem::GetEpic() const {
  return data(eEpicRole).toString();
}

void KanbanTaskItem::SetEpic(const QString& p_epic) {
  setData(eEpicRole, p_epic);
  setData(Qt::DecorationRole, GetEpicColor(p_epic));
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



QColor GetEpicColor(QString const& p_epic) {
  auto colorInt = qHash(p_epic);
  std::stringstream stream;
  stream << std::hex << colorInt;
  auto colorString = QString::fromStdString(stream.str());

  colorString = colorString.rightJustified(6, '0');
  colorString.truncate(6);
  QColor color(QString("#%1").arg(colorString));

  int c[3] = {color.red(), color.green(), color.blue()};
  int s = 64;

  int nearestC[3];
  for (int i = 0; i < 3; ++i) {
    auto cMin = (c[i] / s) * s;
    auto cMax = cMin + s - 1;
    std::abs(c[i] - cMin) < std::abs(c[i] - cMax)?
      nearestC[i] = cMin:
      nearestC[i] = cMax;
  }

  return QColor(nearestC[0], nearestC[1], nearestC[2]);
}
