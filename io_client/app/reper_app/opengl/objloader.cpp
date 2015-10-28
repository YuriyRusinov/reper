#include "objloader.h"

using namespace lsl;

lsl::OBJfile::OBJfile(const QString &fileName)
{
    error = noError;
    load(fileName);
}

void lsl::OBJfile::load(const QString &fileName)
{
    if(fileName.isEmpty())
    {
        error = noFilename;
        return;
    }

    std::ifstream file(fileName.toStdString().c_str(), std::ios_base::in);
    if(!file)
    {
        error = notOpen;
        return;
    }

    std::string bufferString;
    char buffer_c_string[256];
    const int streamSize = 256;

    while(!file.eof())
    {
        file.getline(buffer_c_string,streamSize);
        bufferString = buffer_c_string;

        loadValue(bufferString);
    }
}

void lsl::OBJfile::save(const QString &fileName)
{
    if(vertices.empty() || normals.empty() || faces.empty())
    {
        error = noData;
        return;
    }

    if(fileName.isEmpty())
    {
        error = noFilename;
        return;
    }

    std::ofstream file(fileName.toStdString().c_str(), std::ios_base::out | std::ios_base::trunc);
    if(!file)
    {
        error = notOpen;
        return;
    }

    file << "# Blender v2.66 (sub 1) OBJ File: ''\n# www.blender.org\n";
    file << "o " << objectName.toStdString() << "\n";

    QVector<QVector3D>::const_iterator verticies_iter = vertices.begin();
    for(; verticies_iter != vertices.end(); ++verticies_iter)
    {
        file << "v " << std::fixed << (*verticies_iter).x() << " " << (*verticies_iter).y()
             << " " << (*verticies_iter).z() << "\n";
    }

    verticies_iter = normals.begin();
    for(; verticies_iter != normals.end(); ++verticies_iter)
    {
        file << "vn " << std::fixed << (*verticies_iter).x() << " " << (*verticies_iter).y()
             << " " << (*verticies_iter).z() << "\n";
    }

    file << "usemtl Material\ns off\n";

    verticies_iter = faces.begin();
    int normalNumber = 1;
    for(; verticies_iter != faces.end(); ++verticies_iter)
    {
        file << "f " << static_cast<int>((*verticies_iter).x()) << "//" << normalNumber
             << " " << static_cast<int>((*verticies_iter).y()) << "//" << normalNumber
             << " " << static_cast<int>((*verticies_iter).z()) << "//" << normalNumber << "\n";

        normalNumber++;
    }
}

void lsl::OBJfile::setObject(lsl::Object3D& obj)
{
    objectName = obj.getObjectName();
    normals = obj.getObjectNormals();
    vertices = obj.getObjectVerticies();
    faces = obj.getObjectFaces();
}

Object3D lsl::OBJfile::getObject() const
{
    if(faces.empty() || vertices.empty())
    {
        return Object3D();
    }

    return Object3D(objectName,vertices,faces);
}

void lsl::OBJfile::loadValue(const std::string& in_string)
{
    std::istringstream stringStream(in_string);
    std::string bufferString;

    if(in_string.find("o ") != std::string::npos)
    {
        stringStream >> bufferString >> bufferString;
        objectName = bufferString.c_str();
        return;
    }
    if(in_string.find("v ") != std::string::npos)
    {
        double first,second,third;

        stringStream >> bufferString;
        stringStream >> first >> second >> third;
        vertices.push_back(QVector3D(first,second,third));
        return;
    }
    if(in_string.find("vn ") != std::string::npos)
    {
        double first,second,third;

        stringStream >> bufferString;
        stringStream >> first >> second >> third;
        normals.push_back(QVector3D(first,second,third));
        return;
    }
    if(in_string.find("f ") != std::string::npos)
    {
        std::size_t found1,found2;
        int first,second,third;

        found1 = in_string.find(" ");
        found2 = in_string.find("/");
        std::string num(in_string.substr(found1 + 1,found2 - found1 - 1));
        first = atoi(num.c_str());

        found1 = in_string.find("/",found2 + 1);
        found2 = in_string.find(" ",found1);

        found1 = found2;
        found2 = in_string.find("/",found1);
        num = in_string.substr(found1 + 1,found2 - found1 - 1);
        second = atoi(num.c_str());

        found1 = in_string.find("/",found2 + 1);
        found2 = in_string.find(" ",found1);

        found1 = found2;
        found2 = in_string.find("/",found1);
        num = in_string.substr(found1 + 1,found2 - found1 - 1);
        third = atoi(num.c_str());

        faces.push_back(QVector3D(static_cast<qreal>(first),static_cast<qreal>(second),static_cast<qreal>(third)));

        return;
    }
}

OBJfile_error lsl::OBJfile::getError() const
{
    return error;
}
