#ifndef OBJFILE_H
#define OBJFILE_H

#include <fstream>
#include <sstream>

#include <object.h>

#include "opengl_global.h"

namespace lsl
{
    enum OBJfile_error{noError = 0,noFilename,notOpen,noData};

    class _GL_EXPORT OBJfile
    {
    public:
        OBJfile(){error = noError;}
        OBJfile(const QString& fileName);

        void load(const QString& fileName);
        void save(const QString& fileName);

        OBJfile_error getError() const;

        void setObject(Object3D&);
        Object3D getObject() const;

    private:
        OBJfile(const OBJfile&);
        OBJfile& operator=(const OBJfile&);

        QString objectName;
        QVector<QVector3D> vertices;
        QVector<QVector3D> normals;
        QVector<QVector3D> faces;

        OBJfile_error error;

        void loadValue(const std::string &in_string);
    };
};//lsl

#endif // OBJFILE_H
