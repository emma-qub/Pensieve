#ifndef TASKSERIALIZER_HXX
#define TASKSERIALIZER_HXX

#include <QObject>

class KanbanModel;
class BackLogModel;

class TaskSerializer: public QObject
{
public:
  TaskSerializer(BackLogModel* p_backLogModel = nullptr, KanbanModel* p_model = nullptr, QObject* p_parent = nullptr);

  void SetFileFromModel();
  void SetModelFromFile();

private:
  KanbanModel* m_kanbanModel;
  BackLogModel* m_backLogModel;
};

#endif
