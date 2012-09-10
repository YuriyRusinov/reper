#pragma once

/*! \mainpage Api Bdfz
 *
 * \author ������ �.�. ���. 45 ��� "��� ��"
 * \date 09.08.2012
 *
 * \section intro_sec ������� ��������
 *
 * ��� ���������� ������������� ��� ������� � ������ ����� ���, �����, ���, ���, ���.
 *
 */

#include <QtCore/qglobal.h>

#if defined(API_BDFZ_LIBRARY)
#  define API_BDFZ_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define API_BDFZ_SHARED_EXPORT Q_DECL_IMPORT
#endif

//#define API_BDFZ_SHARED_EXPORT

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QPair>
#include <QFile>
#include <QObject>
#include <QDomElement>
#include <QMutex>

#define VTO_TYPE tr("������� �������� ���")
#define TTX_TYPE tr("��� ������� ��� ����������")
#define REQUEST_TYPE tr("������ �� �� ���")
#define RBP_TYPE tr("������ ������� ���������� ���")
#define POR_TYPE tr("������� �������� �������� ��� ��� ��")
#define PG_TYPE tr("����������� �� ���")
#define ETK_TYPE tr("����������� ����� ���������")
#define UGSH_TYPE tr("�������� �� �� �� �� ��")
#define EOIRD_TYPE tr("�����")
#define EFO_TYPE tr("���")
#define GMO_TYPE tr("�������� ������ �� ���")
#define NVO_TYPE tr("������������-��������� ����������")
#define SKI_TYPE tr("�������� ������������� ����������")
#define CLASSIFICATOR_TYPE tr("�������������� ��������� ����")

#define DICT_ETK_TYPES tr("���� ���")
#define DICT_ORGANIZATIONS tr("��� � �������� ������������")
#define DICT_COUNTRIES tr("������ ����")
#define DICT_ONTYPES tr("���� �������� ����������")
#define DICT_FORECASTTYPES tr("���� ��������� ������")
#define DICT_REGIONS tr("�������������� ������")
#define DICT_WEAPONS tr("���� ���������� � ������� �������")
#define DICT_VS_TYPES tr("���� � ���� ��")
#define DICT_VTO_USAGE_FORMS tr("����� ���������� ���")

class Siu_api2;

/**
* @brief ��������� ������� ���������� �������
*/
struct API_BDFZ_SHARED_EXPORT Param_IO
{
	explicit Param_IO();
    /**
    * @brief ���� ���������� ��, � ������� ���� ������ (� ��������� �� ������)
    */
    QDateTime updated_from;

    /**
    * @brief ���� ���������� ��, �� ������� ���� ������ (� ��������� �� ������)
    */
    QDateTime updated_to;
};

/**
* @brief ��������� ���������� ������� ������� �������� ���
*/
struct API_BDFZ_SHARED_EXPORT Param_VTO:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_VTO();

    /**
    * @brief UID ���� ��� ���� �� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listVSTypes ������ ApiBdfz)
    */
    QString uid_vs_type;

    /**
    * @brief UID ����� ���������� ��� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listVTOUsageForms ������ ApiBdfz)
    */
    QString uid_vto_usage_form;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ��� ������� ��� ����������
*/
struct API_BDFZ_SHARED_EXPORT Param_TTX:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_TTX();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

	/**
    * @brief UID ���� ���������� � ������� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listWeapons ������ ApiBdfz)
    */
    QString uid_weapon_type;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ������� ������� ���������� ���
*/
struct API_BDFZ_SHARED_EXPORT Param_RBP:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_RBP();

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ��������� ������� ���� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ������ �� �� ���
*/
struct API_BDFZ_SHARED_EXPORT Param_Request:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_Request();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ��� ������ ("��" - 3.1.1, "���" - 3.1.2, "���" - 3.1.3, "���" - 3.1.4)
    */
    QString type;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� �������� �������� ��������
*/
struct API_BDFZ_SHARED_EXPORT Param_POR:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_POR();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ������������ �� ���
*/
struct API_BDFZ_SHARED_EXPORT Param_PG:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_PG();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� �������� �� �� �� �� ��
*/
struct API_BDFZ_SHARED_EXPORT Param_UGSH:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_UGSH();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� �������� ������������� ����������
*/
struct API_BDFZ_SHARED_EXPORT Param_SKI:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_SKI();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ���������� �������� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ����
*/
struct API_BDFZ_SHARED_EXPORT Param_ETK:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_ETK();

    /**
    * @brief ������������ (���� ������ ������ - � ������ �� �����������)
    */
    QString nomenclature;

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ����������� �������� (1000000, 500000, 200000, 100000, 50000) (���� ����� 0 - � ������ �� �����������)
    */
    long scale;

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ��������� ������� ���� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� �����
*/
struct API_BDFZ_SHARED_EXPORT Param_EOIRD:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_EOIRD();

    /**
    * @brief ����� �� �-500 (���� ����� 0 - � ������ �� �����������)
    */
    int number_t500;

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ���������� ������� ���������� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

	/**
    * @brief UID ���� ������� ���������� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listONTypes ������ ApiBdfz)
    */
    QString uid_type_obj;

	/**
    * @brief UID ������ ��������������� �������������� ������� ���������� (���� ������ ������ ������ - �� ����������� � ������)  (�� ����� listCountries ������ ApiBdfz)
    */
    QString uid_nationality;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ���
*/
struct API_BDFZ_SHARED_EXPORT Param_EFO:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_EFO();

    /**
    * @brief ����� �� �-500 (���� ����� 0 - � ������ �� �����������)
    */
    int number_t500;

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ���������� ������� ���������� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

	/**
    * @brief UID ���� ������� ���������� (���� ������ ������ ������ - �� ����������� � ������)  (�� ����� listONTypes ������ ApiBdfz)
    */
    QString uid_type_obj;

	/**
    * @brief UID ������ ��������������� �������������� ������� ���������� (���� ������ ������ ������ - �� ����������� � ������)  (�� ����� listCountries ������ ApiBdfz)
    */
    QString uid_nationality;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};


/**
* @brief ��������� ���������� ������� ����������������������� �����������
*/
struct API_BDFZ_SHARED_EXPORT Param_GMO:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_GMO();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief WKT-������, ���������� � ���� �������� ��������, ������� ������ �������� � ���� ��������� ������� ��������� (���� ������ ������ - � ������ �� �����������)
    */
    QString polygon;

	/**
    * @brief UID ���� ����������� (���� ������ ������ ������ - �� ����������� � ������)  (�� ����� listForecastTypes ������ ApiBdfz)
    */
    QString uid_forecast_type;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ��������� ���������� ������� ������������-���������� �����������
*/
struct API_BDFZ_SHARED_EXPORT Param_NVO:Param_IO
{
    /**
    * @brief ����������� �� ���������, ��������� ��� ���� ����������, ������������ �� ��������������� ������� ��������� � �������
    */
    explicit Param_NVO();

    /**
    * @brief ����, � ������� ���� ������
    */
    QDate date_from;

    /**
    * @brief ����, �� ������� ���� ������
    */
    QDate date_to;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������, 9-�����)
    */
    short constraints;
};

/**
* @brief ������� ��������� � ����������� ������ ��
*/
struct API_BDFZ_SHARED_EXPORT Result_IO
{
    Result_IO();

    /**
    * @brief ���� � ����� ���������� �� (� ��������� �� ������)
    */
    QDateTime updated;

	/**
    * @brief ������������ ��
    */
    QString io_name;

    /**
    * @brief ���������� ������������� ��
    */
    QString uid;

    /**
    * @brief ����� ��
    */
    QString author;

    /**
    * @brief ��� �����
    */
    QString file_name;

    /**
    * @brief ������ �� ���� (����� ��� ����������� �����)
    */
    QString file_link;

    /**
    * @brief ���������� ������������� ����� (?)
    */
    QString file_uid;

    /**
    * @brief �����������, ��������� �������������� ������
    */
    QString organization;  

    /**
    * @brief ������� �����������
    */
    int constraint;  
};

/**
* @brief ��������� � ����������� ������ ������� �������� ���
*/
struct API_BDFZ_SHARED_EXPORT Result_VTO:Result_IO
{
    Result_VTO();

    /**
    * @brief ��� ��� ��� ��
    */
    QString vs_type;

	/**
    * @brief ����� ���������� ���
    */
    QString vto_usage_form;

	/**
    * @brief �������� ����� ����� ��������
    */
	QString plan_number;

	/**
    * @brief �������� ����� �������� �����
    */
	QString variant_number;

	/**
    * @brief ����� ���
    */
	QString rbp_number;
};

/**
* @brief ��������� � ����������� ������ ��� ��� ��
*/
struct API_BDFZ_SHARED_EXPORT Result_TTX:Result_IO
{
    Result_TTX();

    /**
    * @brief ���� ����������
    */
    QDate date;

	/**
    * @brief ��� ������� ���
    */
    QString weapon_type;

	/**
    * @brief ����� ������
    */
	QString number;
};

/**
* @brief ��������� � ����������� ������ �� � �������� �� �� ���
*/
struct API_BDFZ_SHARED_EXPORT Result_Request:Result_IO
{
    Result_Request();

    /**
    * @brief ���� �������������
    */
    QDate date;

    /**
    * @brief ����� ������
    */
    QString number;

    /**
    * @brief ��� ������ ("��" - 3.1.1, "���" - 3.1.2, "���" - 3.1.3, "���" - 3.1.4)
    */
    QString type;
};

/**
* @brief ��������� � ����������� ������ �� � ���������� �� �� �� �� ��
*/
struct API_BDFZ_SHARED_EXPORT Result_UGSH:Result_IO
{
    Result_UGSH();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ������� ����� ��������� � ����������������
    */
	QString outgoing_number;

	/**
    * @brief ������� ����� ��������� � ����������������
    */
	QString record_number;
};

/**
* @brief ��������� � ����������� ������ �� � ��������� �������� ��������
*/
struct API_BDFZ_SHARED_EXPORT Result_POR:Result_IO
{
    Result_POR();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ���������
    */
	QString category;	

	/**
    * @brief �������
    */
	QString addressee;
};

/**
* @brief ��������� � ����������� ������ �� � �������-��������� �� ���
*/
struct API_BDFZ_SHARED_EXPORT Result_PG:Result_IO
{
    Result_PG();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ���������
    */
	QString category;

	/**
    * @brief ����� ���
    */
	QString rbp_number;

	/**
    * @brief �������
    */
	QString addressee;
};

/**
* @brief ��������� � ����������� ������ �� � �������� ������� ���������� ���
*/
struct API_BDFZ_SHARED_EXPORT Result_RBP:Result_IO
{
    Result_RBP();

    /**
    * @brief ����� ���
    */
	QString rbp_number;

	/**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;
};

/**
* @brief ��������� � ����������� ������ �� � �������� ������������� �����������
*/
struct API_BDFZ_SHARED_EXPORT Result_SKI:Result_IO
{
    Result_SKI();

    /**
    * @brief ���� ������ ������
    */
    QDate season_begin;

	/**
    * @brief ���� ��������� ������
    */
    QDate season_end;

    /**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;    
};

/**
* @brief ��������� � ����������� ������ �� � �������
*/
struct API_BDFZ_SHARED_EXPORT Result_ETK:Result_IO
{
    Result_ETK();

    /**
    * @brief ������������ �����
    */
    QString nomenclature;

    /**
    * @brief ���� �������� �����
    */
    QDate creation_date;

    /**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;

    /**
    * @brief ����������� ��������
    */
    long scale;

    /**
    * @brief �������� �����
    */
    QString sheet_name;

    /**
    * @brief ��� ����� ��������������
    */
    QString classificator_name;

	/**
    * @brief UID ��������������
    */
    QString classificator_uid;

	/**
    * @brief ������ �� ���� ��������������
    */
    QString classificator_link;
};

/**
* @brief ��������� � ����������� ������ �� � �����
*/
struct API_BDFZ_SHARED_EXPORT Result_EOIRD:Result_IO
{
    Result_EOIRD();

	/**
	* @brief ����� �-500
	*/
	quint32 number_t; 

	/**
	* @brief ����� �-200
	*/
	quint32 number_k;

	/**
	* @brief  ��� ��
	*/
	QString type_obj;

	/**
	* @brief ������������ ��
	*/
	QString name;

	/**
	* @brief ������ ����������
	*/
	QString country;

	/**
	* @brief ��������������� ��������������
	*/
	QString nationality;

	/**
	* @brief ������������� �����������
	*/
	QString executer;

	/**
	* @brief ���� ����������
	*/
	QDate development_date;

	/**
	* @brief ���� ���������, �� �������� ���������� ��������
	*/
	QDate material_date;

	/**
	* @brief ������ �� � �������� � ����������� ������
	*/
	double lat_on;

	/**
	* @brief ������� �� � �������� � ����������� ������
	*/
	double lon_on;
};

/**
* @brief ��������� � ����������� ������ �� � ���
*/
struct API_BDFZ_SHARED_EXPORT Result_EFO:Result_IO
{
    Result_EFO();

	/**
	* @brief ����� �-500
	*/
	quint32 number_t; 

	/**
	* @brief ����� �-200
	*/
	quint32 number_k;

	/**
	* @brief  ��� ��
	*/
	QString type_obj;

	/**
	* @brief ������������ ��
	*/
	QString name;

	/**
	* @brief ������ ����������
	*/
	QString country;

	/**
	* @brief ��������������� ��������������
	*/
	QString nationality;

	/**
	* @brief ������������� �����������
	*/
	QString executer;

	/**
	* @brief ���� ����������
	*/
	QDate development_date;

	/**
	* @brief ���� ���������, �� �������� ���������� ��������
	*/
	QDate material_date;

	/**
	* @brief ������ �� � �������� � ����������� ������
	*/
	double lat_on;

	/**
	* @brief ������� �� � �������� � ����������� ������
	*/
	double lon_on;
};

/**
* @brief ��������� � ����������� ������ �� � ���
*/
struct API_BDFZ_SHARED_EXPORT Result_GMO:Result_IO
{
    Result_GMO();

    /**
    * @brief ���� ��������
    */
    QDate forecast_date;

    /**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;

    /**
    * @brief ��� ��������
    */
    QString forecast_type;
        
};

/**
* @brief ��������� � ����������� ������ �� � ������������-��������� ������������
*/
struct API_BDFZ_SHARED_EXPORT Result_NVO:Result_IO
{
    Result_NVO();

    /**
    * @brief ���� �������� ������
    */
    QDate date;    
};

/**
* @brief ������� ��������� � ������� ��� �������� ��
*/
struct API_BDFZ_SHARED_EXPORT Create_IO
{
    Create_IO();
    /**
    * @brief ��� ��
    */
    QString IOname;

    /**
    * @brief �������� ��
    */
    QString IODescription;

    /**
    * @brief ����������� �����������! ������� ����������� (0-����������, 2-��������, 3-���������� ��������) �� ��������� = 0
    */
    short constraints;

    /**
    * @brief ��� ����
    */
    QString code_skki;

    /**
    * @brief ��� �����
    */
    QString file_name;

    /**
    * @brief ���������� ������������� ���������, � ������������ � ������� ������ ������ ��������
    */
    QString base_doc;

    /**
    * @brief ���������� ����� ������� � ���������, � ������������ � ������� ������ ������ ��������
    */
    QString base_doc_task_number;

    /**
    * @brief ������������� �����������, ��������� �� (������������� �������� � ������� "��� � �������� ������������" �� listOrganizations)
    */
    QString uid_organization;

};

/**
* @brief ��������� � ������� ��� �������� ������� �������� ���
*/
struct API_BDFZ_SHARED_EXPORT Create_VTO:Create_IO
{
    Create_VTO();

    /**
    * @brief UID ���� ��� ���� �� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listVSTypes ������ ApiBdfz)
    */
    QString uid_vs_type;

    /**
    * @brief UID ����� ���������� ��� (���� ������ ������ ������ - �� ����������� � ������) (�� ����� listVTOUsageForms ������ ApiBdfz)
    */
    QString uid_vto_usage_form;

    /**
    * @brief �������� ����� ����� ��������
    */
	QString plan_number;

    /**
    * @brief �������� ����� �������� �����
    */
	QString variant_number;

    /**
    * @brief ����� ���
    */
	QString rbp_number;
};

/**
* @brief  ��������� � ������� ��� �������� �� ��� ��� ��
*/
struct API_BDFZ_SHARED_EXPORT Create_TTX:Create_IO
{
    Create_TTX();

    /**
    * @brief ���� ����������
    */
    QDate date;    

    /**
    * @brief ������������� ���� ������� ��� (������������� �������� � ������� "���� ���������� � ������� �������" �� listWeapons)
    */
    QString uid_weapon_type;  
};

/**
* @brief  ��������� � ������� ��� �������� �� �������� �� �� ���
*/
struct API_BDFZ_SHARED_EXPORT Create_Request:Create_IO
{
    Create_Request();

    /**
    * @brief ���� �������������
    */
    QDate date;    

    /**
    * @brief ����� ������
    */
    QString number;

    /**
    * @brief ��� ������ ("��" - 3.1.1, "���" - 3.1.2, "���" - 3.1.3, "���" - 3.1.4)
    */
    QString type;
};

/**
* @brief ��������� � ������� ��� �������� �� ������� ������� ���������� ��� 4
*/
struct API_BDFZ_SHARED_EXPORT Create_RBP:Create_IO
{
	Create_RBP();

    /**
    * @brief ����� ���
    */
	QString rbp_number;

	/**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;
};

/**
* @brief ��������� � ������� ��� �������� �� �������� �� �� �� �� �� 1
*/
struct API_BDFZ_SHARED_EXPORT Create_UGSH:Create_IO
{
    Create_UGSH();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ������� ����� ��������� � ����������������
    */
	QString outgoing_number;

	/**
    * @brief ������� ����� ��������� � ����������������
    */
	QString record_number;

};

/**
* @brief ��������� � ������� ��� �������� �� ������� �������� �������� 3
*/
struct API_BDFZ_SHARED_EXPORT Create_POR:Create_IO
{
    Create_POR();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ���������
    */
	QString category;

	/**
    * @brief ������������� �������� (������������� �������� � ������� "��� � �������� ������������" �� listOrganizations)
    */
    QString uid_addressee;
};


/**
* @brief ��������� � ������� ��� �������� �� ������������ �� ��� 2
*/
struct API_BDFZ_SHARED_EXPORT Create_PG:Create_IO
{
    Create_PG();

    /**
    * @brief ���� �������
    */
    QDate signature_date;

	/**
    * @brief ��������� ���������
    */
	QString category;

	/**
    * @brief ����� ���
    */
	QString rbp_number;

	/**
    * @brief ������������� �������� (������������� �������� � ������� "��� � �������� ������������" �� listOrganizations)
    */
    QString uid_addressee;
};

/**
* @brief ��������� � ������� ��� �������� �� ���� 44,46,47,48,49
*/
struct API_BDFZ_SHARED_EXPORT Create_ETK:Create_IO
{
    Create_ETK();

    /**
    * @brief ������������ �����
    */
    QString nomenclature;

    /**
    * @brief ���� �������� �����
    */
    QDate creation_date;

    /**
    * @brief ������������� ���� ��� (������������� �������� � ������� "���� ���" �� listEtkTypes)
    */
    QString uid_etk_type;

    /**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;

    /**
    * @brief ����������� ��������
    */
    long scale;

    /**
    * @brief �������� �����
    */
    QString sheet_name;

    /**
    * @brief �������������
    */
    QString classificator;
};

/**
* @brief ��������� � ������� ��� �������� �� �������� ������������� ����������
*/
struct API_BDFZ_SHARED_EXPORT Create_SKI:Create_IO
{
    Create_SKI();

     /**
    * @brief ���� ������ ������
    */
    QDate season_begin;

	/**
    * @brief ���� ��������� ������
    */
    QDate season_end;

    /**
    * @brief ����������� ������
    */
    double lat_min;

    /**
    * @brief ������������ ������
    */
    double lat_max;

    /**
    * @brief ����������� �������
    */
    double lon_min;

    /**
    * @brief ������������ �������
    */
    double lon_max;    
};

/**
* @brief ��������� � ������� ��� �������� �� ����� 33
*/
struct API_BDFZ_SHARED_EXPORT Create_EOIRD:Create_IO
{
    Create_EOIRD();

	/**
	* @brief ����� �-500
	*/
	quint32 number_t; 

	/**
	* @brief ����� �-200
	*/
	quint32 number_k;

	/**
	* @brief  ��� �� (������������� �������� � ������� "���� �������� ����������" �� listONTypes)
	*/
	QString uid_type_obj;

	/**
	* @brief ������������ ��
	*/
	QString name;

	/**
	* @brief ������ ���������� (������������� �������� � ������� "������ ����" �� listCountries)
	*/
	QString uid_country;

	/**
	* @brief ��������������� �������������� (������������� �������� � ������� "������ ����" �� listCountries)
	*/
	QString uid_nationality;

	/**
	* @brief ������������� �����������
	*/
	QString executer;

	/**
	* @brief ���� ����������
	*/
	QDate development_date;

	/**
	* @brief ���� ���������, �� �������� ���������� ��������
	*/
	QDate material_date;

	/**
	* @brief ������ B1 � �������� � ����������� ������
	*/
	double b1;

	/**
	* @brief ������� L1 � �������� � ����������� ������
	*/
	double l1;

	/**
	* @brief ������  B2 � �������� � ����������� ������ 
	*/
	double b2;

	/**
	* @brief ������� L2 � �������� � ����������� ������
	*/
	double l2;

	/**
	* @brief ������ �� � �������� � ����������� ������
	*/
	double lat_on;

	/**
	* @brief ������� �� � �������� � ����������� ������
	*/
	double lon_on;
};

/**
* @brief ��������� � ������� ��� �������� �� ��� 34
*/
struct API_BDFZ_SHARED_EXPORT Create_EFO:Create_EOIRD
{
};

/**
* @brief ��������� � ������� ��� �������� �� ��� - ���������������������� ����������� 53_55
*/
struct API_BDFZ_SHARED_EXPORT Create_GMO:Create_IO
{
    Create_GMO();

	/**
	* @brief  ���������������������� ����������� (������������� �������� � ������� "���� ��������� ������" �� listForecastTypes)
	*/
	QString uid_forecast_type;

	/**
	* @brief �������������� ����� (������������� �������� � ������� "�������������� ������" �� listRegions)
	*/
	QString uid_forecast_region;

	/**
	* @brief ���� ��������
	*/
	QDate forecast_date;
  
	/**
	* @brief ������ B1 � �������� � ����������� ������
	*/
	double b1;

	/**
	* @brief ������� L1 � �������� � ����������� ������
	*/
	double l1;

	/**
	* @brief ������  B2 � �������� � ����������� ������ 
	*/
	double b2;

	/**
	* @brief ������� L2 � �������� � ����������� ������
	*/
	double l2;
};

/**
* @brief ��������� � ������� ��� �������� �� ��� - ������������-��������� ����������� 56
*/
struct API_BDFZ_SHARED_EXPORT Create_NVO:Create_IO
{
    Create_NVO();

	/**
	* @brief ���� ��������
	*/
	QDate prognosis;
};

/**
* @brief ����� ��� ������ � API ��� (�� ���� ������������ ���������� ������ ��� �� ������������ �������� �������)
*/
class API_BDFZ_SHARED_EXPORT ApiBdfz : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timeout_interval READ getTimeout WRITE setTimeout)
public:
    /**
    * @brief ����������� ������ ��������� ������ Siu_api
    */
    explicit ApiBdfz(QObject *parent = 0);

    /**
    * @brief ���������� ������� ��������� ������ Siu_api
    */
    virtual ~ApiBdfz();

    /**
    * @brief ������������� ������� �������� ���������� �������� � �������������, �� ��������� = 3000
    * @param milliseconds ����� �������� �������� � �������������
    * @details ���� � ���, ��� ���������� siu �) �������� ����������, �) �� ���������� ������������� ����� �� �������������� (��������, � ������ ����� ������������� ������), ��� ��� ���������� ������������� �������
    */
    void setTimeout(int milliseconds);

    /**
    * @brief ���������� ������� �������� ���������� �������� � �������������
    * @return ����� �������� �������� � �������������
    */
    int getTimeout() const;

    /**
    * @brief ����������� ������������ �� �������
    * @return true � ������ ������ ��� false � ������ �������
    * @param host IP-����� �������
    * @param port ���� ������ ��� (8080)
    * @param username ��� ������������
    * @param password ������ ������������
    * @param id_constraint ������� ������� (3 � ��� ��������; 2 � ��������; 0 � ����������)
	* @param sop_name ��� ���, � ������� ��������� ��������
    */
    bool authorize(const QString& host, const QString& port, const QString& username, const QString& password,int id_constraint,const QString& sop_name);

    /**
    * @brief ��������� ������ ������������
    */
    bool logout();

	/**
    * @brief ����� ������� �������� ��� (�� ������ ���������� � �������� ��������� Param_VTO)
	* @param ��������� ������ (�� �������� ��������� Param_VTO)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_VTO)
    */
    QList<Result_VTO> searchIO(const Param_VTO& param);

	/**
    * @brief ����� ��� ��� �� (�� ������ ���������� � �������� ��������� Param_TTX)
	* @param ��������� ������ (�� �������� ��������� Param_TTX)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_TTX)
    */
    QList<Result_TTX> searchIO(const Param_TTX& param);

	/**
    * @brief ����� ������ �� �� ��� (�� ������ ���������� � �������� ��������� Param_Request)
	* @param ��������� ������ (�� �������� ��������� Param_Request)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_Request)
    */
    QList<Result_Request> searchIO(const Param_Request& param);

	/**
    * @brief ����� ������� ������� ���������� ��� (�� ������ ���������� � �������� ��������� Param_RBP)
	* @param ��������� ������ (�� �������� ��������� Param_RBP)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_RBP)
    */
    QList<Result_RBP> searchIO(const Param_RBP& param);

	/**
    * @brief ����� �������� �������� �������� (�� ������ ���������� � �������� ��������� Param_POR)
	* @param ��������� ������ (�� �������� ��������� Param_POR)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_POR)
    */
    QList<Result_POR> searchIO(const Param_POR& param);

	/**
    * @brief ����� ������������ �� ��� (�� ������ ���������� � �������� ��������� Param_PG)
	* @param ��������� ������ (�� �������� ��������� Param_PG)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_PG)
    */
    QList<Result_PG> searchIO(const Param_PG& param);
	
	/**
    * @brief ����� �������� ������������� ���������� (�� ������ ���������� � �������� ��������� Param_SKI)
	* @param ��������� ������ (�� �������� ��������� Param_SKI)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_SKI)
    */
    QList<Result_SKI> searchIO(const Param_SKI& param);

	/**
    * @brief ����� �������� �� �� �� �� �� (�� ������ ���������� � �������� ��������� Param_UGSH)
	* @param ��������� ������ (�� �������� ��������� Param_UGSH)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_UGSH)
    */
    QList<Result_UGSH> searchIO(const Param_UGSH& param);

    /**
    * @brief ����� ���� (�� ������ ���������� � �������� ��������� Param_ETK)
	* @param ��������� ������ (�� �������� ��������� Param_ETK)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_ETK)
    */
    QList<Result_ETK> searchIO(const Param_ETK& param);

	/**
    * @brief ����� ��� (�� ������ ���������� � �������� ��������� Param_GMO)
	* @param ��������� ������ (�� �������� ��������� Param_GMO)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_GMO)
    */
    QList<Result_GMO> searchIO(const Param_GMO& param);

	/**
    * @brief ����� ��� (�� ������ ���������� � �������� ��������� Param_NVO)
	* @param ��������� ������ (�� �������� ��������� Param_NVO)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_NVO)
    */
    QList<Result_NVO> searchIO(const Param_NVO& param);

	/**
    * @brief ����� ����� (�� ������ ���������� � �������� ��������� Param_EOIRD)
	* @param ��������� ������ (�� �������� ��������� Param_EOIRD)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_EOIRD)
    */
    QList<Result_EOIRD> searchIO(const Param_EOIRD& param);

	/**
    * @brief ����� ��� (�� ������ ���������� � �������� ��������� Param_EFO)
	* @param ��������� ������ (�� �������� ��������� Param_EFO)
	* @return ���������� ������ ����������� ������ (�� �������� ��������� Result_EFO)
    */
    QList<Result_EFO> searchIO(const Param_EFO& param);

	/**
    * @brief ������ �� ���� "��� ��� ��" �� ������� (�� ������ ���������� � �������� ��������� Create_TTX)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_TTX)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_TTX& param);

	/**
    * @brief ������ �� ���� "������� �������� ���" �� ������� (�� ������ ���������� � �������� ��������� Create_VTO)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_VTO)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_VTO& param);

	/**
    * @brief ������ �� ���� "������ �� �� ���" �� ������� (�� ������ ���������� � �������� ��������� Create_Request)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_Request)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_Request& param);

    /**
    * @brief ������ �� ���� "�������� ������������� ����������" �� ������� (�� ������ ���������� � �������� ��������� Create_SKI)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_SKI)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_SKI& param);

    /**
    * @brief ������ �� ���� "��� ���������" �� ������� (�� ������ ���������� � �������� ��������� Create_ETK)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_ETK)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_ETK& param);

	/**
    * @brief ������ �� ���� "������ ������� ���������� ���" �� ������� (�� ������ ���������� � �������� ��������� Create_RBP)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_RBP)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_RBP& param);

	/**
    * @brief ������ �� ���� "������� �������� ��������" �� ������� (�� ������ ���������� � �������� ��������� Create_POR)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_POR)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_POR& param);

	/**
    * @brief ������ �� ���� "����������� �� ���" �� ������� (�� ������ ���������� � �������� ��������� Create_PG)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_PG)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_PG& param);

	/**
    * @brief ������ �� ���� "�������� �� �� �� �� ��" �� ������� (�� ������ ���������� � �������� ��������� Create_UGSH)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_UGSH)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_UGSH& param);

	/**
    * @brief ������ �� ���� "���������������������� �����������" �� ������� (�� ������ ���������� � �������� ��������� Create_GMO)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_GMO)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_GMO& param);

	/**
    * @brief ������ �� ���� "�����" �� ������� (�� ������ ���������� � �������� ��������� Create_EOIRD)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_EOIRD)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_EOIRD& param);

	/**
    * @brief ������ �� ���� "���" �� ������� (�� ������ ���������� � �������� ��������� Create_EFO)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_EFO)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_EFO& param);

	/**
    * @brief ������ �� ���� "������������-��������� �����������" �� ������� (�� ������ ���������� � �������� ��������� Create_NVO)
	* @param param ��������� ������������ �� (�� �������� ��������� Create_NVO)
	* @return True � ������ ������ � false � ������ ������� (�������� ����������� false, ���� ������� �������� ����� ����� ����������)
    */
    bool createIO(const Create_NVO& param);

	/**
    * @brief ��������� ���� from_file_link �� �� io_uid � ��������� ���� to_file_path
	* @param io_uid ������������� ��, � �������� ����������� ����������� ���� (�� �������� ��������� Result_ETK)
	* @param from_file_link ������ �� ����, ������� ���������� ������� (�� �������� ��������� Result_ETK)
	* @param to_file_path ��������� ����, �� �������� ��������� ���� ����� �������
    */
    void downloadFile(const QString& io_uid, const QString& from_file_link, const QString& to_file_path);

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "���� ���" ("��� �����" ��� �� ���� "���-���������")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listEtkTypes();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "��� � �������� ������������" ("�����������" ��� �� ���� "���-���������")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listOrganizations();
	
	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "������ ����" ("������ ����������" � "������������ ��������������" ��� �� ���� "�����" � "���")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listCountries();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "���� �������� ����������" ("��� ��" ��� �� ���� "�����" � "���")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listONTypes();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "���� ��������� ������" ("���������������������� �����������" ��� �� ���� "���")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listForecastTypes();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "�������������� ������" ("�������������� �����" ��� �� ���� "���")
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listRegions();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "���� ���������� � ������� �������"
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listWeapons();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "����� ���������� ���"
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listVTOUsageForms();

	/**
	* @brief ���������� ������ ��� UID-�������� ��� ������� "���� � ���� ��"
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > listVSTypes();
	
signals:
    /**
    * @brief ������, ������������ ������� �������� �����
    * @param uid ������������� ��
    * @param filename ��� �����
    * @param done ������� ��������� � ������
    * @param full ������� ����� ���� ��������� � ������
    */
    void downloadFileProgress(QString uid, QString filename, qint64 done, qint64 full);

	/**
    * @brief ������, ����������� �� ������ �������� �����
    * @param file_path ���� � ������������ �����
    */
    void downloadError(QString file_path);

	/**
    * @brief ������, ����������� �� ��������� �������� �����
    * @param file_path ���� � ������������ �����
    */
    void downloadFinished(QString file_path);

    /**
    * @brief ������, ����������� �� ��������� �������� ����� (�� ��������)
    * @param file ��������� �� ����������� ����
    */
    void downloadFileFinished(QFile* file);
private slots:
	//void handleFinished(QString);
	//void handleError(QString);
    void handleDownloadFile(QFile*);
    void handleResponse(QDomElement *);
private:    
	/**
	* @brief ���������� ������ ��� ��������-UID ��� �������� ��������� �������
	* @param name �������� �������
	* @return ������ ��� ��������-UID
	*/
	QList<QPair<QString,QString> > getDictionary(const QString& name);

	/**
    * @brief ���������� ������ ���
    * @return ������ �������� � ������ ���� - ������������� ���, ������ - �������� ���
    */
    QList<QPair<QString,QString> > getNau();

	/**
    * @brief ���������� ������ ����� ��
    * @return ������ �������� � ������ ���� - ������������� ����, ������ - �������� ����
    */
    QList<QPair<QString,QString> > getIOTypes();

	/**
    * @brief ���������� ������������� ���� �� � �������� ������
    * @return ������������� ���� ��
	* @param type_name �������� ���� ��
    */
	QString getIOTypeUID(const QString& type_name);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �������� ������������� ����������
	*/
    QString formMetaString(const Param_SKI& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� ������� �������� ���
	*/
    QString formMetaString(const Param_VTO& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ��� ��� ��
	*/
    QString formMetaString(const Param_TTX& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ������ �� �� ���
	*/
    QString formMetaString(const Param_Request& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ������ ������� ���������� ���
	*/
    QString formMetaString(const Param_RBP& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ������� �������� ��������
	*/
    QString formMetaString(const Param_POR& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ����-��������
	*/
    QString formMetaString(const Param_PG& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� �������� �� �� �� �� ��
	*/
    QString formMetaString(const Param_UGSH& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ���
	*/
    QString formMetaString(const Param_ETK& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ���
	*/
    QString formMetaString(const Param_GMO& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ���
	*/
    QString formMetaString(const Param_NVO& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� �����
	*/
    QString formMetaString(const Param_EOIRD& param);

	/**
	* @brief ��������� ������ ���������� ��� ��������� ���������� ��� �� ���
	*/
    QString formMetaString(const Param_EFO& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� ������� �������� ���
	*/
    QString formMetaString(const Create_VTO& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� �������� ������������� ����������
	*/
    QString formMetaString(const Create_SKI& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ��� ��� ��
	*/
    QString formMetaString(const Create_TTX& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ������ �� �� ���
	*/
    QString formMetaString(const Create_Request& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ������ ������� ���������� ���
	*/
    QString formMetaString(const Create_RBP& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ������� �������� ��������
	*/
    QString formMetaString(const Create_POR& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ����-��������
	*/
    QString formMetaString(const Create_PG& param);
	
	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ���
	*/
    QString formMetaString(const Create_NVO& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� �������� �� �� �� �� ��
	*/
    QString formMetaString(const Create_UGSH& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ���
	*/
    QString formMetaString(const Create_GMO& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ���
	*/
    QString formMetaString(const Create_ETK& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� �����
	*/
    QString formMetaString(const Create_EOIRD& param);

	/**
	* @brief ��������� ������ ���������� ��� ���������� �������� �� ���
	*/
    QString formMetaString(const Create_EFO& param);
 
    /**
    * @brief ���������� �������� �������� ����
    * @return ������ �������� � ������ ���� - ������������� ��������, ������ - �������� ��������
    */
    QList<QPair<QString,QString> > getFirstFolders();

    /**
    * @brief ���������� ������ �������� ��������� ��� ��������� �������� ��������� ����
    * @return ������ �������� � ������ ���� - ������������� ���, ������ - �������� ���
    */
    QList<QPair<QString,QString> > getChildFolders(const QString& id_folder);

    /**
    * @brief ���������� ������������� �������� � �������� ����
    */
    QString getFolderIdForSKKI(const QString&);

    /**
    * @brief ��������� ����������� ���������� �� �������������� ��
    */
    QDomElement* getMetadatabyIOId(const QString& id_io);

	/**
	* @brief ���������� UID �������������� ��������� ����� �� ��� ����� � ����� �� ����
	*/
	QPair<QString,QString> getClassificatorParams(const QString& name);

    /**
    * @brief ��������� ����������� ���������� �� �������������� ���� ��
    */
    QDomElement* getMetadatabyTypeId(const QString& id_type);

    int resultCode(QDomElement* value);
    void setResult(QDomElement* value);    
    QString getTagValue(const QDomElement&,const QString& tag_name);
    QDomElement* getResult() const;
    Siu_api2 * siuapi;
    QDomElement* result;
    QFile * file;
    mutable QMutex mutex;
    int p_timeout_interval;
	QString id_nau;
};
