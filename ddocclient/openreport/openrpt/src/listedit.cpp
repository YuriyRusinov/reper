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

#include "listedit.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QList>

#include "booledit.h"
#include "intedit.h"
#include "stringedit.h"
#include "doubleedit.h"
#include "newvariant.h"

static QString formatVariant(const QVariant & var)
{
    QString value = var.toString();
    QString type = var.typeName();

    if(!type.isEmpty()) {
        value += " (" + type + ")";
    }
    return value;
}

ListEdit::ListEdit(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(_ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(_new, SIGNAL(clicked()), this, SLOT(newItem()));
    connect(_up, SIGNAL(clicked()), this, SLOT(moveItemUp()));
    connect(_edit, SIGNAL(clicked()), this, SLOT(editItem()));
    connect(_down, SIGNAL(clicked()), this, SLOT(moveItemDown()));
    connect(_delete, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

ListEdit::~ListEdit()
{
    // no need to delete child widgets, Qt does it all for us
}

void ListEdit::languageChange()
{
    retranslateUi(this);
}

void ListEdit::newItem()
{
    QListWidgetItem * lwi = 0;
    QVariant var;
    NewVariant newVar(this);
    newVar._lblName->hide();
    newVar._name->hide();
    if(newVar.exec() == QDialog::Accepted) {
	QString varType = newVar._type->currentText();
	BoolEdit * be = 0;
	IntEdit * ie = 0;
	DoubleEdit * de = 0;
	StringEdit * se = 0;
	ListEdit * le = 0;
	if(varType == tr("String")) {
	    se = new StringEdit(this);
	    se->_lblName->hide();
	    se->_name->hide();
	    se->_active->hide();
	    if(se->exec() == QDialog::Accepted) {
		var = QVariant(se->_value->text());
                lwi = new QListWidgetItem(formatVariant(var), _list);
                lwi->setData(Qt::UserRole, var);
	    }
	    delete se;
	    se = 0;
	} else if(varType == tr("Int")) {
	    ie = new IntEdit(this);
	    ie->_lblName->hide();
	    ie->_name->hide();
	    ie->_active->hide();
	    if(ie->exec() == QDialog::Accepted) {
		var = QVariant(ie->_value->text().toInt());
                lwi = new QListWidgetItem(formatVariant(var), _list);
                lwi->setData(Qt::UserRole, var);
	    }
	    delete ie;
	    ie = 0;
	} else if(varType == tr("Double")) {
	    de = new DoubleEdit(this);
	    de->_lblName->hide();
	    de->_name->hide();
	    de->_active->hide();
	    if(de->exec() == QDialog::Accepted) {
		var = QVariant(de->_value->text().toDouble());
                lwi = new QListWidgetItem(formatVariant(var), _list);
                lwi->setData(Qt::UserRole, var);
	    }
	    delete de;
	    de = 0;
	} else if(varType == tr("Bool")) {
	    be = new BoolEdit(this);
	    be->_lblName->hide();
	    be->_name->hide();
	    be->_active->hide();
	    if(be->exec() == QDialog::Accepted) {
		var = QVariant((bool)be->value());
                lwi = new QListWidgetItem(formatVariant(var), _list);
                lwi->setData(Qt::UserRole, var);
	    }
	    delete be;
	    be = 0;
	} else if(varType == tr("List")) {
	    le = new ListEdit(this);
	    le->_lblName->hide();
	    le->_name->hide();
	    le->_active->hide();
	    if(le->exec() == QDialog::Accepted) {
		var = QVariant(le->list());
                lwi = new QListWidgetItem(formatVariant(var), _list);
                lwi->setData(Qt::UserRole, var);
	    }
	    delete le;
	    le = 0;
	} else {
	    QMessageBox::warning(this, tr("Unknown Type"), QString(tr("I do not understand the type %1.")).arg(varType));
	}
    }
}


void ListEdit::editItem()
{
    QVariant var;
    QListWidgetItem * item = _list->currentItem();
    if(item) {
	QVariant var = item->data(Qt::UserRole);
	BoolEdit * be = 0;
	IntEdit * ie = 0;
	DoubleEdit * de = 0;
	StringEdit * se = 0;
	ListEdit * le = 0;
	switch(var.type()) {
	    case QVariant::Bool:
		be = new BoolEdit(this);
		be->_lblName->hide();
		be->_name->hide();
		be->_active->hide();
		be->setValue(var.toBool());
		if(be->exec() == QDialog::Accepted) {
                    var = QVariant((bool)be->value());
		    item->setData(Qt::UserRole, var);
                    item->setText(formatVariant(var));
		}
		delete be;
		be = 0;
		break;
	    case QVariant::Int:
		ie = new IntEdit(this);
		ie->_lblName->hide();
		ie->_name->hide();
		ie->_active->hide();
		ie->_value->setText(QString::number(var.toInt()));
		if(ie->exec() == QDialog::Accepted) {
		    var = QVariant(ie->_value->text().toInt());
		    item->setData(Qt::UserRole, var);
                    item->setText(formatVariant(var));
		}
		delete ie;
		ie = 0;
		break;
	    case QVariant::Double:
		de = new DoubleEdit(this);
		de->_lblName->hide();
		de->_name->hide();
		de->_active->hide();
		de->_value->setText(QString::number(var.toDouble()));
		if(de->exec() == QDialog::Accepted) {
		    var = QVariant(de->_value->text().toDouble());
		    item->setData(Qt::UserRole, var);
                    item->setText(formatVariant(var));
		}
		delete de;
		de = 0;
		break;
	    case QVariant::String:
		se = new StringEdit(this);
		se->_lblName->hide();
		se->_name->hide();
		se->_active->hide();
		se->_value->setText(var.toString());
		if(se->exec() == QDialog::Accepted) {
		    var = QVariant(se->_value->text());
		    item->setData(Qt::UserRole, var);
                    item->setText(formatVariant(var));
		}
		delete se;
		se = 0;
		break;
	    case QVariant::List:
		le = new ListEdit(this);
		le->_lblName->hide();
		le->_name->hide();
		le->_active->hide();
		le->setList(var.toList());
		if(le->exec() == QDialog::Accepted) {
		    var = QVariant(le->list());
		    item->setData(Qt::UserRole, var);
                    item->setText(formatVariant(var));
		}
		delete le;
		le = 0;
		break;
	    default:
	        QMessageBox::warning(this, tr("Warning"), QString(tr("I do not know how to edit QVariant type %1.")).arg(var.typeName()));
	};
    }
}


void ListEdit::deleteItem()
{
    QListWidgetItem * item = _list->currentItem();
    if(item) {
	delete item;  
    }
}


void ListEdit::moveItemUp()
{
    int row = _list->currentRow();
    if(row <= 1)
        return;
    QListWidgetItem * item = _list->takeItem(row);
    if(item) {
        _list->insertItem(row-1, item);
    }
}


void ListEdit::moveItemDown()
{
    int row = _list->currentRow();
    if(row >= _list->count())
        return;
    QListWidgetItem * item = _list->takeItem(row);
    if(item) {
        _list->insertItem(row+1, item);
    }
}


void ListEdit::setList( const QList<QVariant> & l )
{
    _list->clear();
    
    QList<QVariant>::const_iterator it;
    for(it = l.begin(); it != l.end(); ++it) {
	new QListWidgetItem(formatVariant(*it), _list);
    }
    
}


QList<QVariant> ListEdit::list()
{
    QList<QVariant> varlist;
    QListWidgetItem * item = 0;
    for(int row = 0; row < _list->count(); row++)
    {
        item = _list->item(row);
	varlist.append(item->data(Qt::UserRole));
    }
    return varlist;
}

