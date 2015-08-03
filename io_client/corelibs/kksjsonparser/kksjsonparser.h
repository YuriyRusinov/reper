#if !defined(__kksjson_parser_h)
#define __kksjson_parser_h

#include "config_kksjsonparser.h"
    

class __EXPORT_KKSJSONPARSER JSONFeature
{
public:
    JSONFeature();
    ~JSONFeature();

    void setUid(const char * s);
    void setName(const char * s);
    void setStyleString(const char * s);
    void setStyleId(const char * s);
    void setGeom(const char * s); //deep copy
    void setGeomDirectly(const char * s);//set only pointer and take ownership
    void setGeomType(int t);

    const char * uid() const;
    const char * name() const;
    const char * styleString() const;
    const char * styleId() const;
    const char * geom() const;
    int geomType() const;

    const char * sql() const;
    char * generateSQL() const;
private:

    friend class KKSJSONParser;

    char * m_uid;
    char * m_name;
    char * m_styleString;
    char * m_styleId;
    char * m_geom;

    //mutable char * m_uid_sql;
    //mutable char * m_name_sql;
    //mutable char * m_styleString_sql;
    //mutable char * m_styleId_sql;
    //mutable char * m_geom_sql;

    int m_geomType; //1 - point, 2 - linestring, 3 - polygon
    char * m_sql;


    //const char * uid_sql() const;
    //const char * name_sql() const;
    //const char * styleString_sql() const;
    //const char * styleId_sql() const;
    //const char * geom_sql() const;

    
    //void generateSQL();
};

class __EXPORT_KKSJSONPARSER KKSJSONParser
{
public:
    KKSJSONParser();
    ~KKSJSONParser();

    void setJSONString(const char * str);

    int parse(bool bGenerateSQL = true);

    const char * getErrorString();
    int getErrorOffset();

    int getFeaturesCount() const;
    JSONFeature * getFeature(int index) const;

private:

    void setErrorString(const char * str);
    void generateSQL(JSONFeature * f);


    JSONFeature ** m_features;
    unsigned int m_featureCount;

    char * m_jsonString;

    char * m_errorString;
    int m_errorOffset;

    bool m_dirty; //if false - parsing is need!
};

#endif
