#ifndef NEWTASKDIALOG_HXX
#define NEWTASKDIALOG_HXX

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QComboBox;
class QDialogButtonBox;

class NewTaskDialog: public QDialog
{
  Q_OBJECT

public:
  NewTaskDialog(QStringList const& p_epics, QWidget* p_parent = nullptr);
  ~NewTaskDialog() override;

  QString GetName() const;
  QString GetDescription() const;
  QStringList GetTags() const;
  QString GetEpic() const;

private:
  QLineEdit* m_nameLineEdit;
  QTextEdit* m_descriptionTextEdit;
  QComboBox* m_categoryComboBox;
  QComboBox* m_epicComboBox;
  QDialogButtonBox* m_buttonBox;
};

#endif
