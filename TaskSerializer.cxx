#include "TaskSerializer.hxx"

#include "TaskItem.hxx"
#include "KanbanView.hxx"
#include "BackLogView.hxx"

#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>



#include <QDebug>



TaskSerializer::TaskSerializer(KanbanView* p_kanbanView, BackLogView* p_backLogView, QObject* p_parent):
  QObject(p_parent),
  m_kanbanView(p_kanbanView),
  m_backLogView(p_backLogView)
{
}

void TaskSerializer::SetFileFromModel()
{
  QJsonObject jsonObject;
  QJsonArray jsonArray;
  QFile taskFile("../Pensieve/json/tasks.json");

  if (taskFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QStringList rowsStringList;
    for (int row = 0; row < m_kanbanView->rowCount(); ++row) {
      auto item = m_kanbanView->item(row, 0);
      if (item->data(KanbanTaskItem::eTaskRole).toBool()) {
        auto taskItem = static_cast<KanbanTaskItem*>(item);
        jsonObject["Name"] = taskItem->GetName();
        jsonObject["Description"] = taskItem->GetDescription();
        jsonObject["Tags"] = QJsonArray::fromStringList(taskItem->GetTags());
        jsonObject["Epic"] = taskItem->GetEpic();
        jsonObject["Status"] = static_cast<int>(taskItem->GetStatus());
        jsonObject["Priority"] = taskItem->GetPriority();

        jsonArray.append(jsonObject);
      }
    }

    QJsonDocument jsonDocument(jsonArray);

    QTextStream inTaskFile(&taskFile);
    inTaskFile << jsonDocument.toJson();
  }

  taskFile.close();
}

void TaskSerializer::SetModelFromFile() {
  m_kanbanView->clear();
  m_backLogView->clear();

  QFile taskFile("../Pensieve/json/tasks.json");

  if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    auto jsonDocument = QJsonDocument::fromJson(taskFile.readAll());

    auto array = jsonDocument.array();
    for (int row = 0; row < array.size(); ++row) {
      auto jsonObject = array.at(row);

      auto tagsArray = jsonObject["Tags"].toArray();
      QStringList tags;
      for (int rowTag = 0; rowTag < tagsArray.size(); ++rowTag) {
        tags << tagsArray.at(rowTag).toString();
      }

      m_kanbanView->AddItem(
        jsonObject["Name"].toString(),
        jsonObject["Description"].toString(),
        tags,
        jsonObject["Epic"].toString(),
        static_cast<TaskStatus>(jsonObject["Status"].toInt()),
        jsonObject["Priority"].toInt());

      m_backLogView->AddItem(
        jsonObject["Name"].toString(),
        tags,
        jsonObject["Epic"].toString(),
        static_cast<TaskStatus>(jsonObject["Status"].toInt()),
        jsonObject["Priority"].toInt());
    }
  }

  taskFile.close();
}
