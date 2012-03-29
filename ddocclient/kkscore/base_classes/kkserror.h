/***************************************************************************
 *   Copyright (C) 2004 by NIL-5                                           *
 *   root@shaman                                                           *
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
#ifndef KKSERROR_H
#define KKSERROR_H

//==============================
//======KKS CLIENT ERRORS=======
//==============================


#include <QString>
#include <QFile>
#include <QWidget>
#include "config_core.h"

/*!\ingroup SystemGroup
\brief Класс, представляющий собой описание ошибки, которая может возникнуть во время выполнения тех или иных операций.

Многие методы классов библиотеки возвращают ссылку на объект Error, который описывает последнюю возникшую ошибку. Кроме того, последнюю возникшую ошибку можно получить при помощи метода KKSSito::getLastError()*/
class __CORE_EXPORT KKSError
{
public:
    /*!\brief Перечисление возможных типов ошибок*/
    enum ErrorTypes
    {
        etNoError =         0x0000,//!<Нет ошибки
        etPGError =         0x0001,//!<Ошибка произошла в подсистеме связи с сервером СУБД (во время выполнения запроса, к примеру)
        etFactoryError =    0x0002 //!<Ошибка в подсистеме фабрик
    };
    
    enum PrintFlag
    {
        none = 0,
        asQDebug = 1,
        asQWarning = 2,
        asInfoDialog = 4,
        asWarningDialog = 8,
        asCriticalDialog = 16,
        asLogFile = 32
    };
    Q_DECLARE_FLAGS(PrintFlags, PrintFlag);

    KKSError();
    KKSError(ErrorTypes type, QString code, QString name, QString message, QString group);
    virtual ~KKSError();
       
    void setType(ErrorTypes type){m_type = type;}
    void setCode(QString code) {m_code = code;}
    void setName(QString name){m_name = name;}
    void setMessage(QString message) {m_message = message;}
    void setGroup(QString group) {m_group = group;}
    void setData(QString data) {m_data = data;}

    void init(ErrorTypes type, QString code, QString name, QString message, QString group);
   
    ErrorTypes type() {return m_type;}
    QString code() {return m_code;}
    QString name() {return m_name;}
    QString message() {return m_message;}
    QString group() {return m_group;}
    QString data() {return m_data;}

    virtual void print(PrintFlags flags = asQDebug, QWidget * parent = NULL);

    virtual int setLogFile(QString & fileName);
    QString logFile() {return log.fileName();}

private:
    ErrorTypes m_type;
    
    QString m_code;
    QString m_name;
    QString m_message;
    QString m_group;
    QString m_data;

    QFile log;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KKSError::PrintFlags)

#endif
