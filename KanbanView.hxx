#ifndef KANBANVIEW_HXX
#define KANBANVIEW_HXX

#include "TaskItem.hxx"

#include <QTableWidget>

class KanbanEpicItem;

class KanbanView: public QTableWidget
{
  Q_OBJECT

public:
  KanbanView(QWidget* p_parent = nullptr);

  KanbanTaskItem* AddItem(QString const& p_name, QString const& p_description, QStringList const& p_tags,
    QString const& p_epic, TaskStatus p_status = TaskStatus::eToDo, int p_priority = 0);

private:
  QMap<QString, KanbanEpicItem*> m_kanbanEpicMap;

};

#endif
