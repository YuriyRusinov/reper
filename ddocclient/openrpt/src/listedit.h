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

#ifndef LISTEDIT_H
#define LISTEDIT_H

#include <QDialog>

#include "ui_listedit.h"

class ListEdit : public QDialog, public Ui::ListEdit
{
    Q_OBJECT

  public:
    ListEdit(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ListEdit();

    virtual QList<QVariant> list();

  public slots:
    virtual void setList( const QList<QVariant> & l );

  protected slots:
    virtual void languageChange();

    virtual void newItem();
    virtual void editItem();
    virtual void deleteItem();
    virtual void moveItemUp();
    virtual void moveItemDown();
};

#endif // LISTEDIT_H
