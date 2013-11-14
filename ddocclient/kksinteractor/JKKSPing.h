/* 
 * File:   JKKSPing.h
 * Author: ksa
 *
 * Created on 29.10.2013 г., 12:17
 */

#if !defined (__KKSSITOOOM_JKKSPING_H)
#define	__KKSSITOOOM_JKKSPING_H

#include "kksinteractor_config.h"
#include "JKKSMessage.h"

class _I_EXPORT JKKSPing : public JKKSMessage
{
public:
    JKKSPing (qint64 id=-1, 
              const JKKSAddress & addr=JKKSAddress(), 
              const QString & kvs=QString(),
              bool isResponse = false);
    JKKSPing (const JKKSPing & ping);
    ~JKKSPing (void);

    qint64 id (void) const;
    void setId (qint64 _id);

    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);

    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);

    JKKSMessageType getMessageType (void) const;

    qint64 idOrgTo() const;
    void setIdOrgTo(qint64 id);

    qint64 idOrgFrom() const;
    void setIdOrgFrom(qint64 id);

    const QString & versionFrom() const;
    const QString & versionTo() const;
    void setVersionFrom(const QString & v);
    void setVersionTo(const QString & v);

    const QString & uidFrom() const;
    const QString & uidTo() const;
    void setUidFrom(const QString & u);
    void setUidTo(const QString & u);

    const QString & nameFrom() const;
    const QString & nameTo() const;
    void setNameFrom(const QString & n);
    void setNameTo(const QString & n);

    int state1() const;
    int state2() const;
    int state3() const;
    int state4() const;

    void setState1(int s);
    void setState2(int s);
    void setState3(int s);
    void setState4(int s);

    int created() const;
    bool completed() const;
    void setCreated(int c);
    void setCompleted(bool c);

    //void setSenderAddress(const JKKSAddress & addr);
    //const JKKSAddress & senderAddress() const;

    bool operator == (const JKKSPing & in) const;
    JKKSPing & operator = ( const JKKSPing & in );

private:
    //
    // Variables
    //
    qint64 m_id; //id самого пинга. Используется для внутренних целей
    qint64 m_idOrgTo; //id целевой организации, к которой направляется пинг
    qint64 m_idOrgFrom;//id организации, которая отправила пинг

    QString m_uidFrom;//уникальный идентификатор организации, которая отправила пинг
    QString m_uidTo;//уникальный идентификатор организации, к которой направляется пинг

    QString m_nameFrom;//название организации, которая отправила пинг
    QString m_nameTo;//название организации, к которой направляется пинг

    QString m_versionFrom;//версия БД DynamicDocs организации, которая отправила пинг
    QString m_versionTo; //версия БД DynamicDocs организации, к которой направляется пинг

    //состояния целевой организации
    //0 - неактивно (или состояние проверить не удалось)
    //1 - активно
    //2 - не используется (для m_state2)
    int m_state1; //Активен ли http_connector, который с ней работает
    int m_state2; //Активна ли среда ТПС, если взаимодействие идет через шлюз ТПС. Также означает, смогла ли среда ТПС доставить данные до целевого http_connector'а
    int m_state3; //Активна ли БД DynamicDocs целевой организации
    int m_state4; //Соответствует ли версия БД DynamicDocs целевой органиации версии БД, которая отправила пинг

    //Данное поле задает условие (и результат) проверки связи с данной организацией.
    //Возможные значения:
    //0 - ожидается проверка связи (значение по умолчанию)
    //1 - проверка связи завершена успешно
    //2 - проверка связи завершилась с ошибкой
    //Работать можно только с организациями, которые имеют 1 в качестве значения данного поля
    int m_created;

    //Флаг, который определяет, что создание в справочнике новой организации завершено. 
    //При смене значения данного флага с FALSE на TRUE запускается механизм проверки связи с введенной организацией.
    //Флаг меняется при задании для данной организации хотя бы одного транспорта и назначения адреса (в справочнике organization_transport)
    bool m_completed;

    bool m_isResponse;//если true - то является ответом на пинг (в этом случае ряд атрибутов будет не заполнен
    //JKKSAddress m_senderAddress;//адрес отправителя пинга. Будет записываться на приемном конце в таблицу queue_results
};


#endif	/* __KKSSITOOOM_JKKSPING_H */

