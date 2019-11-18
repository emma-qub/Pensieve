#include "MainWindow.hxx"

#include "NewTaskDialog.hxx"
#include "TaskItem.hxx"
#include "BackLogView.hxx"
#include "KanbanView.hxx"
#include "TaskSerializer.hxx"

#include <QAction>

MainWindow::MainWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_kanbanView(new KanbanView),
  m_backLogView(new BackLogView),
  m_centralWidget(new QTabWidget),
  m_addTaskAction(new QAction),
  m_serializer(new TaskSerializer(m_kanbanView, m_backLogView, this))
{
  m_addTaskAction->setShortcut(QKeySequence::New);
  m_addTaskAction->setParent(this);
  addAction(m_addTaskAction);
  connect(m_addTaskAction, &QAction::triggered, this, &MainWindow::AddNewTask);

  m_serializer->SetModelFromFile();

  m_centralWidget->addTab(m_backLogView, tr("Back log"));
  m_centralWidget->addTab(m_kanbanView, tr("Kanban"));

  setCentralWidget(m_centralWidget);

  showMaximized();
}

void MainWindow::AddNewTask() {
  auto dialog = new NewTaskDialog(this);
  if (dialog->exec() == QDialog::Accepted) {
    // Kanban
    auto tableWidgetItem = m_kanbanView->AddItem(dialog->GetName(), dialog->GetDescription(), dialog->GetTags(), dialog->GetEpic());
    // BackLog
    auto listWidgetItem = m_backLogView->AddItem(dialog->GetName(), dialog->GetTags(), dialog->GetEpic());

    m_listTableWidgetItemsList[listWidgetItem] = tableWidgetItem;
    m_tableListWidgetItemsList[tableWidgetItem] = listWidgetItem;
  }
}

MainWindow::~MainWindow() = default;
