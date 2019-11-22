#ifndef KANBANITEMDELEGATE_HXX
#define KANBANITEMDELEGATE_HXX

#include <QItemDelegate>


class KanbanItemDelegate: public QItemDelegate
{
  Q_OBJECT

public:
  KanbanItemDelegate(QObject* p_parent = nullptr);

  void paint(QPainter* p_painter, QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;

  QSize sizeHint(QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;
};

#endif
