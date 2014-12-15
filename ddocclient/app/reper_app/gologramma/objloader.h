#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QStringList>

#include <fstream>
#include <sstream>

#include "gologramma_global.h"
#include "model3Dconst.h"

namespace _GOL_EXPORT mslLoader
{
    class  OBJloader
    {
    public:
        OBJloader(){};

        void load(const std::string&);
        void save(const std::string&);

        void setMesh(const mslMesh::mesh3D&);
        mslMesh::mesh3D getMesh() const;

        void setMeshName(const std::string&);
        std::string getMeshName() const;

    private:
        OBJloader(const OBJloader&);
        OBJloader& operator=(const OBJloader&);

        std::string meshName;
        mslMesh::mesh3D mesh;

        void loadValue(const std::string &in_string);
    };
}

#endif // OBJLOADER_H
