#include "BackLogView.hxx"

BackLogView::BackLogView(QWidget* p_parent):
  QListWidget(p_parent) {

  setEditTriggers(QTableWidget::NoEditTriggers);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setDragEnabled(true);
  viewport()->setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);

  //setItemDelegate(new BackLogDelegate);
}

BackLogItem* BackLogView::AddItem(QString const& p_name, QString const& p_description, QStringList const& p_tags, QString const& p_epic, TaskStatus p_status, int p_priority) {
  auto item = new BackLogItem(p_name);
  item->SetDescription(p_description);
  item->SetTags(p_tags);
  item->SetEpic(p_epic);
  item->SetStatus(p_status);
  item->SetPriority(p_priority);

  insertItem(0, item);

  sortItems();

  return item;
}
