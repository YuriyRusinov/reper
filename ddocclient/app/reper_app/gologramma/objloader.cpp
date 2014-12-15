#include "objloader.h"

void mslLoader::OBJloader::load(const std::string& fileName)
{
    if(fileName.empty())
        return;

    std::ifstream file(fileName.c_str(), std::ios_base::in);
    if(!file)
        return;

    std::string bufferString;
    char buffer_c_string[256];
    const int streamSize = 256;

    mesh.vertices.clear();
    mesh.faces.clear();

    while(!file.eof())
    {
        file.getline(buffer_c_string,streamSize);
        bufferString = buffer_c_string;

        loadValue(bufferString);
    }

}

void mslLoader::OBJloader::loadValue(const std::string& in_string)
{
    std::istringstream stringStream(in_string);
    std::string bufferString;

    mslMesh::point3Ddouble bufferPoint;
    mslMesh::faceWithNormal3D<int> bufferFace;

    if(in_string.find("o ") != std::string::npos)
    {
        stringStream >> meshName >> meshName;
        return;
    }
    if(in_string.find("v ") != std::string::npos)
    {
        stringStream >> bufferString;
        stringStream >> bufferPoint.first >> bufferPoint.second >> bufferPoint.third;
        mesh.vertices.push_back(bufferPoint);
        return;
    }
    if(in_string.find("vn ") != std::string::npos)
    {
        stringStream >> bufferString;
        stringStream >> bufferFace.normal.first >> bufferFace.normal.second >> bufferFace.normal.third;
        mesh.faces.push_back(bufferFace);
        return;
    }
    if(in_string.find("f ") != std::string::npos)
    {
        std::size_t found1,found2;
        int index;

        found1 = in_string.find(" ");
        found2 = in_string.find("/");
        std::string num(in_string.substr(found1 + 1,found2 - found1 - 1));

        found1 = in_string.find("/",found2 + 1);
        found2 = in_string.find(" ",found1);
        index = atoi(in_string.substr(found1 + 1,found2 - found1 - 1).c_str());

        mesh.faces[index-1].face.push_back(atoi(num.c_str()));

        found1 = found2;
        found2 = in_string.find("/",found1);
        num = in_string.substr(found1 + 1,found2 - found1 - 1);
        mesh.faces[index-1].face.push_back(atoi(num.c_str()));

        found1 = in_string.find("/",found2 + 1);
        found2 = in_string.find(" ",found1);

        found1 = found2;
        found2 = in_string.find("/",found1);
        num = in_string.substr(found1 + 1,found2 - found1 - 1);
        mesh.faces[index-1].face.push_back(atoi(num.c_str()));

        found1 = in_string.find("/",found2 + 1);
        found2 = in_string.find(" ",found1);

        if(found2 == std::string::npos)
        {
            return;
        }

        found1 = found2;
        found2 = in_string.find("/",found1);
        num = in_string.substr(found1 + 1,found2 - found1 - 1);
        mesh.faces[index-1].face.push_back(atoi(num.c_str()));

        return;
    }
}

void mslLoader::OBJloader::save(const std::string& fileName)
{
    if(fileName.empty())
        return;

    std::ofstream file(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
    if(!file)
        return;

    if(mesh.faces.empty() || mesh.vertices.empty())
        return;

    file << "# Blender v2.66 (sub 1) OBJ File: ''\n# www.blender.org\n";
    file << "o " << meshName << "\n";

    mslMesh::vertices3D::const_iterator verticies_iter = mesh.vertices.begin();
    for(; verticies_iter != mesh.vertices.end(); ++verticies_iter)
    {
        file << "v " << std::fixed << (*verticies_iter).first << " " << (*verticies_iter).second
             << " " << (*verticies_iter).third << "\n";
    }

    mslMesh::VectorIndexedFaces::const_iterator normals_iter = mesh.faces.begin();
    for(; normals_iter != mesh.faces.end(); ++normals_iter)
    {
        file << "vn " << std::fixed << (*normals_iter).normal.first << " " << (*normals_iter).normal.second
             << " " << (*normals_iter).normal.third << "\n";
    }

    file << "usemtl Material\ns off\n";

    normals_iter = mesh.faces.begin();
    std::vector<int>::const_iterator faceIndex_iter;
    int normalNumber = 1;
    for(; normals_iter != mesh.faces.end(); ++normals_iter)
    {
        faceIndex_iter = (*normals_iter).face.begin();

        file << "f ";

        for(; faceIndex_iter != (*normals_iter).face.end(); ++faceIndex_iter)
        {
            file << (*faceIndex_iter) << "//" << normalNumber << " ";
        }        

        file << "\n";

        normalNumber++;
    }
}

void mslLoader::OBJloader::setMesh(const mslMesh::mesh3D& in_mesh)
{
    mesh = in_mesh;
}

mslMesh::mesh3D mslLoader::OBJloader::getMesh() const
{
    return mesh;
}

void mslLoader::OBJloader::setMeshName(const std::string& in_name)
{
    meshName = in_name;
}

std::string mslLoader::OBJloader::getMeshName() const
{
    return meshName;
}
