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

protected:
  void dropEvent(QDropEvent* p_event) override;
  void dragEnterEvent(QDragEnterEvent* p_event) override;
  void dragMoveEvent(QDragMoveEvent* p_event) override;

  QColor GetEpicColor(const QString& p_epic);

private:
  QMap<QString, KanbanEpicItem*> m_kanbanEpicMap;
  int m_formerRow;

};

#endif
