/***************************************************************************
 *   Copyright (C) 2006 by NIL-5                                           *
 *   yrusinov@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _KKS_SET_H
#define _KKS_SET_H

#include <QSettings>
#include <QString>
#include <QStringList>
#include "config_coreapplication.h"

//class SaveSettingsForm;

/*! \class KKSSettings
\ingroup SystemGroup
\brief ����� ������������� ��������� ���������

����� ������������ ��� ���������� � ��������� �������� ������.
� ������ ������ ���������� ���������� �������� ��������� �� ��������� ������� ������. �� ��� �������� ����� KKSApplication (������ KKSApplication::getKKSSettings()).

����� ��������, ��� ������ �������� ������ ���� ��������� �� KKSSettings. �.�. � ��������� ����� ��������������
������ ���� ����� �������� ������������. ������ ����� ����� ������ ��������� ������������, �.�. ������� ��������� ������� ������ �������������� ����������.

������ � ����������� ������������ � ������� ���������� ������ ���������� Qt QSettings. � ������ ������ �������� ������ ��������������, 
������ � �������������� ���������� � ������ ��������� �� �������� ����� ������ � ����� ���������.*/

class __COREAPP_EXPORT KKSSettings : public QSettings
{
    public:
        QString getParam ( const QString& keyname ) const;
        QString getGroupParam ( const QString& group, const QString& keyname );

    public slots:
        void editSettings ( QWidget * parent = NULL );
        void writeSettings ( const QString & group, const QString & key, const QString & val );
        void readSettings ( void );

    private:
        friend class KKSCoreApplication;
        KKSSettings ( const QString & organization, const QString & application, QObject * parent = NULL );
        KKSSettings ( const QString & fileName, Format format = QSettings::IniFormat, QObject * parent=NULL );
        KKSSettings ( QObject * parent=NULL );
        ~KKSSettings ( void );

    private:
        Q_OBJECT
};

#endif
