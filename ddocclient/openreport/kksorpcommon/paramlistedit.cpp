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

#include "paramlistedit.h"
#include "ui_paramlistedit.h"

#include <QVariant>

ParamListEdit::ParamListEdit(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    ui = new Ui::ParamListEdit;
    ui->setupUi(this);

    // signals and slots connections
    connect(ui->_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->_select, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->_list, SIGNAL(itemSelectionChanged()), this, SLOT(sSelectionChanged()));
}

ParamListEdit::~ParamListEdit()
{
    // no need to delete child widgets, Qt does it all for us
}

void ParamListEdit::languageChange()
{
    ui->retranslateUi(this);
}

void ParamListEdit::sSelectionChanged()
{
  ui->_select->setEnabled(ui->_list->currentRow()!=-1);
}

