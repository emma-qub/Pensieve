#include "KanbanView.hxx"

#include "TaskItem.hxx"
#include "KanbanItemDelegate.hxx"

#include <iomanip>

#include <QHeaderView>
#include <QDropEvent>


#include <QDebug>

KanbanView::KanbanView(QWidget* p_parent):
  QTableWidget(p_parent),
  m_formerRow(-1)
{
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

  auto epicColor = GetEpicColor(p_epic);
  item->setData(Qt::DecorationRole, epicColor);

  auto epicItem = m_kanbanEpicMap[p_epic];
  if (epicItem == nullptr) {
    epicItem = new KanbanEpicItem(p_epic);
    auto lastRowIndex = rowCount();
    insertRow(lastRowIndex);
    setItem(lastRowIndex, 0, epicItem);
    setSpan(lastRowIndex, 0, 1, static_cast<int>(TaskStatus::eNone));
    m_kanbanEpicMap[p_epic] = epicItem;

    epicItem->setData(Qt::DecorationRole, epicColor);
  }

  insertRow(epicItem->row()+1);
  setItem(epicItem->row()+1, item->data(KanbanTaskItem::eStatusRole).toInt(), item);

  return item;
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

QColor KanbanView::GetEpicColor(QString const& p_epic) {
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
