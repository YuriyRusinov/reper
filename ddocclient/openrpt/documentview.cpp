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

#include "documentview.h"
#include "documentscene.h"
#include "reportgridoptions.h"
#include "reportpageoptions.h"
#include "reporthandler.h"
#include "graphicsitems.h"
#include "graphicssection.h"

// common
#include <xsqlquery.h>

// qt
#include <QtGui>
#include <QSqlError>
#include <QWheelEvent>

// images
#include <document.xpm>

DocumentView::DocumentView(DocumentScene * ds, QWidget * parent)
  : QGraphicsView(ds, parent)
{
  _ds = ds;
  verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}

void DocumentView::wheelEvent(QWheelEvent * e)
{
  e->accept();

  if(_ds->scaledTo() > 2 && e->delta() > 0)
      return;

  if(e->delta() < 0)
  {
      if(_ds->scaledTo() < .25)
          return;

      if(verticalScrollBar()->maximum() <= verticalScrollBar()->minimum()
          && horizontalScrollBar()->maximum() <= horizontalScrollBar()->minimum())
        return; // no need to zoom in further, the document is already fully visible      
  }

  double zoom = 1.0 + (e->delta() / 1000.0);

  scale(zoom, zoom);
  _ds->setScaledTo(_ds->scaledTo()* zoom);
}

void DocumentView::keyPressEvent ( QKeyEvent * event )
{
    switch(event->key())
    {
    case Qt::Key_Left:
        moveSelectedItems(-1,0);
        break;
    case Qt::Key_Right:
        moveSelectedItems(1,0);
        break;
    case Qt::Key_Up:
        moveSelectedItems(0,-1);
        break;
    case Qt::Key_Down:
        moveSelectedItems(0,1);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


void DocumentView::moveSelectedItems (int x, int y)
{
    double xfactor = 1; 
    double yfactor = 1; 

    ReportGridOptions *rgo = _ds->gridOptions();
    if(rgo)
    {
        xfactor = 100 * rgo->xInterval();
        yfactor = 100 * rgo->yInterval();
    }

    foreach(QGraphicsItem *item, items()) 
    {
        if(item->isSelected())
            item->moveBy(xfactor * x, yfactor * y);
    }
}

