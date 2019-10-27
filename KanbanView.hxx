#ifndef KANBANVIEW_HXX
#define KANBANVIEW_HXX

#include <QTreeView>

class KanbanView: public QTreeView
{
  Q_OBJECT

public:
  KanbanView(QWidget* p_parent = nullptr);
};

#endif
