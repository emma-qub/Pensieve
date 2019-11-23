#include "BackLogView.hxx"

#include "BackLogItemDelegate.hxx"

#include <QDropEvent>

BackLogView::BackLogView(QWidget* p_parent):
  QListWidget(p_parent) {

  setEditTriggers(QTableWidget::NoEditTriggers);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setDragEnabled(true);
  viewport()->setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);

  setItemDelegate(new BackLogItemDelegate);
}

BackLogItem* BackLogView::AddItem(QString const& p_name, QString const& p_description,
  QStringList const& p_tags, QString const& p_epic, TaskStatus p_status, int p_priority) {
  auto item = new BackLogItem(p_name);
  item->SetDescription(p_description);
  item->SetTags(p_tags);
  item->SetEpic(p_epic);
  item->SetStatus(p_status);
  p_priority == 0 ?
    item->SetPriority(count()):
    item->SetPriority(p_priority);

  insertItem(item->GetPriority(), item);

  sortItems();

  return item;
}

void BackLogView::dropEvent(QDropEvent* p_event) {
  QListWidget::dropEvent(p_event);

  Q_EMIT rowMovedBegin();

  for (int r = 0; r < count(); ++r) {
    auto currItem = static_cast<BackLogItem*>(item(r));
    currItem->SetPriority(r+1);
  }

  Q_EMIT rowMovedEnd();
}
