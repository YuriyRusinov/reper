/***************************************************************************
                          qgsvectorlayersaveasdialog.h
 Dialog to select destination, type and crs to save as ogr layers
                             -------------------
    begin                : Mon Mar 22 2010
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
#ifndef QGSVECTORLAYERSAVEASDIALOG_H
#define QGSVECTORLAYERSAVEASDIALOG_H

#include "kksqgis_config.h"

#include <ui_qgsvectorlayersaveasdialogbase.h>
#include <QDialog>
#include "qgscontexthelp.h"
#include "qgsvectorfilewriter.h"

/**
 *  Class to select destination file, type and CRS for ogr layrs
 */
class _QGIS_EXPORT QgsVectorLayerSaveAsDialog : public QDialog, private Ui::QgsVectorLayerSaveAsDialogBase
{
    Q_OBJECT

  public:
    // bitmask of options to be shown
    enum Options
    {
      Symbology = 1,
      AllOptions = ~0
    };

    QgsVectorLayerSaveAsDialog( long srsid, QWidget* parent = 0,  Qt::WFlags fl = 0 );
    QgsVectorLayerSaveAsDialog( long srsid, int options = AllOptions, QWidget* parent = 0,  Qt::WFlags fl = 0 );
    ~QgsVectorLayerSaveAsDialog();

    QString format() const;
    QString encoding() const;
    QString filename() const;
    QStringList datasourceOptions() const;
    QStringList layerOptions() const;
    long crs() const;
    bool skipAttributeCreation() const;
    bool addToCanvas() const;
    /**Returns type of symbology export.
        0: No symbology
        1: Feature symbology
        2: Symbol level symbology*/
    int symbologyExport() const;
    double scaleDenominator() const;

  private slots:
    void on_mFormatComboBox_currentIndexChanged( int idx );
    void on_mCRSSelection_currentIndexChanged( int idx );
    void on_browseFilename_clicked();
    void on_browseCRS_clicked();
    void on_buttonBox_helpRequested() { QgsContextHelp::run( metaObject()->className() ); }
    void on_mSymbologyExportComboBox_currentIndexChanged( const QString& text );
    //void on_mOptionsButton_toggled( bool checked );
    void accept();

  private:
    void setup();
    QList< QPair< QLabel*, QWidget* > > createControls( const QMap<QString, QgsVectorFileWriter::Option*>& options );

    long mCRS;
};

#endif // QGSVECTORLAYERSAVEASDIALOG_H
