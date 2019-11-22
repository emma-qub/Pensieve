#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QMap>

class KanbanView;
class BackLogView;
class KanbanEpicItem;
class QListWidgetItem;
class QTableWidgetItem;
class TaskSerializer;

class MainWindow: public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = nullptr);
  ~MainWindow();

protected:
  void AddNewTask();
  void SynchronizeItemsFromKanban();
  void SynchronizeItemsFromBackLog();

private:
  KanbanView* m_kanbanView;
  BackLogView* m_backLogView;
  QTabWidget* m_centralWidget;

  QAction* m_addTaskAction;

  QMap<QListWidgetItem*, QTableWidgetItem*> m_listTableWidgetItemsList;
  QMap<QTableWidgetItem*, QListWidgetItem*> m_tableListWidgetItemsList;

  TaskSerializer* m_serializer;
};
#endif
