#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>

class KanbanView;
class KanbanModel;
class BackLogView;
class BackLogModel;
class BackLogProxyModel;

class MainWindow: public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* p_parent = nullptr);
  ~MainWindow();

protected:
  void AddNewTask();

private:
  KanbanView* m_kanbanView;
  KanbanModel* m_kanbanModel;
  BackLogView* m_backLogView;
  BackLogModel* m_backLogModel;
  BackLogProxyModel* m_backLogProxyModel;
  QTabWidget* m_centralWidget;

  QAction* m_addTaskAction;
};
#endif
