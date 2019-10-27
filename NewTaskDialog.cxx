#include "NewTaskDialog.hxx"

#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

NewTaskDialog::NewTaskDialog(QWidget* p_parent):
  QDialog(p_parent),
  m_nameLineEdit(new QLineEdit),
  m_descriptionTextEdit(new QTextEdit),
  m_categoryComboBox(new QComboBox),
  m_epicComboBox(new QComboBox)
{
  auto nameLayout = new QFormLayout;
  nameLayout->addRow(tr("Name"), m_nameLineEdit);

  auto descriptionGroupBox = new QGroupBox(tr("Description"));
  auto descriptionLayout = new QVBoxLayout;
  descriptionLayout->addWidget(m_descriptionTextEdit);
  descriptionGroupBox->setLayout(descriptionLayout);

  auto categoryLayout = new QFormLayout;
  categoryLayout->addRow(tr("Category"), m_categoryComboBox);
  m_categoryComboBox->setEditable(true);

  auto epicLayout = new QFormLayout;
  epicLayout->addRow(tr("Epic"), m_epicComboBox);
  m_epicComboBox->setEditable(true);

  m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
  connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionGroupBox);
  mainLayout->addLayout(categoryLayout);
  mainLayout->addLayout(epicLayout);
  mainLayout->addWidget(m_buttonBox);
  mainLayout->setAlignment(m_buttonBox, Qt::AlignRight);

  resize(1000, 800);
  setLayout(mainLayout);
}

NewTaskDialog::~NewTaskDialog() = default;

QString NewTaskDialog::GetName() const
{
  return m_nameLineEdit->text();
}

QString NewTaskDialog::GetDescription() const
{
  return m_descriptionTextEdit->toPlainText();
}

QStringList NewTaskDialog::GetTags() const
{
  return m_categoryComboBox->currentText().split(", ");
}

QString NewTaskDialog::GetEpic() const
{
  return m_epicComboBox->currentText();
}
