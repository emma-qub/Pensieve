#ifndef BACKLOGPROXYMODEL_HXX
#define BACKLOGPROXYMODEL_HXX

#include <QSortFilterProxyModel>

class BackLogProxyModel: public QSortFilterProxyModel
{
  Q_OBJECT

public:
  BackLogProxyModel(QObject* p_parent = nullptr);
};

#endif
