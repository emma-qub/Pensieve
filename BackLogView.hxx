#ifndef BACKLOGVIEW_HXX
#define BACKLOGVIEW_HXX

#include <QTreeView>
#include <QStyledItemDelegate>

class BackLogDelegate: public QStyledItemDelegate
{
  Q_OBJECT

public:
  BackLogDelegate(QObject* p_parent = nullptr);

  void paint(QPainter* p_painter, QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;
  QSize sizeHint(QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;
};

class BackLogView: public QTreeView
{
  Q_OBJECT

public:
  BackLogView(QWidget* p_parent = nullptr);
};

#endif
