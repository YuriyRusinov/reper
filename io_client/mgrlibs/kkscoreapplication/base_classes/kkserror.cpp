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
#include "kkserror.h"
#include <QtDebug>
#include <QMessageBox>
#include <QDateTime>
#include <defines.h>

KKSError::KKSError()
{
    m_type = etNoError;
    m_code = "00000"; 
    m_name = "SUCCESSFUL COMPLETION"; 
    m_message = "SUCCESSFUL COMPLETION"; 
    m_group = "SUCCESSFUL COMPLETION";
}

KKSError::KKSError(ErrorTypes type, QString code, QString name, QString message, QString group)
{
    m_type = type;
    m_code = code; 
    m_name = name; 
    m_message = message; 
    m_group = group;
}

KKSError::~KKSError()
{
    if(log.isOpen())
        log.flush();
}

void KKSError::print(PrintFlags flags, QWidget * parent)
{
    if(flags & asQDebug){
        qDebug() << "ERROR! Code:" << code() << " : " << message();
    }
    if(flags & asQWarning){
        qWarning() << "ERROR! Code:" << code() << " : " << message().toUtf8().constData();
    }
    if(flags & asInfoDialog){
        QMessageBox::information(parent, 
                                 name(), 
                                 QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                                 QMessageBox::Ok);
    }
    if(flags & asWarningDialog){
        QMessageBox::warning(parent, 
                             name(), 
                             QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                             QMessageBox::Ok, 
                             QMessageBox::NoButton);
    }
    if(flags & asCriticalDialog){
        QMessageBox::critical(parent, 
                              name(), 
                              QString("Error code: %1\n\n%2\n%3").arg(code()).arg(name()).arg(message()), 
                              QMessageBox::Ok, 
                              QMessageBox::NoButton);
    }

    if(flags & asLogFile){
        if(!log.isOpen())
            return;

        QByteArray b, b1, b2;
        b = code().toLocal8Bit();
        b1 = message().toLocal8Bit();
        b2 = b + QByteArray(" : ") + b1;
        log.write( b2 );
        log.putChar('\n');
    }

}

void KKSError::init(ErrorTypes type, QString code, QString name, QString message, QString group)
{
    setType(type);
    setCode(code);
    setName(name);
    setMessage(message);
    setGroup(group);
}

int KKSError::setLogFile(QString & fileName)
{
    log.setFileName(fileName);
    bool ok = log.open(QFile::WriteOnly);
    if(!ok || !log.isOpen())
        return ERROR_CODE;

    log.write(QDateTime::currentDateTime().toString(Qt::TextDate).toLocal8Bit());
    log.putChar('\n');
    return OK_CODE;
}
