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

  // Set labels here, otherwise they are overriden somewhere...
  m_kanbanView->setHorizontalHeaderLabels({"To do", "In progress", "Done", "Pause"});

  m_centralWidget->addTab(m_backLogView, tr("Back log"));
  m_centralWidget->addTab(m_kanbanView, tr("Kanban"));

  setCentralWidget(m_centralWidget);

  //connect(m_kanbanView, &KanbanView::itemChanged, this, &MainWindow::SynchronizeItemsFromKanban);
  connect(m_kanbanView, &KanbanView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
  //connect(m_backLogView, &BackLogView::itemChanged, this, &MainWindow::SynchronizeItemsFromBackLog);
  //connect(m_backLogView, &BackLogView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);

  showMaximized();
}

MainWindow::~MainWindow() = default;

void MainWindow::AddNewTask() {
  auto dialog = new NewTaskDialog(this);
  if (dialog->exec() == QDialog::Accepted) {
    // Kanban
    auto tableWidgetItem = m_kanbanView->AddItem(dialog->GetName(), dialog->GetDescription(), dialog->GetTags(), dialog->GetEpic());
    // BackLog
    auto listWidgetItem = m_backLogView->AddItem(dialog->GetName(), dialog->GetDescription(), dialog->GetTags(), dialog->GetEpic());

    m_listTableWidgetItemsList[listWidgetItem] = tableWidgetItem;
    m_tableListWidgetItemsList[tableWidgetItem] = listWidgetItem;
  }
}

void MainWindow::SynchronizeItemsFromKanban() {
  disconnect(m_kanbanView, &KanbanView::itemChanged, this, &MainWindow::SynchronizeItemsFromKanban);
  disconnect(m_kanbanView, &KanbanView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
  disconnect(m_backLogView, &BackLogView::itemChanged, this, &MainWindow::SynchronizeItemsFromBackLog);
  disconnect(m_backLogView, &BackLogView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);

  m_backLogView->clear();
  for (int row = 0; row < m_kanbanView->rowCount(); ++row) {
    for (int col = 0; col < m_kanbanView->columnCount(); ++col) {
      auto const* item = m_kanbanView->item(row, col);
      if (item && item->data(KanbanTaskItem::eTaskRole).toBool()) {
        auto const* taskItem = static_cast<KanbanTaskItem const*>(item);
        m_backLogView->AddItem(taskItem->GetName(), taskItem->GetDescription(), taskItem->GetTags(), taskItem->GetEpic(), taskItem->GetStatus(), taskItem->GetPriority());
      }
    }
  }

  connect(m_kanbanView, &KanbanView::itemChanged, this, &MainWindow::SynchronizeItemsFromKanban);
  connect(m_kanbanView, &KanbanView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
  connect(m_backLogView, &BackLogView::itemChanged, this, &MainWindow::SynchronizeItemsFromBackLog);
  connect(m_backLogView, &BackLogView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
}

void MainWindow::SynchronizeItemsFromBackLog() {
  disconnect(m_kanbanView, &KanbanView::itemChanged, this, &MainWindow::SynchronizeItemsFromKanban);
  disconnect(m_kanbanView, &KanbanView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
  disconnect(m_backLogView, &BackLogView::itemChanged, this, &MainWindow::SynchronizeItemsFromBackLog);
  disconnect(m_backLogView, &BackLogView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);

  m_kanbanView->clear();
  for (int row = 0; row < m_kanbanView->rowCount(); ++row) {
    auto const* item = m_backLogView->item(row);
    if (item) {
      auto const* taskItem = static_cast<BackLogItem const*>(item);
      m_kanbanView->AddItem(taskItem->GetName(), taskItem->GetDescription(), taskItem->GetTags(), taskItem->GetEpic(), taskItem->GetStatus(), taskItem->GetPriority());
    }
  }

  connect(m_kanbanView, &KanbanView::itemChanged, this, &MainWindow::SynchronizeItemsFromKanban);
  connect(m_kanbanView, &KanbanView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
  connect(m_backLogView, &BackLogView::itemChanged, this, &MainWindow::SynchronizeItemsFromBackLog);
  connect(m_backLogView, &BackLogView::itemChanged, m_serializer, &TaskSerializer::SetFileFromModel);
}
