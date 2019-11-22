#ifndef BACKLOGVIEW_HXX
#define BACKLOGVIEW_HXX

#include "TaskItem.hxx"

#include <QListWidget>

class BackLogView: public QListWidget
{
  Q_OBJECT

public:
  BackLogView(QWidget* p_parent = nullptr);

  BackLogItem* AddItem(QString const& p_name, QString const& p_description, QStringList const& p_tags, QString const& p_epic, TaskStatus p_status = TaskStatus::eToDo, int p_priority = 0);

};

#endif
