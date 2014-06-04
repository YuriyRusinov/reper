/*
 * Copyright (C) 2008 by Manuel Soriano
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
#include <QtGui>

#include "builtinSqlFunctions.h"

static const int __fieldTag = 0;
static const int __fieldDriver = 1;
static const int __fieldSql = 2;

static const int __fieldCount = 3;


static const char* __SqlTable[][3] = {
//{"tag",        "Driver",         "Sql Statement"}
// fmt01
  {"fmt01",      "QPSQL",          "SELECT text('0') AS \"0\""},
  {"fmt01",      "QOCI",          "SELECT '0' AS \"0\" from dual"},

// fmt02
  {"fmt02",      "QPSQL",          "SELECT %s(%f)"},
  {"fmt02",      "QOCI",          "SELECT to_char(%f,'999,999.99') from dual"},

// fmt03
  {"fmt03",      "QPSQL",          "SELECT formatDate(CURRENT_DATE) AS report_date,"
                                         " formatTime(CURRENT_TIMESTAMP) AS report_time"},
  {"fmt03",      "QOCI",          "SELECT to_char(sysdate,'YYYY/MM/DD') AS report_date,"
                                         " to_char(sysdate,'HH24:MI:SS') AS report_time"
                                    " from dual"},
// fmt04
  {"fmt04",      "QPSQL",          "SELECT report_grade, report_source "
                                        "  FROM report "
                                        " WHERE (report_name=:report_name)"
                                        " ORDER BY report_grade DESC LIMIT 1"},
  {"fmt04",      "QOCI",          "SELECT report_grade, report_source "
                                        "  FROM report "
                                        " WHERE (report_name=:report_name)"
                                        " ORDER BY report_grade HAVING ROWCOUNT < 1"},
// fmt05
  {"fmt05",      "QPSQL",          "SELECT login()"},
  {"fmt05",      "QOCI",          "SELECT 1 from dual"},

// fmt06
  {"fmt06",      "QPSQL",          "SELECT report_name, report_grade, report_source "
                                        "  FROM report"},
  {"fmt06",      "QOCI",          "SELECT report_name, report_grade, report_source "
                                        "  FROM report"},
// fmt07
  {"fmt07",      "QPSQL",          "SELECT CURRENT_DATE"},
  {"fmt07",      "QOCI",          "SELECT sysdate FROM dual"},

// fmt08
  {"fmt08",      "QPSQL",          "SELECT metric_value, CURRENT_USER AS username "
                                        " FROM metric "
                                        " WHERE (metric_name='DatabaseName')"},
  {"fmt08",      "QOCI",          "SELECT  SYS_CONTEXT ('USERENV', 'DB_NAME') || "
                                   " '.' || SYS_CONTEXT ('USERENV', 'DB_DOMAIN') as metric_value, "
                                   " SYS_CONTEXT ('USERENV', 'SESSION_USER') as username from dual"},
// fmt09
  {"fmt09",      "QPSQL",          "SELECT report_id "
                                        "  FROM report "
                                        " WHERE ((report_name=:report_name) "
                                        "   AND (report_grade=:report_grade))"},
  {"fmt09",      "QOCI",          "SELECT report_id "
                                        "  FROM report "
                                        " WHERE ((report_name=:report_name) "
                                        "   AND (report_grade=:report_grade))"},
// fmt10
  {"fmt10",      "QPSQL",          "UPDATE report "
                                        "   SET report_descrip=:report_desc, "
                                        "       report_source =:report_src "
                                        " WHERE ((report_id=:report_id) "
                                        "   AND (report_name=:report_name))"},
  {"fmt10",      "QOCI",          "UPDATE report "
                                        "   SET report_descrip=:report_desc, "
                                        "       report_source =:report_src "
                                        " WHERE ((report_id=:report_id) "
                                        "   AND (report_name=:report_name))"},
// fmt11
  {"fmt11",      "QPSQL",          "INSERT INTO report "
                                        "       (report_name, report_descrip, report_source, report_grade) "
                                        " VALUES (:report_name, :report_desc, :report_src, :report_grade)"},
  {"fmt11",      "QOCI",          "INSERT INTO report "
                                        "       (report_name, report_descrip, report_source, report_grade) "
                                        " VALUES (:report_name, :report_desc, :report_src, :report_grade)"},
// fmt12
  {"fmt12",      "QPSQL",          "SELECT report_id, report_name, report_grade "
                                        "  FROM report "
                                        "ORDER BY report_name, report_grade"},
// fmt13
  {"fmt13",      "QPSQL",          "SELECT report_source "
                                        "  FROM report "
                                        " WHERE report_id=:report_id "},
// fmt14
  {"fmt14",      "QPSQL",          "SELECT report_name "
                                        "  FROM report "
                                        " WHERE (report_id=:report_id)"},
// fmt15
  {"fmt15",      "QPSQL",          "SELECT report_grade "
                                        "  FROM report "
                                        " WHERE (report_id=:report_id)"},
// fmt16
  {"fmt16",      "QPSQL",          "ALTER TABLE metasql DISABLE TRIGGER metasqlaltertrigger;"},
// fmt17
  {"fmt17",      "QPSQL",          "SELECT saveMetasql(:group, :name, :notes, :query, :system, :schema) AS result;"},
// fmt18
  {"fmt18",      "QPSQL",          "ALTER TABLE metasql ENABLE TRIGGER metasqlaltertrigger;"},

  // get the available schemas
  {"fmt19",      "QPSQL",          "SELECT nspname,"
                                   "       CASE WHEN nspname = 'public' THEN 0 "
                                   "            ELSE 1 END AS sort "
                                   "  FROM pg_namespace, pg_class "
                                   " WHERE ((pg_namespace.oid=relnamespace)"
                                   "   AND  (relname in ('metasql', 'pkgmetasql'))) "
                                   "ORDER BY sort, nspname;" },

// crt00
  {"crt00",      "QODBC",          "Just fur fun"},			// Used only to validate the ODBC driver, the creation is depending on the server encapsulated by the ODBC itself
// crt01
  {"crt01",      "QPSQL",          "CREATE TABLE report ( "
                                        " report_id SERIAL PRIMARY KEY, "
                                        " report_name TEXT, "  
                                        " report_descrip TEXT NOT NULL, "
                                        " report_grade INTEGER NOT NULL DEFAULT 0, "
                                        " report_source TEXT) " },
// crt01
  {"crt01",      "QMYSQL",         "CREATE TABLE report ( "
                                        " report_id integer AUTO_INCREMENT PRIMARY KEY, "
                                        " report_name TEXT, "  
                                        " report_descrip TEXT NOT NULL, "
                                        " report_grade INTEGER NOT NULL DEFAULT 0, "
                                        " report_source TEXT) " },
// crt01
  {"crt01",      "QOCI",           "CREATE TABLE report ( "
                                        " report_id number PRIMARY KEY, "
                                        " report_name varchar(1024), "
                                        " report_descrip varchar2(1024) NOT NULL, "
                                        " report_grade integer default 0, "
                                        " report_source clob ) "},
// crt02 - ONLY For Oracle
  {"crt02",      "QOCI",           "create sequence openrpt_seq start with 1 increment by 1"},
// crt02 - ONLY For Oracle
  {"crt03",      "QOCI",           "create or replace trigger tr_openrpt "
                                        " before insert on report for each row "
                                        " begin "
                                        " if (:new.report_id is null) then "
                                        " select openrpt_seq.nextval into :new.report_id from dual; "
                                        " end if; "
                                        " end;"},

  {0,            0,                0} // DO NOT REMOVE -- null record
};

bool getSqlDriver(const QString & stDriver)
{
  for (int i = 0; __SqlTable[i][__fieldTag] != 0; i++)
    if (__SqlTable[i][__fieldDriver] == stDriver)
	{
      return true;
	}
	return false;
}

QString getSqlFromTag(const QString & stTag, const QString & stDriver)
{

  QString stDfltDriver = "QPSQL";

  // qDebug() << "getSqlFromTag :" << stTag << stDriver;

  for (int i = 0; __SqlTable[i][__fieldTag] != 0; i++)
    if ((__SqlTable[i][__fieldTag] == stTag) && (__SqlTable[i][__fieldDriver] == stDriver))
    {
      return __SqlTable[i][__fieldSql];
    }

  //
  // There is no statement for the driver, now with default driver
  //
  for (int i = 0; __SqlTable[i][__fieldTag] != 0; i++)
    if ((__SqlTable[i][__fieldTag] == stTag) && (__SqlTable[i][__fieldDriver] == stDfltDriver))
    {
      return __SqlTable[i][__fieldSql];
    }

  return QString::null;
}
