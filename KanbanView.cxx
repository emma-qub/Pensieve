#include "KanbanView.hxx"

#include "TaskItem.hxx"

#include <iomanip>

#include <QHeaderView>
#include <QDebug>

KanbanView::KanbanView(QWidget* p_parent):
  QTableWidget(p_parent)
{
  setColumnCount(static_cast<int>(TaskStatus::eNone));
  setHorizontalHeaderLabels({"To do", "In progress", "Done", "Pause"});

  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  setEditTriggers(QTableWidget::NoEditTriggers);

  verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  verticalHeader()->hide();

  setItemDelegate(new KanbanTaskItemDelegate);

  setSelectionMode(QAbstractItemView::SingleSelection);
  setDragEnabled(true);
  viewport()->setAcceptDrops(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);
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

    auto colorInt = qHash(p_epic);
    std::stringstream stream;
    stream << std::hex << colorInt;
    auto colorString = QString::fromStdString(stream.str());

    colorString = colorString.rightJustified(6, '0');
    colorString.truncate(6);
    epicItem->setData(Qt::DecorationRole, QColor(QString("#%1").arg(colorString)));
  }

  insertRow(epicItem->row()+1);
  setItem(epicItem->row()+1, 0, item);

  return item;
}
