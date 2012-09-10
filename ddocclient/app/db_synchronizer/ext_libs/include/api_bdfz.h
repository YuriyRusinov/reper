#pragma once

/*! \mainpage Api Bdfz
 *
 * \author Семёнов А.В. отд. 45 ОАО "НИИ ТП"
 * \date 09.08.2012
 *
 * \section intro_sec Краткое описание
 *
 * Эта библиотека предназначена для доступа к данным типов ЭТК, ЭОИРД, ЭФО, НВО, ГМО.
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

#define VTO_TYPE tr("Выписки объектов ВТО")
#define TTX_TYPE tr("ТТХ средств ПВО противника")
#define REQUEST_TYPE tr("Заявки на ИО ВТО")
#define RBP_TYPE tr("Районы боевого применения ВТО")
#define POR_TYPE tr("Перечни объектов разведки для ВТО БД")
#define PG_TYPE tr("План–графики ИО ВТО")
#define ETK_TYPE tr("Электронные карты векторные")
#define UGSH_TYPE tr("Указания ГШ ВС РФ по ИО")
#define EOIRD_TYPE tr("ЭОИРД")
#define EFO_TYPE tr("ЭФО")
#define GMO_TYPE tr("Прогнозы погоды по РБП")
#define NVO_TYPE tr("Навигационно-временная информация")
#define SKI_TYPE tr("Сезонная климатическая информация")
#define CLASSIFICATOR_TYPE tr("Классификаторы векторных карт")

#define DICT_ETK_TYPES tr("Типы ЭТК")
#define DICT_ORGANIZATIONS tr("ОВУ и воинские формирования")
#define DICT_COUNTRIES tr("Страны мира")
#define DICT_ONTYPES tr("Типы объектов наблюдения")
#define DICT_FORECASTTYPES tr("Типы прогнозов погоды")
#define DICT_REGIONS tr("Географические районы")
#define DICT_WEAPONS tr("Типы вооружения и военной техники")
#define DICT_VS_TYPES tr("Виды и рода ВС")
#define DICT_VTO_USAGE_FORMS tr("Формы применения ВТО")

class Siu_api2;

/**
* @brief Структура базовых параметров запроса
*/
struct API_BDFZ_SHARED_EXPORT Param_IO
{
	explicit Param_IO();
    /**
    * @brief Дата обновления ИО, с которой надо искать (с точностью до минуты)
    */
    QDateTime updated_from;

    /**
    * @brief Дата обновления ИО, по которую надо искать (с точностью до минуты)
    */
    QDateTime updated_to;
};

/**
* @brief Структура параметров запроса Выписок объектов ВТО
*/
struct API_BDFZ_SHARED_EXPORT Param_VTO:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_VTO();

    /**
    * @brief UID вида или рода ВС (если задана пустая строка - не учитывается в поиске) (см метод listVSTypes класса ApiBdfz)
    */
    QString uid_vs_type;

    /**
    * @brief UID формы применения ВТО (если задана пустая строка - не учитывается в поиске) (см метод listVTOUsageForms класса ApiBdfz)
    */
    QString uid_vto_usage_form;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса ТТХ средств ПВО противника
*/
struct API_BDFZ_SHARED_EXPORT Param_TTX:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_TTX();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

	/**
    * @brief UID типа вооружения и техники (если задана пустая строка - не учитывается в поиске) (см метод listWeapons класса ApiBdfz)
    */
    QString uid_weapon_type;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса районов боевого применения ВТО
*/
struct API_BDFZ_SHARED_EXPORT Param_RBP:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_RBP();

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя геометрии искомых карт (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса Заявок на ИО ВТО
*/
struct API_BDFZ_SHARED_EXPORT Param_Request:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_Request();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Тип заявки ("РО" - 3.1.1, "ТГО" - 3.1.2, "ГМО" - 3.1.3, "НВО" - 3.1.4)
    */
    QString type;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса Перечней объектов разведки
*/
struct API_BDFZ_SHARED_EXPORT Param_POR:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_POR();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса План–графиков ИО ВТО
*/
struct API_BDFZ_SHARED_EXPORT Param_PG:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_PG();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса Указаний ГШ ВС РФ по ИО
*/
struct API_BDFZ_SHARED_EXPORT Param_UGSH:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_UGSH();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса Сезонной климатической информации
*/
struct API_BDFZ_SHARED_EXPORT Param_SKI:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_SKI();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя территорию прогноза (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса карт
*/
struct API_BDFZ_SHARED_EXPORT Param_ETK:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_ETK();

    /**
    * @brief Номенклатура (если строка пустая - в поиске не учитывается)
    */
    QString nomenclature;

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Знаменатель масштаба (1000000, 500000, 200000, 100000, 50000) (если равен 0 - в поиске не учитывается)
    */
    long scale;

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя геометрии искомых карт (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса ЭОИРД
*/
struct API_BDFZ_SHARED_EXPORT Param_EOIRD:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_EOIRD();

    /**
    * @brief Номер ОН Т-500 (если равен 0 - в поиске не учитывается)
    */
    int number_t500;

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя координаты объекта наблюдения (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

	/**
    * @brief UID типа объекта наблюдения (если задана пустая строка - не учитывается в поиске) (см метод listONTypes класса ApiBdfz)
    */
    QString uid_type_obj;

	/**
    * @brief UID страны государственной принадлежности объекта наблюдения (если задана пустая строка - не учитывается в поиске)  (см метод listCountries класса ApiBdfz)
    */
    QString uid_nationality;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса ЭФО
*/
struct API_BDFZ_SHARED_EXPORT Param_EFO:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_EFO();

    /**
    * @brief Номер ОН Т-500 (если равен 0 - в поиске не учитывается)
    */
    int number_t500;

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя координаты объекта наблюдения (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

	/**
    * @brief UID типа объекта наблюдения (если задана пустая строка - не учитывается в поиске)  (см метод listONTypes класса ApiBdfz)
    */
    QString uid_type_obj;

	/**
    * @brief UID страны государственной принадлежности объекта наблюдения (если задана пустая строка - не учитывается в поиске)  (см метод listCountries класса ApiBdfz)
    */
    QString uid_nationality;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};


/**
* @brief Структура параметров запроса гидрометеорологического обеспечения
*/
struct API_BDFZ_SHARED_EXPORT Param_GMO:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_GMO();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief WKT-строка, содержащая в себе описание полигона, который должен включать в себя геометрии искомых прогнозов (если строка пустая - в поиске не учитывается)
    */
    QString polygon;

	/**
    * @brief UID типа обеспечения (если задана пустая строка - не учитывается в поиске)  (см метод listForecastTypes класса ApiBdfz)
    */
    QString uid_forecast_type;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Структура параметров запроса навигационно-временного обеспечения
*/
struct API_BDFZ_SHARED_EXPORT Param_NVO:Param_IO
{
    /**
    * @brief Конструктор по умолчанию, заполняет все поля значениями, указывающими на неиспользование данного параметра в запросе
    */
    explicit Param_NVO();

    /**
    * @brief Дата, с которой надо искать
    */
    QDate date_from;

    /**
    * @brief Дата, по которую надо искать
    */
    QDate date_to;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно, 9-любой)
    */
    short constraints;
};

/**
* @brief Базовая структура с результатом поиска ИО
*/
struct API_BDFZ_SHARED_EXPORT Result_IO
{
    Result_IO();

    /**
    * @brief Дата и время обновления ИО (с точностью до минуты)
    */
    QDateTime updated;

	/**
    * @brief Наименование ИО
    */
    QString io_name;

    /**
    * @brief Уникальный идентификатор ИО
    */
    QString uid;

    /**
    * @brief Автор ИО
    */
    QString author;

    /**
    * @brief Имя файла
    */
    QString file_name;

    /**
    * @brief Ссылка на файл (нужна для скачаивания файла)
    */
    QString file_link;

    /**
    * @brief Уникальный идентификатор файла (?)
    */
    QString file_uid;

    /**
    * @brief Организация, создавшая информационный объект
    */
    QString organization;  

    /**
    * @brief Уровень секретности
    */
    int constraint;  
};

/**
* @brief Структура с результатом поиска Выписок объектов ВТО
*/
struct API_BDFZ_SHARED_EXPORT Result_VTO:Result_IO
{
    Result_VTO();

    /**
    * @brief Вид или род ВС
    */
    QString vs_type;

	/**
    * @brief Форма применения ВТО
    */
    QString vto_usage_form;

	/**
    * @brief Условный номер плана операции
    */
	QString plan_number;

	/**
    * @brief Условный номер варианта плана
    */
	QString variant_number;

	/**
    * @brief Номер РБП
    */
	QString rbp_number;
};

/**
* @brief Структура с результатом поиска ТТХ ПВО ИГ
*/
struct API_BDFZ_SHARED_EXPORT Result_TTX:Result_IO
{
    Result_TTX();

    /**
    * @brief Дата подготовки
    */
    QDate date;

	/**
    * @brief Тип образца ВВТ
    */
    QString weapon_type;

	/**
    * @brief Номер заявки
    */
	QString number;
};

/**
* @brief Структура с результатом поиска ИО с Заявками на ИО ВТО
*/
struct API_BDFZ_SHARED_EXPORT Result_Request:Result_IO
{
    Result_Request();

    /**
    * @brief Дата представления
    */
    QDate date;

    /**
    * @brief Номер заявки
    */
    QString number;

    /**
    * @brief Тип заявки ("РО" - 3.1.1, "ТГО" - 3.1.2, "ГМО" - 3.1.3, "НВО" - 3.1.4)
    */
    QString type;
};

/**
* @brief Структура с результатом поиска ИО с Указаниями ГШ ВС РФ по ИО
*/
struct API_BDFZ_SHARED_EXPORT Result_UGSH:Result_IO
{
    Result_UGSH();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Исходящий учетный номер документа в делопроизводстве
    */
	QString outgoing_number;

	/**
    * @brief Учетный номер документа в делопроизводстве
    */
	QString record_number;
};

/**
* @brief Структура с результатом поиска ИО с Перечнями объектов разведки
*/
struct API_BDFZ_SHARED_EXPORT Result_POR:Result_IO
{
    Result_POR();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Категория документа
    */
	QString category;	

	/**
    * @brief Адресат
    */
	QString addressee;
};

/**
* @brief Структура с результатом поиска ИО с Планами-графиками ИО ВТО
*/
struct API_BDFZ_SHARED_EXPORT Result_PG:Result_IO
{
    Result_PG();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Категория документа
    */
	QString category;

	/**
    * @brief Номер РБП
    */
	QString rbp_number;

	/**
    * @brief Адресат
    */
	QString addressee;
};

/**
* @brief Структура с результатом поиска ИО с Районами боевого применения ВТО
*/
struct API_BDFZ_SHARED_EXPORT Result_RBP:Result_IO
{
    Result_RBP();

    /**
    * @brief Номер РБП
    */
	QString rbp_number;

	/**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;
};

/**
* @brief Структура с результатом поиска ИО с Сезонной климатической информацией
*/
struct API_BDFZ_SHARED_EXPORT Result_SKI:Result_IO
{
    Result_SKI();

    /**
    * @brief Дата начала сезона
    */
    QDate season_begin;

	/**
    * @brief Дата окончания сезона
    */
    QDate season_end;

    /**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;    
};

/**
* @brief Структура с результатом поиска ИО с картами
*/
struct API_BDFZ_SHARED_EXPORT Result_ETK:Result_IO
{
    Result_ETK();

    /**
    * @brief Номенклатура карты
    */
    QString nomenclature;

    /**
    * @brief Дата создания карты
    */
    QDate creation_date;

    /**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;

    /**
    * @brief Знаменатель масштаба
    */
    long scale;

    /**
    * @brief Название листа
    */
    QString sheet_name;

    /**
    * @brief Имя файла классификатора
    */
    QString classificator_name;

	/**
    * @brief UID классификатора
    */
    QString classificator_uid;

	/**
    * @brief Ссылка на файл классификатора
    */
    QString classificator_link;
};

/**
* @brief Структура с результатом поиска ИО с ЭОИРД
*/
struct API_BDFZ_SHARED_EXPORT Result_EOIRD:Result_IO
{
    Result_EOIRD();

	/**
	* @brief Номер Т-500
	*/
	quint32 number_t; 

	/**
	* @brief Номер К-200
	*/
	quint32 number_k;

	/**
	* @brief  Тип ОН
	*/
	QString type_obj;

	/**
	* @brief Наименование ОН
	*/
	QString name;

	/**
	* @brief Страна дислокации
	*/
	QString country;

	/**
	* @brief Государственная принадлежность
	*/
	QString nationality;

	/**
	* @brief Ответственный исполнитель
	*/
	QString executer;

	/**
	* @brief Дата разработки
	*/
	QDate development_date;

	/**
	* @brief Дата материала, по которому изготовлен документ
	*/
	QDate material_date;

	/**
	* @brief Широта ОН в градусах с десятичными долями
	*/
	double lat_on;

	/**
	* @brief Долгота ОН в градусах с десятичными долями
	*/
	double lon_on;
};

/**
* @brief Структура с результатом поиска ИО с ЭФО
*/
struct API_BDFZ_SHARED_EXPORT Result_EFO:Result_IO
{
    Result_EFO();

	/**
	* @brief Номер Т-500
	*/
	quint32 number_t; 

	/**
	* @brief Номер К-200
	*/
	quint32 number_k;

	/**
	* @brief  Тип ОН
	*/
	QString type_obj;

	/**
	* @brief Наименование ОН
	*/
	QString name;

	/**
	* @brief Страна дислокации
	*/
	QString country;

	/**
	* @brief Государственная принадлежность
	*/
	QString nationality;

	/**
	* @brief Ответственный исполнитель
	*/
	QString executer;

	/**
	* @brief Дата разработки
	*/
	QDate development_date;

	/**
	* @brief Дата материала, по которому изготовлен документ
	*/
	QDate material_date;

	/**
	* @brief Широта ОН в градусах с десятичными долями
	*/
	double lat_on;

	/**
	* @brief Долгота ОН в градусах с десятичными долями
	*/
	double lon_on;
};

/**
* @brief Структура с результатом поиска ИО с ГМО
*/
struct API_BDFZ_SHARED_EXPORT Result_GMO:Result_IO
{
    Result_GMO();

    /**
    * @brief Дата прогноза
    */
    QDate forecast_date;

    /**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;

    /**
    * @brief Тип прогноза
    */
    QString forecast_type;
        
};

/**
* @brief Структура с результатом поиска ИО с Навигационно-временным обеспечением
*/
struct API_BDFZ_SHARED_EXPORT Result_NVO:Result_IO
{
    Result_NVO();

    /**
    * @brief Дата привязки данных
    */
    QDate date;    
};

/**
* @brief Базовая структура с данными для создания ИО
*/
struct API_BDFZ_SHARED_EXPORT Create_IO
{
    Create_IO();
    /**
    * @brief Имя ИО
    */
    QString IOname;

    /**
    * @brief Описание ИО
    */
    QString IODescription;

    /**
    * @brief Указывается обязательно! Уровень секретности (0-несекретно, 2-секретно, 3-совершенно секретно) По умолчанию = 0
    */
    short constraints;

    /**
    * @brief Код СККИ
    */
    QString code_skki;

    /**
    * @brief Имя файла
    */
    QString file_name;

    /**
    * @brief Уникальный идентификатор документа, в соответствии с которым создан данный документ
    */
    QString base_doc;

    /**
    * @brief Уникальный номер задания в документе, в соответствии с которым создан данный документ
    */
    QString base_doc_task_number;

    /**
    * @brief Идентификатор организации, создавшей ИО (идентификатор значения в словаре "ОВУ и воинские формирования" см listOrganizations)
    */
    QString uid_organization;

};

/**
* @brief Структура с данными для создания Выписок объектов ВТО
*/
struct API_BDFZ_SHARED_EXPORT Create_VTO:Create_IO
{
    Create_VTO();

    /**
    * @brief UID вида или рода ВС (если задана пустая строка - не учитывается в поиске) (см метод listVSTypes класса ApiBdfz)
    */
    QString uid_vs_type;

    /**
    * @brief UID формы применения ВТО (если задана пустая строка - не учитывается в поиске) (см метод listVTOUsageForms класса ApiBdfz)
    */
    QString uid_vto_usage_form;

    /**
    * @brief Условный номер плана операции
    */
	QString plan_number;

    /**
    * @brief Условный номер варианта плана
    */
	QString variant_number;

    /**
    * @brief Номер РБП
    */
	QString rbp_number;
};

/**
* @brief  Структура с данными для создания ИО ТТХ ПВО ИГ
*/
struct API_BDFZ_SHARED_EXPORT Create_TTX:Create_IO
{
    Create_TTX();

    /**
    * @brief Дата подготовки
    */
    QDate date;    

    /**
    * @brief Идентификатор типа образца ВВТ (идентификатор значения в словаре "Типы вооружения и военной техники" см listWeapons)
    */
    QString uid_weapon_type;  
};

/**
* @brief  Структура с данными для создания ИО Заявками на ИО ВТО
*/
struct API_BDFZ_SHARED_EXPORT Create_Request:Create_IO
{
    Create_Request();

    /**
    * @brief Дата представления
    */
    QDate date;    

    /**
    * @brief Номер заявки
    */
    QString number;

    /**
    * @brief Тип заявки ("РО" - 3.1.1, "ТГО" - 3.1.2, "ГМО" - 3.1.3, "НВО" - 3.1.4)
    */
    QString type;
};

/**
* @brief Структура с данными для создания ИО Районов боевого применения ВТО 4
*/
struct API_BDFZ_SHARED_EXPORT Create_RBP:Create_IO
{
	Create_RBP();

    /**
    * @brief Номер РБП
    */
	QString rbp_number;

	/**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;
};

/**
* @brief Структура с данными для создания ИО Указаний ГШ ВС РФ по ИО 1
*/
struct API_BDFZ_SHARED_EXPORT Create_UGSH:Create_IO
{
    Create_UGSH();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Исходящий учетный номер документа в делопроизводстве
    */
	QString outgoing_number;

	/**
    * @brief Учетный номер документа в делопроизводстве
    */
	QString record_number;

};

/**
* @brief Структура с данными для создания ИО Перечни объектов разведки 3
*/
struct API_BDFZ_SHARED_EXPORT Create_POR:Create_IO
{
    Create_POR();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Категория документа
    */
	QString category;

	/**
    * @brief Идентификатор адресата (идентификатор значения в словаре "ОВУ и воинские формирования" см listOrganizations)
    */
    QString uid_addressee;
};


/**
* @brief Структура с данными для создания ИО План–графиков ИО ВТО 2
*/
struct API_BDFZ_SHARED_EXPORT Create_PG:Create_IO
{
    Create_PG();

    /**
    * @brief Дата подписи
    */
    QDate signature_date;

	/**
    * @brief Категория документа
    */
	QString category;

	/**
    * @brief Номер РБП
    */
	QString rbp_number;

	/**
    * @brief Идентификатор адресата (идентификатор значения в словаре "ОВУ и воинские формирования" см listOrganizations)
    */
    QString uid_addressee;
};

/**
* @brief Структура с данными для создания ИО карт 44,46,47,48,49
*/
struct API_BDFZ_SHARED_EXPORT Create_ETK:Create_IO
{
    Create_ETK();

    /**
    * @brief Номенклатура карты
    */
    QString nomenclature;

    /**
    * @brief Дата создания карты
    */
    QDate creation_date;

    /**
    * @brief Идентификатор типа ЭТК (идентификатор значения в словаре "Типы ЭТК" см listEtkTypes)
    */
    QString uid_etk_type;

    /**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;

    /**
    * @brief Знаменатель масштаба
    */
    long scale;

    /**
    * @brief Название листа
    */
    QString sheet_name;

    /**
    * @brief Классификатор
    */
    QString classificator;
};

/**
* @brief Структура с данными для создания ИО Сезонной климатической информации
*/
struct API_BDFZ_SHARED_EXPORT Create_SKI:Create_IO
{
    Create_SKI();

     /**
    * @brief Дата начала сезона
    */
    QDate season_begin;

	/**
    * @brief Дата окончания сезона
    */
    QDate season_end;

    /**
    * @brief Минимальная широта
    */
    double lat_min;

    /**
    * @brief Максимальная широта
    */
    double lat_max;

    /**
    * @brief Минимальная долгота
    */
    double lon_min;

    /**
    * @brief Максимальная долгота
    */
    double lon_max;    
};

/**
* @brief Структура с данными для создания ИО ЭОИРД 33
*/
struct API_BDFZ_SHARED_EXPORT Create_EOIRD:Create_IO
{
    Create_EOIRD();

	/**
	* @brief Номер Т-500
	*/
	quint32 number_t; 

	/**
	* @brief Номер К-200
	*/
	quint32 number_k;

	/**
	* @brief  Тип ОН (идентификатор значения в словаре "Типы объектов наблюдения" см listONTypes)
	*/
	QString uid_type_obj;

	/**
	* @brief Наименование ОН
	*/
	QString name;

	/**
	* @brief Страна дислокации (идентификатор значения в словаре "Страны мира" см listCountries)
	*/
	QString uid_country;

	/**
	* @brief Государственная принадлежность (идентификатор значения в словаре "Страны мира" см listCountries)
	*/
	QString uid_nationality;

	/**
	* @brief Ответственный исполнитель
	*/
	QString executer;

	/**
	* @brief Дата разработки
	*/
	QDate development_date;

	/**
	* @brief Дата материала, по которому изготовлен документ
	*/
	QDate material_date;

	/**
	* @brief Широта B1 в градусах с десятичными долями
	*/
	double b1;

	/**
	* @brief Долгота L1 в градусах с десятичными долями
	*/
	double l1;

	/**
	* @brief Широта  B2 в градусах с десятичными долями 
	*/
	double b2;

	/**
	* @brief Долгота L2 в градусах с десятичными долями
	*/
	double l2;

	/**
	* @brief Широта ОН в градусах с десятичными долями
	*/
	double lat_on;

	/**
	* @brief Долгота ОН в градусах с десятичными долями
	*/
	double lon_on;
};

/**
* @brief Структура с данными для создания ИО ЭФО 34
*/
struct API_BDFZ_SHARED_EXPORT Create_EFO:Create_EOIRD
{
};

/**
* @brief Структура с данными для создания ИО ГМО - Гидрометеорологическое обеспечение 53_55
*/
struct API_BDFZ_SHARED_EXPORT Create_GMO:Create_IO
{
    Create_GMO();

	/**
	* @brief  Гидрометеорологическое обеспечение (идентификатор значения в словаре "Типы прогнозов погоды" см listForecastTypes)
	*/
	QString uid_forecast_type;

	/**
	* @brief Географический район (идентификатор значения в словаре "Географические районы" см listRegions)
	*/
	QString uid_forecast_region;

	/**
	* @brief Дата прогноза
	*/
	QDate forecast_date;
  
	/**
	* @brief Широта B1 в градусах с десятичными долями
	*/
	double b1;

	/**
	* @brief Долгота L1 в градусах с десятичными долями
	*/
	double l1;

	/**
	* @brief Широта  B2 в градусах с десятичными долями 
	*/
	double b2;

	/**
	* @brief Долгота L2 в градусах с десятичными долями
	*/
	double l2;
};

/**
* @brief Структура с данными для создания ИО НВО - Навигационно-временное обеспечение 56
*/
struct API_BDFZ_SHARED_EXPORT Create_NVO:Create_IO
{
    Create_NVO();

	/**
	* @brief Дата прогноза
	*/
	QDate prognosis;
};

/**
* @brief Класс для работы с API ИСТ (по сути осуществляет синхронную обёртку над их асинхронными методами доступа)
*/
class API_BDFZ_SHARED_EXPORT ApiBdfz : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timeout_interval READ getTimeout WRITE setTimeout)
public:
    /**
    * @brief Конструктор создаёт экземпляр класса Siu_api
    */
    explicit ApiBdfz(QObject *parent = 0);

    /**
    * @brief Деструктор удаляет экземпляр класса Siu_api
    */
    virtual ~ApiBdfz();

    /**
    * @brief Устанавливает таймаут ожидания выполнения запросов в миллисекундах, по умолчанию = 3000
    * @param milliseconds Время ожидания запросов в миллисекундах
    * @details Дело в том, что библиотека siu а) работает асинхронно, б) не возвращает отрицательный ответ на аутентификацию (например, в случае ввода неправильного пароля), так что приходится устанавливать таймаут
    */
    void setTimeout(int milliseconds);

    /**
    * @brief Возвращает таймаут ожидания выполнения запросов в миллисекундах
    * @return Время ожидания запросов в миллисекундах
    */
    int getTimeout() const;

    /**
    * @brief Авторизация пользователя на сервере
    * @return true в случае успеха или false в случае неудачи
    * @param host IP-адрес сервера
    * @param port Порт службы СИУ (8080)
    * @param username Имя пользователя
    * @param password Пароль пользователя
    * @param id_constraint Уровень доступа (3 – сов секретно; 2 – секретно; 0 – несекретно)
	* @param sop_name Имя СОП, с которым предстоит работать
    */
    bool authorize(const QString& host, const QString& port, const QString& username, const QString& password,int id_constraint,const QString& sop_name);

    /**
    * @brief Закрывает сессию пользователя
    */
    bool logout();

	/**
    * @brief Поиск Выписок объектов ВТО (См список параметров в описании структуры Param_VTO)
	* @param Параметры поиска (см описание структуры Param_VTO)
	* @return Возвращает список результатов поиска (см описание структуры Result_VTO)
    */
    QList<Result_VTO> searchIO(const Param_VTO& param);

	/**
    * @brief Поиск ТТХ ПВО ИГ (См список параметров в описании структуры Param_TTX)
	* @param Параметры поиска (см описание структуры Param_TTX)
	* @return Возвращает список результатов поиска (см описание структуры Result_TTX)
    */
    QList<Result_TTX> searchIO(const Param_TTX& param);

	/**
    * @brief Поиск Заявок на ИО ВТО (См список параметров в описании структуры Param_Request)
	* @param Параметры поиска (см описание структуры Param_Request)
	* @return Возвращает список результатов поиска (см описание структуры Result_Request)
    */
    QList<Result_Request> searchIO(const Param_Request& param);

	/**
    * @brief Поиск Районов боевого применения ВТО (См список параметров в описании структуры Param_RBP)
	* @param Параметры поиска (см описание структуры Param_RBP)
	* @return Возвращает список результатов поиска (см описание структуры Result_RBP)
    */
    QList<Result_RBP> searchIO(const Param_RBP& param);

	/**
    * @brief Поиск Перечней объектов разведки (См список параметров в описании структуры Param_POR)
	* @param Параметры поиска (см описание структуры Param_POR)
	* @return Возвращает список результатов поиска (см описание структуры Result_POR)
    */
    QList<Result_POR> searchIO(const Param_POR& param);

	/**
    * @brief Поиск План–графиков ИО ВТО (См список параметров в описании структуры Param_PG)
	* @param Параметры поиска (см описание структуры Param_PG)
	* @return Возвращает список результатов поиска (см описание структуры Result_PG)
    */
    QList<Result_PG> searchIO(const Param_PG& param);
	
	/**
    * @brief Поиск Сезонной климатической информации (См список параметров в описании структуры Param_SKI)
	* @param Параметры поиска (см описание структуры Param_SKI)
	* @return Возвращает список результатов поиска (см описание структуры Result_SKI)
    */
    QList<Result_SKI> searchIO(const Param_SKI& param);

	/**
    * @brief Поиск Указаний ГШ ВС РФ по ИО (См список параметров в описании структуры Param_UGSH)
	* @param Параметры поиска (см описание структуры Param_UGSH)
	* @return Возвращает список результатов поиска (см описание структуры Result_UGSH)
    */
    QList<Result_UGSH> searchIO(const Param_UGSH& param);

    /**
    * @brief Поиск карт (См список параметров в описании структуры Param_ETK)
	* @param Параметры поиска (см описание структуры Param_ETK)
	* @return Возвращает список результатов поиска (см описание структуры Result_ETK)
    */
    QList<Result_ETK> searchIO(const Param_ETK& param);

	/**
    * @brief Поиск ГМО (См список параметров в описании структуры Param_GMO)
	* @param Параметры поиска (см описание структуры Param_GMO)
	* @return Возвращает список результатов поиска (см описание структуры Result_GMO)
    */
    QList<Result_GMO> searchIO(const Param_GMO& param);

	/**
    * @brief Поиск НВО (См список параметров в описании структуры Param_NVO)
	* @param Параметры поиска (см описание структуры Param_NVO)
	* @return Возвращает список результатов поиска (см описание структуры Result_NVO)
    */
    QList<Result_NVO> searchIO(const Param_NVO& param);

	/**
    * @brief Поиск ЭОИРД (См список параметров в описании структуры Param_EOIRD)
	* @param Параметры поиска (см описание структуры Param_EOIRD)
	* @return Возвращает список результатов поиска (см описание структуры Result_EOIRD)
    */
    QList<Result_EOIRD> searchIO(const Param_EOIRD& param);

	/**
    * @brief Поиск ЭФО (См список параметров в описании структуры Param_EFO)
	* @param Параметры поиска (см описание структуры Param_EFO)
	* @return Возвращает список результатов поиска (см описание структуры Result_EFO)
    */
    QList<Result_EFO> searchIO(const Param_EFO& param);

	/**
    * @brief Создаёт ИО типа "ТТХ ПВО ИГ" на сервере (См список параметров в описании структуры Create_TTX)
	* @param param Параметры создаваемого ИО (см описание структуры Create_TTX)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_TTX& param);

	/**
    * @brief Создаёт ИО типа "Выписки объектов ВТО" на сервере (См список параметров в описании структуры Create_VTO)
	* @param param Параметры создаваемого ИО (см описание структуры Create_VTO)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_VTO& param);

	/**
    * @brief Создаёт ИО типа "Заявки на ИО ВТО" на сервере (См список параметров в описании структуры Create_Request)
	* @param param Параметры создаваемого ИО (см описание структуры Create_Request)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_Request& param);

    /**
    * @brief Создаёт ИО типа "Сезонная климатическая информация" на сервере (См список параметров в описании структуры Create_SKI)
	* @param param Параметры создаваемого ИО (см описание структуры Create_SKI)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_SKI& param);

    /**
    * @brief Создаёт ИО типа "ЭТК векторные" на сервере (См список параметров в описании структуры Create_ETK)
	* @param param Параметры создаваемого ИО (см описание структуры Create_ETK)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_ETK& param);

	/**
    * @brief Создаёт ИО типа "Районы боевого применения ВТО" на сервере (См список параметров в описании структуры Create_RBP)
	* @param param Параметры создаваемого ИО (см описание структуры Create_RBP)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_RBP& param);

	/**
    * @brief Создаёт ИО типа "Перечни объектов разведки" на сервере (См список параметров в описании структуры Create_POR)
	* @param param Параметры создаваемого ИО (см описание структуры Create_POR)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_POR& param);

	/**
    * @brief Создаёт ИО типа "План–графики ИО ВТО" на сервере (См список параметров в описании структуры Create_PG)
	* @param param Параметры создаваемого ИО (см описание структуры Create_PG)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_PG& param);

	/**
    * @brief Создаёт ИО типа "Указания ГШ ВС РФ по ИО" на сервере (См список параметров в описании структуры Create_UGSH)
	* @param param Параметры создаваемого ИО (см описание структуры Create_UGSH)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_UGSH& param);

	/**
    * @brief Создаёт ИО типа "Гидрометеорологическое обеспечение" на сервере (См список параметров в описании структуры Create_GMO)
	* @param param Параметры создаваемого ИО (см описание структуры Create_GMO)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_GMO& param);

	/**
    * @brief Создаёт ИО типа "ЭОИРД" на сервере (См список параметров в описании структуры Create_EOIRD)
	* @param param Параметры создаваемого ИО (см описание структуры Create_EOIRD)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_EOIRD& param);

	/**
    * @brief Создаёт ИО типа "ЭФО" на сервере (См список параметров в описании структуры Create_EFO)
	* @param param Параметры создаваемого ИО (см описание структуры Create_EFO)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_EFO& param);

	/**
    * @brief Создаёт ИО типа "Навигационно-временное обеспечение" на сервере (См список параметров в описании структуры Create_NVO)
	* @param param Параметры создаваемого ИО (см описание структуры Create_NVO)
	* @return True в случае успеха и false в случае неудачи (возможно возвращение false, если процесс загрузки файла будет длительным)
    */
    bool createIO(const Create_NVO& param);

	/**
    * @brief Скачивает файл from_file_link из ИО io_uid в локальный файл to_file_path
	* @param io_uid Идентификатор ИО, к которому принадлежит скачиваемый файл (см описание структуры Result_ETK)
	* @param from_file_link Ссылка на файл, который необходимо скачать (см описание структуры Result_ETK)
	* @param to_file_path Локальный путь, по которому скачанный файл будет сохранён
    */
    void downloadFile(const QString& io_uid, const QString& from_file_link, const QString& to_file_path);

	/**
	* @brief Возвращает список пар UID-название для словаря "Типы ЭТК" ("Тип карты" для ИО типа "ЭТК-векторные")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listEtkTypes();

	/**
	* @brief Возвращает список пар UID-название для словаря "ОВУ и воинские формирования" ("Организация" для ИО типа "ЭТК-векторные")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listOrganizations();
	
	/**
	* @brief Возвращает список пар UID-название для словаря "Страны мира" ("Страна дислокации" и "Национальная принадлежность" для ИО типа "ЭОИРД" и "ЭФО")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listCountries();

	/**
	* @brief Возвращает список пар UID-название для словаря "Типы объектов наблюдения" ("Тип ОН" для ИО типа "ЭОИРД" и "ЭФО")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listONTypes();

	/**
	* @brief Возвращает список пар UID-название для словаря "Типы прогнозов погоды" ("Гидрометеорологическое обеспечение" для ИО типа "ГМО")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listForecastTypes();

	/**
	* @brief Возвращает список пар UID-название для словаря "Географические районы" ("Географический район" для ИО типа "ГМО")
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listRegions();

	/**
	* @brief Возвращает список пар UID-название для словаря "Типы вооружения и военной техники"
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listWeapons();

	/**
	* @brief Возвращает список пар UID-название для словаря "Формы применения ВТО"
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listVTOUsageForms();

	/**
	* @brief Возвращает список пар UID-название для словаря "Виды и рода ВС"
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > listVSTypes();
	
signals:
    /**
    * @brief Сигнал, отображающий процесс загрузки файла
    * @param uid Идентификатор ИО
    * @param filename Имя файла
    * @param done Сколько загружено в байтах
    * @param full Сколько всего надо загрузить в байтах
    */
    void downloadFileProgress(QString uid, QString filename, qint64 done, qint64 full);

	/**
    * @brief Сигнал, оповещающий об ошибке загрузки файла
    * @param file_path Путь к загружаемому файлу
    */
    void downloadError(QString file_path);

	/**
    * @brief Сигнал, оповещающий об окончании загрузки файла
    * @param file_path Путь к загруженному файлу
    */
    void downloadFinished(QString file_path);

    /**
    * @brief Сигнал, оповещающий об окончании загрузки файла (не работает)
    * @param file Указатель на загруженный файл
    */
    void downloadFileFinished(QFile* file);
private slots:
	//void handleFinished(QString);
	//void handleError(QString);
    void handleDownloadFile(QFile*);
    void handleResponse(QDomElement *);
private:    
	/**
	* @brief Возвращает список пар название-UID для значений заданного словаря
	* @param name Название словаря
	* @return Список пар название-UID
	*/
	QList<QPair<QString,QString> > getDictionary(const QString& name);

	/**
    * @brief Возвращает список СОП
    * @return Первый параметр в каждой паре - идентификатор СОП, второй - название СОП
    */
    QList<QPair<QString,QString> > getNau();

	/**
    * @brief Возвращает список типов ИО
    * @return Первый параметр в каждой паре - идентификатор типа, второй - название типа
    */
    QList<QPair<QString,QString> > getIOTypes();

	/**
    * @brief Возвращает идентификатор типа ИО с заданным именем
    * @return Идентификатор типа ИО
	* @param type_name Название типа ИО
    */
	QString getIOTypeUID(const QString& type_name);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для Сезонной климатической информации
	*/
    QString formMetaString(const Param_SKI& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для Выписок объектов ВТО
	*/
    QString formMetaString(const Param_VTO& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО ТТХ ПВО ИГ
	*/
    QString formMetaString(const Param_TTX& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО Заявки на ИО ВТО
	*/
    QString formMetaString(const Param_Request& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО Районы боевого применения ВТО
	*/
    QString formMetaString(const Param_RBP& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО Перечни объектов разведки
	*/
    QString formMetaString(const Param_POR& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО План-графиков
	*/
    QString formMetaString(const Param_PG& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО Указания ГШ ВС РФ по ИО
	*/
    QString formMetaString(const Param_UGSH& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО ЭТК
	*/
    QString formMetaString(const Param_ETK& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО ГМО
	*/
    QString formMetaString(const Param_GMO& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО НВО
	*/
    QString formMetaString(const Param_NVO& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО ЭОИРД
	*/
    QString formMetaString(const Param_EOIRD& param);

	/**
	* @brief Формирует строку метаданных для поисковых параметров для ИО ЭФО
	*/
    QString formMetaString(const Param_EFO& param);

	/**
	* @brief Формирует строку метаданных для параметров создания Выписок объектов ВТО
	*/
    QString formMetaString(const Create_VTO& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО Сезонной климатической информации
	*/
    QString formMetaString(const Create_SKI& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО ТТХ ПВО ИГ
	*/
    QString formMetaString(const Create_TTX& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО Заявки на ИО ВТО
	*/
    QString formMetaString(const Create_Request& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО Районы боевого применения ВТО
	*/
    QString formMetaString(const Create_RBP& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО Перечни объектов разведки
	*/
    QString formMetaString(const Create_POR& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО План-графиков
	*/
    QString formMetaString(const Create_PG& param);
	
	/**
	* @brief Формирует строку метаданных для параметров создания ИО НВО
	*/
    QString formMetaString(const Create_NVO& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО Указания ГШ ВС РФ по ИО
	*/
    QString formMetaString(const Create_UGSH& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО ГМО
	*/
    QString formMetaString(const Create_GMO& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО ЭТК
	*/
    QString formMetaString(const Create_ETK& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО ЭОИРД
	*/
    QString formMetaString(const Create_EOIRD& param);

	/**
	* @brief Формирует строку метаданных для параметров создания ИО ЭФО
	*/
    QString formMetaString(const Create_EFO& param);
 
    /**
    * @brief Возвращает корневые каталоги СОПа
    * @return Первый параметр в каждой паре - идентификатор каталога, второй - название каталога
    */
    QList<QPair<QString,QString> > getFirstFolders();

    /**
    * @brief Возвращает список дочерних каталогов для заданного каталога заданного СОПа
    * @return Первый параметр в каждой паре - идентификатор СОП, второй - название СОП
    */
    QList<QPair<QString,QString> > getChildFolders(const QString& id_folder);

    /**
    * @brief Возвращает идентификатор каталога с заданным СККИ
    */
    QString getFolderIdForSKKI(const QString&);

    /**
    * @brief Получение расширенных метаданных по идентификатору ИО
    */
    QDomElement* getMetadatabyIOId(const QString& id_io);

	/**
	* @brief Возвращает UID классификатора векторной карты по его имени и ссылу на файл
	*/
	QPair<QString,QString> getClassificatorParams(const QString& name);

    /**
    * @brief Получение расширенных метаданных по идентификатору типа ИО
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
