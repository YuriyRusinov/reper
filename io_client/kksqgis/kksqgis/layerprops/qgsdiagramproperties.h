/***************************************************************************
  qgsdiagramproperties.h
  Properties for diagram layers
  -------------------
         begin                : August 2012
         copyright            : (C) Matthias Kuhn
         email                : matthias dot kuhn at gmx dot ch

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSDIAGRAMPROPERTIES_H
#define QGSDIAGRAMPROPERTIES_H

#include "kksqgis_config.h"

#include <QDialog>
#include <ui_qgsdiagrampropertiesbase.h>

class QgsVectorLayer;
class KKSGISWidgetBase;
class _QGIS_EXPORT QgsDiagramProperties : public QWidget, private Ui::QgsDiagramPropertiesBase
{
    Q_OBJECT

  public:
    QgsDiagramProperties( QgsVectorLayer* layer, QWidget* parent );
    /**Adds an attribute from the list of available attributes to the assigned attributes with a random color.*/
    void addAttribute( QTreeWidgetItem * item );
    void setWorkingWidget(KKSGISWidgetBase * w);

  public slots:
    void apply();
    void on_mDiagramTypeComboBox_currentIndexChanged( int index );
    void on_mTransparencySlider_valueChanged( int value );
    void on_mAddCategoryPushButton_clicked();
    void on_mAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column );
    void on_mFindMaximumValueButton_clicked();
    void on_mDisplayDiagramsGroupBox_toggled( bool checked );
    void on_mRemoveCategoryPushButton_clicked();
    void on_mDiagramFontButton_clicked();
    void on_mDiagramAttributesTreeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column );
    void on_mEngineSettingsButton_clicked();
    void showSizeAttributeExpressionDialog();
    void showAddAttributeExpressionDialog();

  protected:
    QFont mDiagramFont;

    QgsVectorLayer* mLayer;

  private:
    KKSGISWidgetBase * mWorkingWidget;
    int mAvailableAttributes;

};

#endif // QGSDIAGRAMPROPERTIES_H
