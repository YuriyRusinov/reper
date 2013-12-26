/***************************************************************************
                      qgsfeatureaction.h  -  description
                               ------------------
        begin                : 2010-09-20
        copyright            : (C) 2010 by J�rgen E. Fischer
        email                : jef at norbit dot de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSFEATUREACTION_H
#define QGSFEATUREACTION_H

#include "kksqgis_config.h"

#include "qgsfeature.h"
#include "qgsvectorlayertools.h"

#include <QList>
#include <QPair>
#include <QAction>

class QgsIdentifyResultsDialog;
class QgsVectorLayer;
class QgsHighlight;
class QgsAttributeDialog;

class KKSGISWidgetBase;

class _QGIS_EXPORT QgsFeatureAction : public QAction
{
    Q_OBJECT

  public:
    QgsFeatureAction( KKSGISWidgetBase * w, const QString &name, QgsFeature &f, QgsVectorLayer *vl, int action = -1, int defaultAttr = -1, QObject *parent = NULL );
    
    void setWorkingWidget( KKSGISWidgetBase * w);

  public slots:
    void execute();
    bool viewFeatureForm( QgsHighlight *h = 0 );
    bool editFeature();

    /**
     * Add a new feature to the layer.
     * Will set the default values to recently used or provider defaults based on settings
     * and override with values in defaultAttributes if provided.
     *
     * @param defaultAttributes  Provide some default attributes here if desired.
     *
     * @return true if feature was added
     */
    bool addFeature( const QgsAttributeMap& defaultAttributes = QgsAttributeMap() );

  private:
    QgsAttributeDialog *newDialog( bool cloneFeature );

    QgsVectorLayer *mLayer;
    QgsFeature &mFeature;
    int mAction;
    int mIdx;

    KKSGISWidgetBase * mWorkingWidget;

    static QMap<QgsVectorLayer *, QgsAttributeMap> mLastUsedValues;
};

#endif
