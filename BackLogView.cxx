#include "BackLogView.hxx"

#include "BackLogModel.hxx"
#include "BackLogProxyModel.hxx"
#include "TaskItem.hxx"

#include <QPaintEvent>
#include <QPainter>



#include <QDebug>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BackLogDelegate
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BackLogDelegate::BackLogDelegate(QObject* p_parent):
  QStyledItemDelegate(p_parent)
{
}

void BackLogDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const
{
  //return QStyledItemDelegate::paint(p_painter, p_option, p_index);

  auto proxy = qobject_cast<BackLogProxyModel const*>(p_index.model());
  auto model = qobject_cast<BackLogModel*>(proxy->sourceModel());
  auto taskItem = model->GetTaskItemFromIndex(proxy->mapToSource(p_index));

  if (taskItem) {
    auto font = p_painter->font();
    font.setBold(true);
    p_painter->setFont(font);
    auto fontMetrics = p_painter->fontMetrics();
    auto itemRect = p_option.rect;
    int topShift = p_index.row() * itemRect.height();
    int shift = 15;
    int globalShift = shift;

    // Save painter
    p_painter->save();

    // Task epic
    auto taskEpic = taskItem->GetEpic();
    auto taskEpicBoundingRect = fontMetrics.boundingRect(taskEpic);
    QRect taskEpicRect = taskEpicBoundingRect;
    taskEpicRect.moveLeft(globalShift);
    taskEpicRect.moveTop(topShift + (itemRect.height()-taskEpicBoundingRect.height())/2);
    QRect taskEpicUnderlineRect = taskEpicRect.marginsAdded(QMargins(5, 5, 5, 5));
    taskEpicRect.moveLeft(taskEpicRect.left()+3);
    globalShift += taskEpicRect.width() + shift;

    // Task name
    auto taskName = taskItem->GetName();
    auto taskNameBoundingRect = fontMetrics.boundingRect(taskName);
    QRect taskNameRect = taskNameBoundingRect;
    taskNameRect.moveLeft(globalShift);
    taskNameRect.moveTop(topShift + (itemRect.height()-taskNameBoundingRect.height())/2);

    // Draw
    p_painter->drawRect(p_option.rect);
    p_painter->setBrush(Qt::black);
    p_painter->setPen(Qt::NoPen);
    p_painter->drawRoundedRect(taskEpicUnderlineRect, 5, 5);
    p_painter->setBrush(Qt::NoBrush);
    p_painter->setPen(Qt::white);
    p_painter->drawText(taskEpicRect, taskEpic);
    p_painter->setPen(Qt::black);
    p_painter->drawText(taskNameRect, taskName);

    // Restore painter
    p_painter->restore();
  }
}

QSize BackLogDelegate::sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const
{
  return QSize(QStyledItemDelegate::sizeHint(p_option, p_index).width(), 50);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BackLogView
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BackLogView::BackLogView(QWidget* p_parent):
  QTreeView(p_parent)
{
  setItemDelegate(new BackLogDelegate);
}
