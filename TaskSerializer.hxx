#ifndef TASKSERIALIZER_HXX
#define TASKSERIALIZER_HXX

#include <QFile>
#include <QObject>

class KanbanView;
class BackLogView;

class TaskSerializer: public QObject
{
  Q_OBJECT

public:
  TaskSerializer(KanbanView* p_kanbanView, BackLogView* p_backLogView, QObject* p_parent = nullptr);

  void SetFileFromModel();
  void SetModelFromFile();

protected:
  bool Backup() const;

private:
  KanbanView* m_kanbanView;
  BackLogView* m_backLogView;
};

#endif
