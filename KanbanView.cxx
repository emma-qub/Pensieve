#include "KanbanView.hxx"

#include "TaskItem.hxx"
#include "KanbanItemDelegate.hxx"

#include <QHeaderView>
#include <QDropEvent>

KanbanView::KanbanView(QWidget* p_parent):
  QTableWidget(p_parent),
  m_formerRow(-1) {

  setColumnCount(static_cast<int>(TaskStatus::eNone));

  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

  verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  verticalHeader()->hide();

  setEditTriggers(QTableWidget::NoEditTriggers);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setDragEnabled(true);
  viewport()->setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);

  setItemDelegate(new KanbanItemDelegate);
}

KanbanTaskItem* KanbanView::AddItem(QString const& p_name, QString const& p_description, QStringList const& p_tags,
  QString const& p_epic, TaskStatus p_status, int p_priority) {
  auto item = new KanbanTaskItem(p_name);
  item->SetDescription(p_description);
  item->SetTags(p_tags);
  item->SetEpic(p_epic);
  item->SetStatus(p_status);
  item->SetPriority(p_priority);

  auto epicItem = m_kanbanEpicMap[p_epic];
  if (epicItem == nullptr) {
    epicItem = new KanbanEpicItem(p_epic);
    auto lastRowIndex = rowCount();
    insertRow(lastRowIndex);
    setItem(lastRowIndex, 0, epicItem);
    setSpan(lastRowIndex, 0, 1, static_cast<int>(TaskStatus::eNone));
    m_kanbanEpicMap[p_epic] = epicItem;
  }

  insertRow(epicItem->row()+1);
  setItem(epicItem->row()+1, item->data(KanbanTaskItem::eStatusRole).toInt(), item);

  return item;
}

void KanbanView::Clear() {
  QTableWidget::clearContents();
  setRowCount(0);
  m_kanbanEpicMap.clear();
}

QStringList KanbanView::GetEpicsList() const {
  return m_kanbanEpicMap.keys();
}

void KanbanView::dropEvent(QDropEvent* p_event) {
  if (rowAt(p_event->pos().y()) != m_formerRow) {
    p_event->setAccepted(false);
    m_formerRow = -1;
    return;
  }

  QTableWidget::dropEvent(p_event);
  itemAt(p_event->pos())->setData(KanbanTaskItem::eStatusRole, columnAt(p_event->pos().x()));
  m_formerRow = -1;
}

void KanbanView::dragEnterEvent(QDragEnterEvent* p_event) {
  m_formerRow = rowAt(p_event->pos().y());
  QTableWidget::dragEnterEvent(p_event);
}

void KanbanView::dragMoveEvent(QDragMoveEvent* p_event) {
  QTableWidget::dragMoveEvent(p_event);
  if (rowAt(p_event->pos().y()) != m_formerRow) {
    p_event->setAccepted(false);
  }
}

void KanbanView::mouseDoubleClickEvent(QMouseEvent* p_event) {
  if (p_event->button() == Qt::LeftButton) {
    auto itemDClicked = itemAt(p_event->pos());
    if (itemDClicked && !itemDClicked->data(KanbanTaskItem::eEpicRole).toBool()) {
      auto epicItem = static_cast<KanbanEpicItem*>(itemDClicked);
      epicItem->ToggleExpand();
      auto expanded = epicItem->IsExpanded();
      for (int r = 0; r < rowCount(); ++r) {
        for (int c = 0; c < columnCount(); ++c) {
          auto currItem = item(r, c);
          if (currItem && currItem->data(KanbanTaskItem::eEpicRole).toBool()) {
            auto taskItem = static_cast<KanbanTaskItem*>(currItem);
            if (taskItem->GetEpic() == epicItem->text()) {
              setRowHidden(r, !expanded);
            }
          }
        }
      }
    }
  }

  QTableWidget::mouseDoubleClickEvent(p_event);
}
