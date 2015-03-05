/***************************************************************************
                     qgsfeatureaction.cpp  -  description
                              -------------------
      begin                : 2010-09-20
      copyright            : (C) 2010 by Juergen E. Fischer
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
#include "qgsfeatureaction.h"

#include "qgisapp.h"
//ksa
#include "kksgiswidgetbase.h"

#include "qgsattributedialog.h"
#include "qgsdistancearea.h"
#include "qgsfeatureaction.h"
#include "qgsguivectorlayertools.h"
#include "qgsidentifyresultsdialog.h"
#include "qgslogger.h"
#include "qgsmapcanvas.h"
#include "qgsproject.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"

#include <QPushButton>
#include <QSettings>

QgsFeatureAction::QgsFeatureAction( const QString &name, QgsFeature &f, QgsVectorLayer *layer, int action, int defaultAttr, QObject *parent )
    : QAction( name, parent )
    , mLayer( layer )
    , mFeature( f )
    , mAction( action )
    , mIdx( defaultAttr )
    , mFeatureSaved( false )
{
}

void QgsFeatureAction::execute()
{
  mLayer->actions()->doAction( mAction, mFeature, mIdx );
}

QgsAttributeDialog *QgsFeatureAction::newDialog( bool cloneFeature )
{
  QgsFeature *f = cloneFeature ? new QgsFeature( mFeature ) : &mFeature;

  QgsAttributeEditorContext context;

  QgsDistanceArea myDa;

  myDa.setSourceCrs( mLayer->crs() );
  myDa.setEllipsoidalMode( QgisApp::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
  myDa.setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );

  context.setDistanceArea( myDa );
  context.setVectorLayerTools( QgisApp::instance()->vectorLayerTools() );

  QgsAttributeDialog *dialog = new QgsAttributeDialog( mLayer, f, cloneFeature, NULL, true, context );

  if ( mLayer->actions()->size() > 0 )
  {
    dialog->setContextMenuPolicy( Qt::ActionsContextMenu );

    QAction *a = new QAction( tr( "Run actions" ), dialog );
    a->setEnabled( false );
    dialog->addAction( a );

    for ( int i = 0; i < mLayer->actions()->size(); i++ )
    {
      const QgsAction &action = mLayer->actions()->at( i );

      if ( !action.runable() )
        continue;

      QgsFeatureAction *a = new QgsFeatureAction( action.name(), *f, mLayer, i, -1, dialog );
      dialog->addAction( a );
      connect( a, SIGNAL( triggered() ), a, SLOT( execute() ) );

      QAbstractButton *pb = dialog->findChild<QAbstractButton *>( action.name() );
      if ( pb )
        connect( pb, SIGNAL( clicked() ), a, SLOT( execute() ) );
    }
  }

  return dialog;
}

bool QgsFeatureAction::viewFeatureForm( QgsHighlight *h )
{
  if ( !mLayer )
    return false;

  QgsAttributeDialog *dialog = newDialog( true );
  dialog->setHighlight( h );
  dialog->show(); // will also delete the dialog on close (show() is overridden)

  return true;
}

bool QgsFeatureAction::editFeature( bool showModal )
{
  if ( !mLayer )
    return false;

  QgsAttributeDialog *dialog = newDialog( false );

  if ( !mFeature.isValid() )
    dialog->setIsAddDialog( true );

  if ( showModal )
  {
    dialog->setAttribute( Qt::WA_DeleteOnClose );
    int rv = dialog->exec();

    mFeature.setAttributes( dialog->feature()->attributes() );
    return rv;
  }
  else
  {
    dialog->show(); // will also delete the dialog on close (show() is overridden)
  }

  return true;
}

bool QgsFeatureAction::addFeature( const QgsAttributeMap& defaultAttributes, bool showModal )
{
  if ( !mLayer || !mLayer->isEditable() )
    return false;

  QgsVectorDataProvider *provider = mLayer->dataProvider();

  QSettings settings;
  bool reuseLastValues = settings.value( "/qgis/digitizing/reuseLastValues", false ).toBool();
  QgsDebugMsg( QString( "reuseLastValues: %1" ).arg( reuseLastValues ) );

  // add the fields to the QgsFeature
  const QgsFields& fields = mLayer->pendingFields();
  mFeature.initAttributes( fields.count() );
  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    QVariant v;

    if ( defaultAttributes.contains( idx ) )
    {
      v = defaultAttributes.value( idx );
    }
    else if ( reuseLastValues && sLastUsedValues.contains( mLayer ) && sLastUsedValues[ mLayer ].contains( idx ) )
    {
      v = sLastUsedValues[ mLayer ][idx];
    }
    else
    {
      v = provider->defaultValue( idx );
    }

    mFeature.setAttribute( idx, v );
  }

  // show the dialog to enter attribute values
  bool isDisabledAttributeValuesDlg = settings.value( "/qgis/digitizing/disable_enter_attribute_values_dialog", false ).toBool();
  // override application-wide setting with any layer setting
  switch ( mLayer->featureFormSuppress() )
  {
    case QgsVectorLayer::SuppressOn:
      isDisabledAttributeValuesDlg = true;
      break;
    case QgsVectorLayer::SuppressOff:
      isDisabledAttributeValuesDlg = false;
      break;
    case QgsVectorLayer::SuppressDefault:
      break;
  }
  if ( isDisabledAttributeValuesDlg )
  {
    mLayer->beginEditCommand( text() );
    mFeatureSaved = mLayer->addFeature( mFeature );

    if ( mFeatureSaved )
      mLayer->endEditCommand();
    else
      mLayer->destroyEditCommand();
  }
  else
  {
/*ksa код из исходной версии qgis 2.6.1
    QgsAttributeDialog *dialog = newDialog( false );
    dialog->setIsAddDialog( true );
    dialog->setEditCommandMessage( text() );

    connect( dialog->attributeForm(), SIGNAL( featureSaved( const QgsFeature & ) ), this, SLOT( onFeatureSaved( const QgsFeature & ) ) );

    if ( !showModal )
    {
      setParent( dialog ); // keep dialog until the dialog is closed and destructed
      dialog->show(); // will also delete the dialog on close (show() is overridden)
      return true;
    }

    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->exec();
*/

    //ksa
    //пробуем открыть форму DynamicDocs, если создается новый объект в слое DynamicDocs.
    //определяем по наличию атрибута unique_id
    bool uniqueIdFound = false;
    const QgsFields * flds = mFeature.fields();
    QString providerName = mLayer->dataProvider()->name();
    if(providerName == "postgres" && flds){
        //если поле unique_id присутствует, то считаем, что слой из нашей БД. Поэтому показываем нашу форму информационного ресурса
        int indx = flds->indexFromName("unique_id");
        if(indx > -1){
            uniqueIdFound = true;

            QString uri = mLayer->dataProvider()->dataSourceUri();
            QStringList uriSections = uri.split(" ");
            QString layerTable;

            for(int i=0; i<uriSections.count(); i++){
                QString & sec = uriSections[i];
                if(sec.startsWith("table=")){
                    QStringList tableSec = sec.split("=");
                    if(tableSec.count() != 2){
                        return false;
                    }
                    QString fullTableName = tableSec[1];
                    QStringList tableNameSec = fullTableName.split(".");
                    if(tableNameSec.count() != 2){
                        return false;
                    }
                    layerTable = tableNameSec[1];
                    layerTable.replace("\"", "");
                    break;
                }
            }
            
            //поскольку класс QgsGeometry не имеет метода exportToEWkt, необходимо передать EWKT-строку самостоятельно
            QString geomAsEWKT = mFeature.geometry()->exportToWkt();
            long srid = mLayer->crs().postgisSrid();
            geomAsEWKT = QString("SRID=%1;%2").arg(srid).arg(geomAsEWKT);

            //ksa
            bool ok = ((KKSGISWidgetBase *)QgisApp::instance())->featureFromEIO(QgisApp::instance()->mapCanvas(), mFeature, geomAsEWKT, layerTable);
            if(ok){
                //bool res = mLayer->addFeature( mFeature ); //не надо пытаться добавить объект в слой. 
                                                             //В этом случае система попытается создать его в БД самостоятельно,
                                                             //что приведет к ошибке
                                                             //просто перезагрузим слой из БД, т.к. объект был создан в методе featureFromEIO
                ((KKSGISWidgetBase *)QgisApp::instance())->reloadLayer(mLayer->id());
                mFeatureSaved = true;
            }
            else{
                QgsDebugMsg( "Adding feature to layer failed" );
                mFeatureSaved = false;
            }
        }
    }
    

    //-------------
    if(!uniqueIdFound){
        QgsAttributeDialog *dialog = newDialog( false );
        dialog->setIsAddDialog( true );
        dialog->setEditCommandMessage( text() );

        connect( dialog->attributeForm(), SIGNAL( featureSaved( const QgsFeature & ) ), this, SLOT( onFeatureSaved( const QgsFeature & ) ) );

        if ( !showModal )
        {
          setParent( dialog ); // keep dialog until the dialog is closed and destructed
          dialog->show(); // will also delete the dialog on close (show() is overridden)
          return true;
        }

        dialog->setAttribute( Qt::WA_DeleteOnClose );
        dialog->exec();
    }
    //-----------------
    //ksa

  }

  // Will be set in the onFeatureSaved SLOT
  return mFeatureSaved;
}

void QgsFeatureAction::onFeatureSaved( const QgsFeature& feature )
{
  QgsAttributeForm* form = qobject_cast<QgsAttributeForm*>( sender() );
  Q_UNUSED( form ) // only used for Q_ASSERT
  Q_ASSERT( form );

  mFeatureSaved = true;

  QSettings settings;
  bool reuseLastValues = settings.value( "/qgis/digitizing/reuseLastValues", false ).toBool();
  QgsDebugMsg( QString( "reuseLastValues: %1" ).arg( reuseLastValues ) );

  if ( reuseLastValues )
  {
    QgsFields fields = mLayer->pendingFields();
    for ( int idx = 0; idx < fields.count(); ++idx )
    {
      const QgsAttributes &newValues = feature.attributes();
      QgsAttributeMap origValues = sLastUsedValues[ mLayer ];
      if ( origValues[idx] != newValues[idx] )
      {
        QgsDebugMsg( QString( "saving %1 for %2" ).arg( sLastUsedValues[ mLayer ][idx].toString() ).arg( idx ) );
        sLastUsedValues[ mLayer ][idx] = newValues[idx];
      }
    }
  }
}

QMap<QgsVectorLayer *, QgsAttributeMap> QgsFeatureAction::sLastUsedValues;
