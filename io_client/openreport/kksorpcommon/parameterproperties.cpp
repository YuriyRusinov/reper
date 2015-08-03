/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#include "parameterproperties.h"
#include "ui_parameterproperties.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QList>

static QString formatVariant(const QVariant & var)
{
  QString value = var.toString();
  QString type  = var.typeName();

  if(!type.isEmpty())
    value += " (" + type + ")";

  return value;
}

ParameterProperties::ParameterProperties(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
  ui = new Ui::ParameterProperties;

  ui->setupUi(this);

  connect(ui->_listDelete, SIGNAL(clicked()), this, SLOT(deleteItem()));
  connect(ui->_listDown,   SIGNAL(clicked()), this, SLOT(moveItemDown()));
  connect(ui->_listEdit,   SIGNAL(clicked()), this, SLOT(editItem()));
  connect(ui->_listNew,    SIGNAL(clicked()), this, SLOT(newItem()));
  connect(ui->_listUp,     SIGNAL(clicked()), this, SLOT(moveItemUp()));

  ui->_int->setValidator(new QIntValidator(this));
  ui->_double->setValidator(new QDoubleValidator(this));
}

ParameterProperties::~ParameterProperties()
{
  // no need to delete child widgets, Qt does it all for us
}

void ParameterProperties::languageChange()
{
  ui->retranslateUi(this);
}

void ParameterProperties::newItem()
{
  QListWidgetItem *lwi = 0;
  QVariant var;
  ParameterProperties pedit(this);
  pedit.ui->_nameLit->hide();
  pedit.ui->_name->hide();
  if(pedit.exec() == QDialog::Accepted)
  {
    var = pedit.value();

    lwi = new QListWidgetItem(formatVariant(var), ui->_list);
    lwi->setData(Qt::UserRole, var);
  }
}

void ParameterProperties::editItem()
{
  QVariant var;
  QListWidgetItem * item = ui->_list->currentItem();
  if (item)
  {
    ParameterProperties pedit(this);
    pedit.ui->_nameLit->hide();
    pedit.ui->_name->hide();

    QVariant var = item->data(Qt::UserRole);
    QString typeName;

    switch(var.type())
    {
      case QVariant::Bool:
        typeName = tr("Bool");
        pedit.ui->_bool->setCurrentIndex(var.toBool() ? 1 : 0);
        break;
      case QVariant::Int:
        typeName = tr("Int");
        pedit.ui->_int->setText(QString::number(var.toInt()));
        break;
      case QVariant::Double:
        typeName = tr("Double");
        pedit.ui->_double->setText(QString::number(var.toDouble()));
        break;
      case QVariant::String:
        typeName = tr("String");
        pedit.ui->_string->setText(var.toString());
        break;
      case QVariant::List:
        typeName = tr("List");
        pedit.setList(var.toList());
        break;
      default:
        QMessageBox::warning(this, tr("Warning"),
                             tr("I do not know how to edit QVariant type %1.")
                               .arg(var.typeName()));
        return;
    };

    pedit.ui->_type->setCurrentIndex(ui->_type->findText(typeName));

    if (pedit.exec() == QDialog::Accepted)
    {
      var = pedit.value();

      item->setData(Qt::UserRole, var);
      item->setText(formatVariant(var));
    }
  }
}

void ParameterProperties::deleteItem()
{
  QListWidgetItem * item = ui->_list->currentItem();
  if(item)
    delete item;  
}

void ParameterProperties::moveItemUp()
{
  int row = ui->_list->currentRow();
  if(row <= 1)
    return;
  QListWidgetItem * item = ui->_list->takeItem(row);
  if (item)
    ui->_list->insertItem(row-1, item);
}

void ParameterProperties::moveItemDown()
{
  int row = ui->_list->currentRow();
  if(row >= ui->_list->count())
    return;
  QListWidgetItem * item = ui->_list->takeItem(row);
  if(item)
    ui->_list->insertItem(row+1, item);
}

void ParameterProperties::setActive(bool p)
{
  ui->_active->setChecked(p);
}

void ParameterProperties::setList( const QList<QVariant> & l )
{
  ui->_list->clear();
  
  QList<QVariant>::const_iterator it;
  for(it = l.begin(); it != l.end(); ++it) {
      new QListWidgetItem(formatVariant(*it), ui->_list);
  }
}

void ParameterProperties::setName(QString p)
{
  ui->_name->setText(p);
}

void ParameterProperties::setType(QString p)
{
  int idx = ui->_type->findText(p);
  if (idx >= 0)
    ui->_type->setCurrentIndex(idx);
  else if (QVariant::nameToType(qPrintable(p)) != QVariant::Invalid)
    setType(QVariant::nameToType(qPrintable(p)));
  else
    QMessageBox::warning(this, tr("Warning"),
                         tr("I do not recognize type %1.") .arg(p));
}

void ParameterProperties::setType(QVariant::Type p)
{
  QString typeName;
  switch(p)
  {
    case QVariant::Bool:
      typeName = tr("Bool");
      break;
    case QVariant::Int:
      typeName = tr("Int");
      break;
    case QVariant::Double:
      typeName = tr("Double");
      break;
    case QVariant::String:
      typeName = tr("String");
      break;
    case QVariant::List:
      typeName = tr("List");
      break;
    default:
      QMessageBox::warning(this, tr("Warning"),
                           tr("I do not recognize type %1.")
                           .arg(QVariant::typeToName(p)));
  }
  int idx = ui->_type->findText(typeName);
  if (idx >= 0)
    ui->_type->setCurrentIndex(idx);
}

void ParameterProperties::setValue(QVariant p)
{
  setType(p.type());
  switch (p.type())
  {
    case QVariant::Bool:
      ui->_bool->setCurrentIndex(p.toBool() ? 1 : 0);
      break;
    case QVariant::Int:
      ui->_int->setText(QString::number(p.toInt()));
      break;
    case QVariant::Double:
      ui->_double->setText(QString::number(p.toDouble()));
      break;
    case QVariant::String:
      ui->_string->setText(p.toString());
    break;
    case QVariant::List:
      setList(p.toList());
      break;
    default:
      ui->_string->setText(p.toString());
  }
}

bool ParameterProperties::active()
{
  return ui->_active->isChecked();
}

QList<QVariant> ParameterProperties::list()
{
  QList<QVariant> varlist;
  QListWidgetItem * item = 0;
  for(int row = 0; row < ui->_list->count(); row++)
  {
    item = ui->_list->item(row);
    varlist.append(item->data(Qt::UserRole));
  }
  return varlist;
}

QString ParameterProperties::name()
{
  return ui->_name->text();
}

QVariant::Type ParameterProperties::type()
{
  QVariant value;
  QString valueType = ui->_type->currentText();

  if (valueType == tr("String"))
    value = QVariant(ui->_string->text());
  else if (valueType == tr("Int"))
    value = QVariant(ui->_int->text().toInt());
  else if (valueType == tr("Double"))
    value = QVariant(ui->_double->text().toDouble());
  else if (valueType == tr("Bool"))
    value = QVariant(ui->_bool->currentText().toLower() == "true");
  else if (valueType == tr("List"))
    value = QVariant(list());
  else
    QMessageBox::warning(this, tr("Unknown Type"),
                         QString(tr("I do not understand the type %1."))
                             .arg(valueType));
  return value.type();
}

QString ParameterProperties::typeName()
{
  return ui->_type->currentText();
}

QVariant ParameterProperties::value()
{
  QVariant value;
  QString valueType = ui->_type->currentText();

  if (valueType == tr("String"))
    value = QVariant(ui->_string->text());
  else if (valueType == tr("Int"))
    value = QVariant(ui->_int->text().toInt());
  else if (valueType == tr("Double"))
    value = QVariant(ui->_double->text().toDouble());
  else if (valueType == tr("Bool"))
    value = QVariant(ui->_bool->currentText().toLower() == "true");
  else if (valueType == tr("List"))
    value = QVariant(list());
  else
    QMessageBox::warning(this, tr("Unknown Type"),
                         QString(tr("I do not understand the type %1."))
                             .arg(valueType));
  return value;
}
