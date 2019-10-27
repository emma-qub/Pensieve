#include "MainWindow.hxx"

#include "NewTaskDialog.hxx"
#include "TaskItem.hxx"
#include "KanbanModel.hxx"
#include "BackLogModel.hxx"
#include "BackLogProxyModel.hxx"
#include "BackLogView.hxx"
#include "KanbanView.hxx"
#include "TaskSerializer.hxx"

#include <QAction>
#include <QHeaderView>



#include <QDebug>



MainWindow::MainWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_kanbanView(new KanbanView),
  m_kanbanModel(new KanbanModel(this)),
  m_backLogView(new BackLogView),
  m_backLogModel(new BackLogModel(this)),
  m_backLogProxyModel(new BackLogProxyModel),
  m_centralWidget(new QTabWidget),
  m_addTaskAction(new QAction)
{
  m_addTaskAction->setShortcut(QKeySequence::New);
  m_addTaskAction->setParent(this);
  addAction(m_addTaskAction);
  connect(m_addTaskAction, &QAction::triggered, this, &MainWindow::AddNewTask);

  m_backLogModel->setObjectName("MODEL");
  m_backLogProxyModel->setObjectName("PROXY");
  m_backLogProxyModel->setSourceModel(m_backLogModel);
  m_backLogView->setModel(m_backLogProxyModel);

  m_kanbanView->setModel(m_kanbanModel);
  for (int col = 0; col < m_kanbanModel->columnCount(); ++col)
    m_kanbanView->header()->setSectionResizeMode(col, QHeaderView::Stretch);

  m_centralWidget->addTab(m_backLogView, tr("Back log"));
  m_centralWidget->addTab(m_kanbanView, tr("Kanban"));

  TaskSerializer serializer(m_backLogModel, m_kanbanModel, this);
  serializer.SetModelFromFile();
  m_backLogProxyModel->sort(0);

  setCentralWidget(m_centralWidget);

  showMaximized();
}

void MainWindow::AddNewTask()
{
  auto dialog = new NewTaskDialog(this);
  if (dialog->exec() == QDialog::Accepted) {
    auto taskItem = new TaskItem(dialog->GetName());
    taskItem->SetDescription(dialog->GetDescription());
    taskItem->SetTags(dialog->GetTags());
    taskItem->SetEpic(dialog->GetEpic());
    taskItem->MoveToToDo();
    m_kanbanModel->appendRow(taskItem);
  }

  TaskSerializer serializer(m_backLogModel, m_kanbanModel, this);
  serializer.SetFileFromModel();
}

MainWindow::~MainWindow() = default;
