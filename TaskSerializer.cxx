#include "TaskSerializer.hxx"

#include "KanbanModel.hxx"
#include "BackLogModel.hxx"
#include "TaskItem.hxx"

#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>



#include <QDebug>



TaskSerializer::TaskSerializer(BackLogModel* p_backLogModel, KanbanModel* p_kanbanModel, QObject* p_parent):
  QObject(p_parent),
  m_kanbanModel(p_kanbanModel),
  m_backLogModel(p_backLogModel)
{
}

void TaskSerializer::SetFileFromModel()
{
  QJsonObject jsonObject;
  QJsonArray jsonArray;
  QFile taskFile("../Pensieve/json/tasks.json");

  if (taskFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QStringList rowsStringList;
    for (int row = 0; row < m_kanbanModel->rowCount(); ++row) {
      auto taskItem = m_kanbanModel->GetTaskItemFromIndex(m_kanbanModel->index(row, 0));

      jsonObject["Name"] = taskItem->GetName();
      jsonObject["Description"] = taskItem->GetDescription();
      jsonObject["Tags"] = QJsonArray::fromStringList(taskItem->GetTags());
      jsonObject["Epic"] = taskItem->GetEpic();
      jsonObject["Status"] = taskItem->GetStatus();
      jsonObject["Priority"] = taskItem->GetPriority();

      jsonArray.append(jsonObject);
    }

    QJsonDocument jsonDocument(jsonArray);

    QTextStream inTaskFile(&taskFile);
    inTaskFile << jsonDocument.toJson();
  }

  taskFile.close();
}

void TaskSerializer::SetModelFromFile()
{
  QFile taskFile("../Pensieve/json/tasks.json");

  if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    auto jsonDocument = QJsonDocument::fromJson(taskFile.readAll());

    auto array = jsonDocument.array();
    for (int row = 0; row < array.size(); ++row) {
      auto jsonObject = array.at(row);

      auto taskItem = new TaskItem(jsonObject["Name"].toString());
      taskItem->SetDescription(jsonObject["Description"].toString());
      auto tagsArray = jsonObject["Tags"].toArray();
      QStringList tags;
      for (int rowTag = 0; rowTag < tagsArray.size(); ++rowTag) {
        tags << tagsArray.at(rowTag).toString();
      }
      taskItem->SetTags(tags);
      taskItem->SetEpic(jsonObject["Epic"].toString());
      taskItem->SetStatus(static_cast<TaskItem::Status>(jsonObject["Status"].toInt()));
      taskItem->SetPriority(jsonObject["Priority"].toInt());

      m_kanbanModel->AppendTaskItem(taskItem);
      m_backLogModel->AppendTaskItem(taskItem->clone());
    }
  }

  taskFile.close();
}
