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

#include "parameteredit.h"
#include "ui_parameteredit.h"
#include "ui_paramlistedit.h"

#include "parsexmlutils.h"
#include "xsqlquery.h"

#include "paramlistedit.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>

#include "parameterproperties.h"

ParameterEdit::ParameterEdit(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
{
    ui = new Ui::ParameterEdit;
    ui->setupUi(this);

  // OK and Cancel are only appropriate if the widget is a stand-alone window
  if (parent)
  {
    ui->_ok->hide();
    ui->_cancel->hide();
    ui->_buttonDiv->hide();
  }

  ui->_list->hide(); // parameter lists come from document definitions, so are only visible after a call to setDocument()
  ui->_list->setDisabled(true);
  ui->_edit->setDisabled(true);

  connect(ui->_new, SIGNAL(clicked()), this, SLOT(newItem()));
  connect(ui->_edit, SIGNAL(clicked()), this, SLOT(edit()));
  connect(ui->_list, SIGNAL(clicked()), this, SLOT(editItemList()));
  connect(ui->_delete, SIGNAL(clicked()), this, SLOT(deleteItem()));
  connect(ui->_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(doubleClick(int, int)));
  connect(ui->_table, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChange()));
}

ParameterEdit::~ParameterEdit()
{
    // no need to delete child widgets, Qt does it all for us
}

void ParameterEdit::languageChange()
{
    ui->retranslateUi(this);
}

bool ParameterEdit::setDocument(const QDomDocument & doc)
{
  QDomElement root = doc.documentElement();
  if(root.tagName() != "report")
  {
    QMessageBox::critical(this, tr("Not a Valid Report"),
      tr("The report definition does not appear to be a valid report."
         "\n\nThe root node is not 'report'."));
    return false;
  }

  ui->_list->show();	
  ui->_new->hide();	
  ui->_delete->hide();	

  for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
  {
    if(n.nodeName() == "parameter")
    {
      QDomElement elemSource = n.toElement();
      ORParameter param;

      param.name = elemSource.attribute("name");
      if(param.name.isEmpty())
        continue;
    
      param.type = elemSource.attribute("type");
      param.defaultValue  = elemSource.attribute("default");
      param.active = (elemSource.attribute("active") == "true");
      param.listtype = elemSource.attribute("listtype");
      
      QList<QPair<QString,QString> > pairs;
      if(param.listtype.isEmpty())
        param.description = elemSource.text();
      else
      {
        QDomNodeList section = elemSource.childNodes();
        for(int nodeCounter = 0; nodeCounter < section.count(); nodeCounter++)
        {
          QDomElement elemThis = section.item(nodeCounter).toElement();
          if(elemThis.tagName() == "description")
            param.description = elemThis.text();
          else if(elemThis.tagName() == "query")
            param.query = elemThis.text();
          else if(elemThis.tagName() == "item")
            param.values.append(qMakePair(elemThis.attribute("value"), elemThis.text()));
          else
            qDebug("While parsing parameter encountered an unknown element: %s",(const char*)elemThis.tagName().toLatin1().data());
        }
      }
      QVariant defaultVar;
      if(!param.defaultValue.isEmpty())
        defaultVar = QVariant(param.defaultValue);
      if("integer" == param.type)
        defaultVar = defaultVar.toInt();
      else if("double" == param.type)
        defaultVar = defaultVar.toDouble();
      else if("bool" == param.type)
        defaultVar = QVariant(defaultVar.toBool());
      else
        defaultVar = defaultVar.toString();
      updateParam(param.name, defaultVar, param.active);
      QList<QPair<QString, QString> > list;
      if("static" == param.listtype)
        list = param.values;
      else if("dynamic" == param.listtype && !param.query.isEmpty())
      {
        QSqlQuery qry(param.query);
        while(qry.next())
          list.append(qMakePair(qry.value(0).toString(), qry.value(1).toString()));
      }
      if(!list.isEmpty())
        _lists.insert(param.name, list);
    }
  }

  if(_lists.isEmpty())
    return false; // no defined parameters
  else 
    return true;
}

void ParameterEdit::updateParam(const QString & name, const QVariant & value, bool active)
{
  _params[name] = value;
  int r;
  for(r = 0; r < ui->_table->rowCount(); r++) {
    if(ui->_table->item(r, 1)->text() == name)
    {
      ui->_table->item(r, 0)->setCheckState((active ? Qt::Checked : Qt::Unchecked));
      ui->_table->item(r, 2)->setText(value.typeName());
      ui->_table->item(r, 3)->setText(value.toString());
      return;
    }
  }

  // If we didn't find an existing parameter by the name specified add one
  r = ui->_table->rowCount();
  ui->_table->setRowCount(r+1);
  QTableWidgetItem * ctItem = 0;
  ctItem = new QTableWidgetItem();
  ctItem->setFlags(Qt::ItemIsUserCheckable);
  ctItem->setCheckState((active ? Qt::Checked : Qt::Unchecked));
  ui->_table->setItem(r, 0, ctItem);
  ui->_table->setItem(r, 1, new QTableWidgetItem(name));
  ui->_table->setItem(r, 2, new QTableWidgetItem(value.typeName()));
  ui->_table->setItem(r, 3, new QTableWidgetItem(value.toString()));
}

void ParameterEdit::newItem()
{
  ParameterProperties pedit(this);
  if (pedit.exec() == QDialog::Accepted)
  {
    QString name = pedit.name();

    if(_params.contains(name))
      QMessageBox::warning(this, tr("Name already exists"),
                           tr("<p>The name for the parameter you specified "
                              "already exists in the list."));

    _params[name] = pedit.value();

    int r = ui->_table->rowCount();
    ui->_table->setRowCount(r+1);
    QTableWidgetItem * ctItem = 0;
    ctItem = new QTableWidgetItem();
    ctItem->setFlags(Qt::ItemIsUserCheckable);
    ctItem->setCheckState((pedit.active() ? Qt::Checked : Qt::Unchecked));
    ui->_table->setItem(r, 0, ctItem);
    ui->_table->setItem(r, 1, new QTableWidgetItem(name));
    ui->_table->setItem(r, 2, new QTableWidgetItem(pedit.value().typeName()));
    ui->_table->setItem(r, 3, new QTableWidgetItem(pedit.value().toString()));
  }
}

void ParameterEdit::edit()
{
    if(ui->_table->currentRow() != -1) 
    {
	editItem(ui->_table->currentRow());
    }
}

void ParameterEdit::editItem(int row)
{
  QString name = ui->_table->item(row, 1)->text();

  ParameterProperties pedit(this);
  pedit.setName(name);
  pedit.setValue(_params[name]);
  pedit.setActive(ui->_table->item(row, 0)->checkState() == Qt::Checked);

  if (pedit.exec() == QDialog::Accepted)
  {
    _params[name] = pedit.value();
    ui->_table->item(row, 0)->setCheckState((pedit.active() ? Qt::Checked
                                                        : Qt::Unchecked));
    ui->_table->item(row, 1)->setText(pedit.name());
    ui->_table->item(row, 2)->setText(pedit.value().typeName());
    ui->_table->item(row, 3)->setText(pedit.value().toString());
  }
}

void ParameterEdit::doubleClick(int row, int /*col*/)
{
    editItem(row);
}

void ParameterEdit::selectionChange()
{
    if(ui->_table->currentRow() != -1)
    {
        ui->_edit->setDisabled(false);

        int r = ui->_table->currentRow();
        QString name = ui->_table->item(r, 1)->text();
        QList<QPair<QString, QString> > list = _lists[name];
        if(list.size()>0) 
             ui->_list->setDisabled(false); // the selected parameter is associated with a list
        else ui->_list->setDisabled(true); 
    }
    else 
    {
        ui->_list->setDisabled(true);
        ui->_edit->setDisabled(true);
    }
}

void ParameterEdit::editItemList()
{
    if(ui->_table->currentRow() == -1)
        return;

    int r = ui->_table->currentRow();
    QString name = ui->_table->item(r, 1)->text();
    QVariant var = _params[name];
    QList<QPair<QString, QString> > list = _lists[name];

    ParamListEdit newdlg(this);
    for(QList<QPair<QString, QString> >::iterator it = list.begin();
        it != list.end(); it++ )
    {
        QListWidgetItem * item = new QListWidgetItem((*it).second, newdlg.ui->_list);
        if((*it).first == var.toString())
            newdlg.ui->_list->setCurrentItem(item);
    }

    if(newdlg.exec() == QDialog::Accepted)
    {
        QVariant tmp(list[newdlg.ui->_list->currentRow()].first);
        if(tmp.convert(var.type()))
        {
            _params[name] = tmp;
            ui->_table->item(r, 3)->setText(tmp.toString());
        }
    }
}


void ParameterEdit::deleteItem()
{
    if(ui->_table->currentRow() != -1) {
        QString name = ui->_table->item(ui->_table->currentRow(), 1)->text();
        _params.remove(name);
        ui->_table->removeRow(ui->_table->currentRow());
    }
}


ParameterList ParameterEdit::getParameterList()
{
    ParameterList plist;

    QString name;
    QVariant value;
    for(int r = 0; r < ui->_table->rowCount(); r++) {
        if(ui->_table->item(r, 0)->checkState() == Qt::Checked) {
            name = ui->_table->item(r, 1)->text();
            value = _params[name];
            plist.append(name, value);
        }
    }

    return plist;
}

void ParameterEdit::clear()
{
  while (ui->_table->rowCount() > 0)
    ui->_table->removeRow(0);
}

QDialog *ParameterEdit::ParameterEditDialog(ParameterEdit *p, QWidget *parent, Qt::WindowFlags f)
{
  QDialog *dlg = new QDialog(parent, f);
  QGridLayout *lyt = new QGridLayout(dlg);
  dlg->setLayout(lyt);
  lyt->addWidget(p);

  p->ui->_ok->show();
  p->ui->_cancel->show();
  p->ui->_buttonDiv->show();

  connect(p->ui->_cancel, SIGNAL(clicked()), dlg, SLOT(reject()));
  connect(p->ui->_ok,     SIGNAL(clicked()), dlg, SLOT(accept()));

  return dlg;
}
