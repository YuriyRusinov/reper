#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QVector>
#include <QVector3D>
#include <QtCore/qmath.h>

#include "opengl_global.h"

namespace lsl
{
    QVector3D calculationOfNormal(const QVector<QVector3D> &polygon);

    class _GL_EXPORT InterfaceObject3D
    {
    public:
        virtual void setObjectName(const QString&) = 0;
        virtual void setObjectPolygons(const QVector<QVector3D>&,const QVector<QVector3D>&) = 0;

        virtual QString getObjectName() const = 0;
        virtual QVector<QVector3D> getObjectVerticies() const = 0;
        virtual QVector<QVector3D> getObjectFaces() const = 0;
        virtual QVector<QVector3D> getObjectNormals() const = 0;

        virtual QVector<QVector3D> polygon(unsigned int) const = 0;
        virtual QVector3D normal(unsigned int) const = 0;
    };

    class _GL_EXPORT Object3D: public InterfaceObject3D
    {
    public:
        Object3D();
        Object3D(const QString& in_objectName,const QVector<QVector3D>& in_vertices,const QVector<QVector3D>& in_faces);
        Object3D(const Object3D& rhs);

        virtual ~Object3D();

        Object3D& operator=(const Object3D& rhs);

        virtual void setObjectName(const QString&);
        virtual void setObjectPolygons(const QVector<QVector3D>&in_vertices, const QVector<QVector3D>&in_faces);

        virtual QString getObjectName() const;
        virtual QVector<QVector3D> getObjectVerticies() const;
        virtual QVector<QVector3D> getObjectFaces() const;
        virtual QVector<QVector3D> getObjectNormals() const;

        virtual QVector<QVector3D> polygon(unsigned int) const;
        virtual QVector3D normal(unsigned int index) const;

    private:
        QString objectName;
        QVector<QVector3D> vertices;
        QVector<QVector3D> faces;
        QVector<QVector3D> normals;

        void calculatingNormals();
    };

}//lsl

#endif // OBJECT3D_H
