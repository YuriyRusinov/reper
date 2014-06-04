/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2008 by OpenMFG, LLC
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

#include "parsexmlutils.h"
#include "xsqlquery.h"
#include "parameteredit.h"
#include "paramlistedit.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>

#include "newvariant.h"
#include "booledit.h"
#include "intedit.h"
#include "doubleedit.h"
#include "stringedit.h"
#include "listedit.h"

ParameterEdit::ParameterEdit(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);

    _list->hide(); // parameter lists come from document definitions, so are only visible after a call to setDocument()
    _list->setDisabled(true);
    _edit->setDisabled(true);

    // signals and slots connections
    connect(_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(_ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(_new, SIGNAL(clicked()), this, SLOT(newItem()));
    connect(_edit, SIGNAL(clicked()), this, SLOT(edit()));
    connect(_list, SIGNAL(clicked()), this, SLOT(editItemList()));
    connect(_delete, SIGNAL(clicked()), this, SLOT(deleteItem()));
    connect(_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(doubleClick(int, int)));
    connect(_table, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChange()));
}

ParameterEdit::~ParameterEdit()
{
    // no need to delete child widgets, Qt does it all for us
}

void ParameterEdit::languageChange()
{
    retranslateUi(this);
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

  _list->show();	
  _new->hide();	
  _delete->hide();	

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
  for(r = 0; r < _table->rowCount(); r++) {
    if(_table->item(r, 1)->text() == name)
    {
      _table->item(r, 0)->setCheckState((active ? Qt::Checked : Qt::Unchecked));
      _table->item(r, 2)->setText(value.typeName());
      _table->item(r, 3)->setText(value.toString());
      return;
    }
  }

  // If we didn't find an existing parameter by the name specified add one
  r = _table->rowCount();
  _table->setRowCount(r+1);
  QTableWidgetItem * ctItem = 0;
  ctItem = new QTableWidgetItem();
  ctItem->setFlags(Qt::ItemIsUserCheckable);
  ctItem->setCheckState((active ? Qt::Checked : Qt::Unchecked));
  _table->setItem(r, 0, ctItem);
  _table->setItem(r, 1, new QTableWidgetItem(name));
  _table->setItem(r, 2, new QTableWidgetItem(value.typeName()));
  _table->setItem(r, 3, new QTableWidgetItem(value.toString()));
}

void ParameterEdit::newItem()
{
    NewVariant newVar(this);
    if(newVar.exec() == QDialog::Accepted) {
	QString varType = newVar._type->currentText();
	QString name = newVar._name->text();
	bool active = false;
	if(_params.contains(name)) {
	    QMessageBox::warning(this, tr("Name already exists"), tr("The name for the parameter you specified already exists in the list."));
	}
	BoolEdit * be = 0;
	IntEdit * ie = 0;
	DoubleEdit * de = 0;
	StringEdit * se = 0;
	ListEdit * le = 0;
	if(varType == tr("String")) {
	    se = new StringEdit(this);
	    se->_name->setText(name);
	    if(se->exec() == QDialog::Accepted) {
		_params[name] = QVariant(se->_value->text());
		active = se->_active->isChecked();
	    }
	    delete se;
	    se = 0;
	} else if(varType == tr("Int")) {
	    ie = new IntEdit(this);
	    ie->_name->setText(name);
	    if(ie->exec() == QDialog::Accepted) {
		_params[name] = QVariant(ie->_value->text().toInt());
		active = ie->_active->isChecked();
	    }
	    delete ie;
	    ie = 0;
	} else if(varType == tr("Double")) {
	    de = new DoubleEdit(this);
	    de->_name->setText(name);
	    if(de->exec() == QDialog::Accepted) {
		_params[name] = QVariant(de->_value->text().toDouble());
		active = de->_active->isChecked();
	    }
	    delete de;
	    de = 0;
	} else if(varType == tr("Bool")) {
	    be = new BoolEdit(this);
	    be->_name->setText(name);
	    if(be->exec() == QDialog::Accepted) {
		_params[name] = QVariant((bool)be->value());
		active = be->_active->isChecked();
	    }
	    delete be;
	    be = 0;
	} else if(varType == tr("List")) {
	    le = new ListEdit(this);
	    le->_name->setText(name);
	    if(le->exec() == QDialog::Accepted) {
		_params[name] = QVariant(le->list());
		active = le->_active->isChecked();
	    }
	    delete le;
	    le = 0;
	} else {
	    QMessageBox::warning(this, tr("Unknown Type"), QString(tr("I do not understand the type %1.")).arg(varType));
	    return;
	}
	int r = _table->rowCount();
	_table->setRowCount(r+1);
        QTableWidgetItem * ctItem = 0;
        ctItem = new QTableWidgetItem();
        ctItem->setFlags(Qt::ItemIsUserCheckable);
        ctItem->setCheckState((active ? Qt::Checked : Qt::Unchecked));
        _table->setItem(r, 0, ctItem);
        _table->setItem(r, 1, new QTableWidgetItem(name));
        QVariant var = _params[name];
        _table->setItem(r, 2, new QTableWidgetItem(var.typeName()));
        _table->setItem(r, 3, new QTableWidgetItem(var.toString()));
    }
}

void ParameterEdit::edit()
{
    if(_table->currentRow() != -1) 
    {
	editItem(_table->currentRow());
    }
}

void ParameterEdit::editItem(int row)
{
    bool do_update = false;
    bool active = _table->item(row, 0)->checkState() == Qt::Checked;
    QString name = _table->item(row, 1)->text();
    QVariant var = _params[name];
    BoolEdit * be = 0;
    IntEdit * ie = 0;
    DoubleEdit * de = 0;
    StringEdit * se = 0;
    ListEdit * le = 0;
    switch(var.type()) {
      case QVariant::Bool:
        be = new BoolEdit(this);
        be->_name->setText(name);
        be->_active->setChecked(active);
        be->setValue(var.toBool());
        if(be->exec() == QDialog::Accepted) {
        var = QVariant((bool)be->value());
        active = be->_active->isChecked();
        do_update = TRUE;
        }
        delete be;
        be = 0;
        break;
      case QVariant::Int:
        ie = new IntEdit(this);
        ie->_name->setText(name);
        ie->_active->setChecked(active);
        ie->_value->setText(QString::number(var.toInt()));
        if(ie->exec() == QDialog::Accepted) {
        var = QVariant(ie->_value->text().toInt());
        active = ie->_active->isChecked();
        do_update = TRUE;
        }
        delete ie;
        ie = 0;
        break;
      case QVariant::Double:
        de = new DoubleEdit(this);
        de->_name->setText(name);
        de->_active->setChecked(active);
        de->_value->setText(QString::number(var.toDouble()));
        if(de->exec() == QDialog::Accepted) {
        var = QVariant(de->_value->text().toDouble());
        active = de->_active->isChecked();
        do_update = TRUE;
        }
        delete de;
        de = 0;
        break;
      case QVariant::String:
        se = new StringEdit(this);
        se->_name->setText(name);
        se->_active->setChecked(active);
        se->_value->setText(var.toString());
        if(se->exec() == QDialog::Accepted) {
        var = QVariant(se->_value->text());
        active = se->_active->isChecked();
        do_update = TRUE;
        }
        delete se;
        se = 0;
        break;
      case QVariant::List:
        le = new ListEdit(this);
        le->_name->setText(name);
        le->_active->setChecked(active);
        le->setList(var.toList());
        if(le->exec() == QDialog::Accepted) {
        var = QVariant(le->list());
        active = le->_active->isChecked();
        do_update = TRUE;
        }
        delete le;
        le = 0;
        break;
      default:
        QMessageBox::warning(this, tr("Warning"), QString(tr("I do not know how to edit QVariant type %1.")).arg(var.typeName()));
    };
        
    if(do_update) {
        _params[name] = var;
        _table->item(row, 0)->setCheckState((active ? Qt::Checked : Qt::Unchecked));
        _table->item(row, 1)->setText(name);
        _table->item(row, 2)->setText(var.typeName());
        _table->item(row, 3)->setText(var.toString());
    }
}

void ParameterEdit::doubleClick(int row, int /*col*/)
{
    editItem(row);
}

void ParameterEdit::selectionChange()
{
    if(_table->currentRow() != -1)
    {
        _edit->setDisabled(false);

        int r = _table->currentRow();
        QString name = _table->item(r, 1)->text();
        QList<QPair<QString, QString> > list = _lists[name];
        if(list.size()>0) 
             _list->setDisabled(false); // the selected parameter is associated with a list
        else _list->setDisabled(true); 
    }
    else 
    {
        _list->setDisabled(true);
        _edit->setDisabled(true);
    }
}

void ParameterEdit::editItemList()
{
    if(_table->currentRow() == -1)
        return;

    int r = _table->currentRow();
    QString name = _table->item(r, 1)->text();
    QVariant var = _params[name];
    QList<QPair<QString, QString> > list = _lists[name];

    ParamListEdit newdlg(this);
    for(QList<QPair<QString, QString> >::iterator it = list.begin();
        it != list.end(); it++ )
    {
        QListWidgetItem * item = new QListWidgetItem((*it).second, newdlg._list);
        if((*it).first == var.toString())
            newdlg._list->setCurrentItem(item);
    }

    if(newdlg.exec() == QDialog::Accepted)
    {
        QVariant tmp(list[newdlg._list->currentRow()].first);
        if(tmp.convert(var.type()))
        {
            _params[name] = tmp;
            _table->item(r, 3)->setText(tmp.toString());
        }
    }
}


void ParameterEdit::deleteItem()
{
    if(_table->currentRow() != -1) {
        QString name = _table->item(_table->currentRow(), 1)->text();
        _params.remove(name);
        _table->removeRow(_table->currentRow());
    }
}


ParameterList ParameterEdit::getParameterList()
{
    ParameterList plist;

    QString name;
    QVariant value;
    for(int r = 0; r < _table->rowCount(); r++) {
        if(_table->item(r, 0)->checkState() == Qt::Checked) {
            name = _table->item(r, 1)->text();
            value = _params[name];
            plist.append(name, value);
        }
    }

    return plist;
}

