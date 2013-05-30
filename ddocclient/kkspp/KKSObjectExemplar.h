/***********************************************************************
 * Module:  KKSObjectExemplar.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:00
 * Purpose: Declaration of the class KKSObjectExemplar
 * Comment: ЭИО
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSObjectExemplar_h)
#define __KKSSITOOOM_KKSObjectExemplar_h

class KKSObject;
class KKSAttrValue;
class KKSCategoryAttr;
class KKSRubric;

#include <KKSRecord.h>
#include "kkspp_config.h"
#include "KKSList.h"
#include "KKSFile.h"

class _PP_EXPORT KKSObjectExemplar : public KKSRecord
{
public:
    KKSObjectExemplar();
    KKSObjectExemplar(const KKSObjectExemplar & eio);
    KKSObjectExemplar(qint64 id, const QString & name, KKSObject * io);
    virtual ~KKSObjectExemplar();

    //методы для работы с атрибутами записей справочников (колонками таблиц)
    const KKSList<KKSAttrValue *> & attrValues() const;
    KKSList<KKSAttrValue *> & attrValues();
    void setAttrValues(const KKSList<KKSAttrValue*> & _attrValues);

    KKSAttrValue * attrValueIndex(int index);
    const KKSAttrValue * attrValueIndex(int index) const;
    KKSAttrValue * attrValue(int id);
    const KKSAttrValue * attrValue(int id) const;

    void addAttrValue(KKSAttrValue * av);
    void addAttrValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue = NULL);
    void removeAttrValue(KKSAttrValue * av);
    void removeAttrValue(int index);

    //методы для работы с показателями записей справочников
    const KKSList<KKSAttrValue *> & indValues() const;
    KKSList<KKSAttrValue *> & indValues();
    void setIndValues(const KKSList<KKSAttrValue*> & _indValues);

    KKSAttrValue * indValueIndex(int index);
    const KKSAttrValue * indValueIndex(int index) const;
    KKSAttrValue * indValue(int id);
    const KKSAttrValue * indValue(int id) const;

    void addIndValue(KKSAttrValue * av);
    void addIndValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue = NULL);
    void removeIndValue(KKSAttrValue * av);
    void removeIndValue(int index);


    KKSObject * io() const;
    void setIo(KKSObject * _io);

    //Для экземпляров информационных объектов (т.е. записей в таблицах БД)
    //Параметры name, id, code, desc присутствуют в списке атрибутов, 
    //поскольку этот список атрибутов описывает запись в таблице БД.
    //В связи с этим чтобы избежать рассогласования в этих четырех параметрах
    //и значениях в наборе атрибутов, необходимо переопределить нижеследующие методы,
    //чтобы они также устанавливали и новое значение в соответствующие данным параметрам атрибуты.
    //Если соответствующего атрибута нет - надо генерировать предупреждение, 
    //что задается параметр, которого нет в списке атрибутов
    void setName(const QString & newName);
    void setId(qint64 newId);
    void setDesc(const QString & newDesc);
    void setCode(const QString & newCode);

    //данные методы используются для задания и получения корневой рубрики, 
    //которая содержит вложения данной записи
    //корневая рубрика у ЭИО может быть только одна или не быть вообще.
    void setRootRubric(KKSRubric * r);
    KKSRubric * rootRubric() const;

    const KKSList<KKSFile *> & files() const;
    KKSList<KKSFile *> & files();
    void setFiles(const KKSList<KKSFile *> & _files);
    void addFile(KKSFile * f);

    //удаляет из списка файлов указанный файл
    int removeFile(KKSFile * f);
    //удаляет из списка файлов файл с указанным порядковым индексом
    int removeFile(int index);

    KKSFile * file(int index);
    const KKSFile * file(int index) const;



protected:
private:
    friend class KKSEIOFactory;

    KKSObject* m_io;
    KKSList<KKSAttrValue*> m_attrValues;
    KKSList<KKSAttrValue*> m_indValues;//показатели записей справочников
    
    KKSList<KKSFile *> m_files;
    bool m_filesModified;

    KKSRubric * m_rootRubric;

};

#endif
