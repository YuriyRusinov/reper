#include "kksjsonparser.h"

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

#include <stdio.h>
#include <stdlib.h>
    
using namespace rapidjson;


KKSJSONParser::KKSJSONParser()
{
    m_featureCount = 0;
    m_features = NULL;

    m_jsonString = NULL;

    m_errorOffset = -1;
    m_errorString = NULL;

    m_dirty = true;
}

KKSJSONParser::~KKSJSONParser()
{
    if(m_features){
        for(unsigned int i=0; i<m_featureCount; i++)
            delete m_features[i];
        delete m_features;
    }

    m_featureCount = 0;

    if(m_jsonString)
        delete[] m_jsonString;

    if(m_errorString)
        delete[] m_errorString;
}

void KKSJSONParser::setJSONString(const char * str)
{
    if(m_jsonString)
        delete[] m_jsonString;
    m_jsonString = NULL;

    if(str){
        m_jsonString = new char[strlen(str)+1];
        strcpy(m_jsonString, str);
    }

    if(m_features){
        for(unsigned int i=0; i<m_featureCount; i++)
            delete m_features[i];
        delete m_features;
    }
    m_features = NULL;

    m_featureCount = 0;

    m_errorOffset = -1;
    if(m_errorString)
        delete[] m_errorString;
    m_errorString = NULL;

    //need to parse!
    m_dirty = true;
}

int KKSJSONParser::parse(bool bGenerateSQL)
{
    m_dirty = true;
    setErrorString(NULL);
    m_errorOffset = -1;

    if(!m_jsonString){
        setErrorString("Empty input data!");
        return -1;
    }

    Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
    
	// In-situ parsing, decode strings directly in the source string. Source must be string.
	int length = strlen(m_jsonString);
    //char * tBuffer = new char [length+1];
	//memcpy(tBuffer, m_jsonString, length+1);

    char * outBuffer = new char[length+1]; //size of m_jsonString
    
    
    if (document.Parse<0>(m_jsonString).HasParseError()){
        setErrorString(document.GetParseError());
        m_errorOffset = document.GetErrorOffset();
        return -1;
    }

    if(!document.IsObject()){
        setErrorString("JSON root element is not object!");
        return -1;
    }

    if(!document.HasMember("type")){
        setErrorString("JSON root element has no element \"type\"!");
        return -1;
    }

    const char * str = document["type"].GetString();
    if(strcmp(str, "FeatureCollection") != 0){
        setErrorString("JSON root element \"type\" has value not equal to \"FeatureCollection\"!");
        return -1;
    }

    if(!document["features"].IsArray()){
        setErrorString("JSON element \"features\" has type not equal to array!");
        return -1;
    }

    m_featureCount = (int)document["features"].Size();
    if(m_featureCount <= 0){
        setErrorString("Features count = 0!");
        return -2;
    }

    m_features = new JSONFeature *[m_featureCount];
    
    for(unsigned int i=0; i<m_featureCount; i++){
        JSONFeature * feature = new JSONFeature();
        
        const Value & dFeature = document["features"][i];
        if(!dFeature.IsObject()){
            setErrorString("feature element is not json object!");
            return -1;
        }

        if(!dFeature.HasMember("type")){
            setErrorString("Feature structure is inconsistent! \"type\" element is missing!");
            return -1;
        }

        const char * str = dFeature["type"].GetString();
        if(strcmp(str, "Feature") != 0){ //value if element "type" should be "Feature"
            setErrorString("Value of feature element \"type\" should be \"Feature\"!");
            return -1;
        }

       
        //
        //parsing geometry
        //
        
        if(!dFeature.HasMember("geometry")){
            setErrorString("Feature structure is inconsistent! \"geometry\" element is missing!");
            return -1;
        }

        const Value & fGeometry = dFeature["geometry"];
        if(!fGeometry.IsObject()){
            setErrorString("\"geometry\" element is not json object!");
            return -1;
        }

        if(!fGeometry.HasMember("type")){
            setErrorString("Geometry structure is inconsistent! \"type\" element is missing!");
            return -1;
        }
        const char * geomTypeStr = fGeometry["type"].GetString();
        
        if(!fGeometry.HasMember("coordinates")){
            setErrorString("Geometry structure is inconsistent! \"coordinates\" element is missing!");
            return -1;
        }
        const Value & coords = fGeometry["coordinates"];
        if(!coords.IsArray()){
            setErrorString("\"coordinates\" element is not json array!");
            return -1;
        }

        //char * outBuffer = new char[length]; //size of m_jsonString
        strcpy(outBuffer, "\0");
        InsituStringStream stream(outBuffer);
        Writer<InsituStringStream> writer(stream);
        stream.PutBegin();
        writer.StartArray();

        //Point
        if(!strcmp(geomTypeStr, "Point")){
            
            feature->setGeomType(1);
            
            SizeType coordsCount = coords.Size(); //always should be 2 or 3 for points
            if(coordsCount < 2 || coordsCount > 3){
                setErrorString("Geometry structure is inconsistent! \"Point\" geometries should contain 2 or 3 points!");
                return -1;
            }
            
            for(unsigned int iCoord=0; iCoord<coordsCount; iCoord++){
                const Value & coord = coords[iCoord];
                if(!coord.IsDouble() && !coord.IsInt()){
                    return 1;
                }

                writer.Double(coord.GetDouble());
            }

        }//end of point

        //LineString
        if(!strcmp(geomTypeStr, "LineString")){
            
            feature->setGeomType(2);

            SizeType pointsCount = coords.Size(); //count of points in linestring
            //if(linesCount != 1){
            //    setErrorString("Geometry structure is inconsistent! \"LineString\" geometries should contain only 1 line string!");
            //    return -1;
            //}

            for(unsigned int iPoint=0; iPoint<pointsCount; iPoint++){
                const Value & point = coords[iPoint];
                if(!point.IsArray()){
                    setErrorString("Geometry structure is inconsistent! \"LineString\" geometries should be arrays!");
                    return -1;
                }

                writer.StartArray();
                
                SizeType coordsCount = point.Size(); //always should be 2 or 3 
                for(unsigned int iCoord=0; iCoord<coordsCount; iCoord++){
                    const Value & coord = point[iCoord];
                    if(!coord.IsDouble() && !coord.IsInt()){
                        setErrorString("coords should be of type Double or Integer!");
                        return -1;
                    }

                    writer.Double(coord.GetDouble());
                }

                writer.EndArray();
            }

        } //end of linestring


        //Polygon
        if(!strcmp(geomTypeStr, "Polygon")){
            
            feature->setGeomType(3);
            
            SizeType ringsCount = coords.Size(); 
            for(unsigned int iRing=0; iRing<ringsCount; iRing++){
                const Value & ring = coords[iRing];
                if(!ring.IsArray()){
                    setErrorString("Geometry structure is inconsistent! \"Polygon\" geometries should be arrays!");
                    return -1;
                }
                
                writer.StartArray();
                
                SizeType pointsCount = ring.Size();
                for(unsigned int iPoint=0; iPoint<pointsCount; iPoint++){
                    const Value & point = ring[iPoint];
                    if(!point.IsArray()){
                        setErrorString("Geometry structure is inconsistent! \"Polygon\" geometries should be arrays!");
                        return -1;
                    }

                    writer.StartArray();
                    
                    SizeType coordCount = point.Size(); //always should be 2 or 3 for polygons
                    for(unsigned int iCoord=0; iCoord<coordCount; iCoord++){
                        const Value & coord = point[iCoord];
                        if(!coord.IsDouble() && !coord.IsInt()){
                            setErrorString("coords should be of type Double or Integer!");
                            return -1;
                        }

                        writer.Double(coord.GetDouble());
                    }

                    writer.EndArray();
                }

                writer.EndArray();
            }        
        } //end of polygon

        writer.EndArray();

        stream.Put('\0');
        stream.PutEnd(0);
        //document.Accept(writer);!!! on big data raises an exception!

        int l = strlen(outBuffer);
        char * geomAsStr = new char[l + 50];
        sprintf(geomAsStr, "{ \"type\" : \"%s\", \"coordinates\" : %s}", geomTypeStr, outBuffer);

        //feature->setGeom(geomAsStr); //deep copy
        feature->setGeomDirectly(geomAsStr); //set only pointer
        //delete[] outBuffer;
        //delete[] geomAsStr;
        
        //
        //parsing properties
        //

        const Value & fProperties = dFeature["properties"];
        if(!fProperties.IsObject()){
            setErrorString("\"properties\" element is not a json object!");
            return -1;
        }

        if(!fProperties.HasMember("name")){
            //setErrorString("Geometry structure is inconsistent! \"type\" element is missing!");
            //return -1;
            feature->setName("new json feature");
        }
        else{
            feature->setName(fProperties["name"].GetString());
        }

        //
        //parsing style ????
        //

        //
        //parsing uid ????
        //


        //
        //generating SQL
        //
        if(bGenerateSQL)
            generateSQL(feature);


        m_features[i] = feature;



    } //all features was parsed!

    //delete[] tBuffer;
    delete[] outBuffer;

    m_dirty = false;
    return 1;
}

void KKSJSONParser::generateSQL(JSONFeature * f)
{
    if(!f)
        return;

    if(f->m_sql)
        delete[] f->m_sql;

    f->m_sql = NULL;

    char tableName[50];
    if(f->geomType() == 1){
        strcpy(tableName, "kml_placemarks_points");
    }
    else if(f->geomType() == 2){
        strcpy(tableName, "kml_placemarks_lines");
    }
    else if(f->geomType() == 3){
        strcpy(tableName, "kml_placemarks_poly");
    }
    else{
        return;
    }

    int l_g = 5;
    if(f->m_geom)
        l_g = strlen(f->m_geom);
    int l_s = 5;
    if(f->m_styleString)
        l_s = strlen(f->m_styleString);
    int l_s_id = 5;
    if(f->m_styleId)
        l_s_id = strlen(f->m_styleId);
    int l_n = 5;
    if(f->m_name)
        l_n = strlen(f->m_name);
    int l_u = 5;
    if(f->m_uid)
        l_u = strlen(f->m_uid);


    f->m_sql = new char[l_g + l_s + l_s_id + l_n + l_u + 1000];

    const char * n = f->name();
    sprintf(f->m_sql, "insert into %s (placemark_style_id, name, placemark_uid, placemark_style_string, placemark_geom, placemark_name) "
                         "values (kmlGetStyleInternalId('%s', %d), '%s', getPlacemarkUid('%s'), '%s', ST_GeomFromGeoJSON('%s'), '%s')", 
                           tableName,  
                           f->styleId() ? f->styleId() : "", 
                           f->geomType(),
                           n ? n : "",
                           f->uid() ? f->uid() : "",
                           f->styleString() ? f->styleString() : "",
                           f->geom(),
                           n ? n : "");
    
    
}

void KKSJSONParser::setErrorString(const char * str)
{
    if(m_errorString)
        delete[] m_errorString;
    m_errorString = NULL;

    
    if(str){
        strcpy(m_errorString, str);
        m_errorString = new char[strlen(str)+1];
    }
}

const char * KKSJSONParser::getErrorString()
{
    return m_errorString;
}

int KKSJSONParser::getErrorOffset()
{
    return m_errorOffset;
}

int KKSJSONParser::getFeaturesCount() const
{
    return m_featureCount;
}

JSONFeature * KKSJSONParser::getFeature(int index) const
{
    if(index < 0 || index >= m_featureCount)
        return NULL;

    return m_features[index];
}




/****************************************
****************************************/
JSONFeature::JSONFeature()
{
    m_uid = NULL;
    m_name = NULL;
    m_styleString = NULL;
    m_styleId = NULL;
    m_geom = NULL;

    //m_uid_sql = NULL;
    //m_name_sql = NULL;
    //m_styleString_sql = NULL;
    //m_styleId_sql = NULL;
    //m_geom_sql = NULL;

    m_geomType = 0;
    m_sql = NULL;
}

JSONFeature::~JSONFeature()
{
    if(m_uid)
        delete[] m_uid;
    if(m_name)
        delete[] m_name;
    if(m_styleString)
        delete[] m_styleString;
    if(m_styleId)
        delete[] m_styleId;
    if(m_geom)
        delete[] m_geom;
/*

    if(m_uid_sql)
        delete[] m_uid_sql;
    if(m_name_sql)
        delete[] m_name_sql;
    if(m_styleString_sql)
        delete[] m_styleString_sql;
    if(m_styleId_sql)
        delete[] m_styleId_sql;
    if(m_geom_sql)
        delete[] m_geom_sql;
*/

    if(m_sql)
        delete[] m_sql;
}

void JSONFeature::setUid(const char * s)
{
    if(m_uid)
        delete[] m_uid;

    m_uid = NULL;

    if(s){
        m_uid = new char[strlen(s)+1];
        strcpy(m_uid, s);
    }
}

void JSONFeature::setName(const char * s)
{
    if(m_name)
        delete[] m_name;

    m_name = NULL;
    
    if(s){
        m_name = new char[strlen(s)+1];
        strcpy(m_name, s);
    }
}

void JSONFeature::setStyleString(const char * s)
{
    if(m_styleString)
        delete[] m_styleString;

    m_styleString = NULL;
    
    if(s){
        m_styleString = new char[strlen(s)+1];
        strcpy(m_styleString, s);
    }
}

void JSONFeature::setStyleId(const char * s)
{
    if(m_styleId)
        delete[] m_styleId;

    m_styleId = NULL;
    
    if(s){
        m_styleId = new char[strlen(s)+1];
        strcpy(m_styleId, s);
    }
}

void JSONFeature::setGeom(const char * s)
{
    if(m_geom)
        delete[] m_geom;

    m_geom = NULL;
    
    if(s){
        m_geom = new char[strlen(s)+1];
        strcpy(m_geom, s);
    }
}

void JSONFeature::setGeomDirectly(const char * s)
{
    if(m_geom)
        delete[] m_geom;

    m_geom = NULL;
    
    if(s){
        //m_geom = new char[strlen(s)+1];
        //strcpy(m_geom, s);
        m_geom = (char*) s;
    }
}

void JSONFeature::setGeomType(int t)
{
    m_geomType = t;
}

const char * JSONFeature::uid() const
{
    return m_uid;
}

const char * JSONFeature::name() const
{
    return m_name;
}

const char * JSONFeature::styleString() const
{
    return m_styleString;
}

const char * JSONFeature::styleId() const
{
    return m_styleId;
}

const char * JSONFeature::geom() const
{
    return m_geom;
}

int JSONFeature::geomType() const
{
    return m_geomType;
}

/*
const char * JSONFeature::uid_sql() const
{
    if(!m_uid)
        return "NULL";

    if(m_uid_sql)
        delete[] m_uid_sql;

    m_uid_sql = new char[strlen(m_uid) + 10];

    sprintf(m_uid_sql, "'%s'", m_uid);

    return m_uid_sql;
}

const char * JSONFeature::name_sql() const
{
    if(!m_name)
        return "NULL";

    if(m_name_sql)
        delete[] m_name_sql;

    m_name_sql = new char[strlen(m_name) + 10];

    sprintf(m_name_sql, "'%s'", m_name);

    return m_name_sql;
}

const char * JSONFeature::styleString_sql() const
{
    if(!m_styleString)
        return "NULL";

    if(m_styleString_sql)
        delete[] m_styleString_sql;

    m_styleString_sql = new char[strlen(m_styleString) + 10];

    sprintf(m_styleString_sql, "'%s'", m_styleString);

    return m_styleString_sql;
}

const char * JSONFeature::styleId_sql() const
{
    if(!m_styleId)
        return "NULL";

    if(m_styleId_sql)
        delete[] m_styleId_sql;

    m_styleId_sql = new char[strlen(m_styleId) + 10];

    sprintf(m_styleId_sql, "'%s'", m_styleId);

    return m_styleId_sql;
}

const char * JSONFeature::geom_sql() const
{
    if(!m_geom)
        return "NULL";

    if(m_geom_sql)
        delete[] m_geom_sql;

    m_geom_sql = new char[strlen(m_geom) + 10];

    sprintf(m_geom_sql, "'%s'", m_geom);

    return m_geom_sql;
}
*/

const char * JSONFeature::sql() const
{
  
    return m_sql;
}

char * JSONFeature::generateSQL() const
{

    char * sql = NULL;

    //if(f->m_sql)
    //    delete[] f->m_sql;

    //f->m_sql = NULL;

    char tableName[50];
    if(geomType() == 1){
        strcpy(tableName, "kml_placemarks_points");
    }
    else if(geomType() == 2){
        strcpy(tableName, "kml_placemarks_lines");
    }
    else if(geomType() == 3){
        strcpy(tableName, "kml_placemarks_poly");
    }
    else{
        return NULL;
    }

    if(!m_geom)
        return NULL;

    int l_g = 5;
    if(m_geom)
        l_g = strlen(m_geom);

    int l_s = 5;
    if(m_styleString)
        l_s = strlen(m_styleString);

    int l_s_id = 5;
    if(m_styleId)
        l_s_id = strlen(m_styleId);

    int l_n = 5;
    if(m_name)
        l_n = strlen(m_name);

    int l_u = 5;
    if(m_uid)
        l_u = strlen(m_uid);

    sql = new char[l_g + l_s + l_s_id + l_n + l_u + 1000];

    char * n = new char [l_n+3];
    if(name())
        sprintf(n, "'%s'", name());
    else
        strcpy(n, "NULL");

    char * s_id = new char [l_s_id+3];
    if(styleId())
        sprintf(s_id, "'%s'", styleId());
    else
        strcpy(s_id, "NULL");

    char * s_s = new char [l_s+3];
    if(styleString())
        sprintf(s_s, "'%s'", styleString());
    else
        strcpy(s_s, "NULL");

    char * u = new char [l_u+3];
    if(uid())
        sprintf(u, "'%s'", uid());
    else
        strcpy(u, "NULL");

    sprintf(sql, "insert into %s (placemark_style_id, name, placemark_uid, placemark_style_string, placemark_geom, placemark_name) "
                         "values (kmlGetStyleInternalId(%s, %d), %s, getPlacemarkUid(%s), %s, ST_GeomFromGeoJSON('%s'), %s)", 
                           tableName,  
                           s_id, 
                           geomType(),
                           n,
                           u,
                           s_s,
                           geom(),
                           n);

    delete[] n;
    delete[] s_id;
    delete[] s_s;
    delete[] u;

    return sql;
}

