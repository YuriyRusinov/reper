#include "object.h"

using namespace lsl;

QVector3D lsl::calculationOfNormal(const QVector<QVector3D>& polygon)
{

    qreal A,B,C,D;

    A = (polygon.at(1).y() - polygon.at(0).y())*(polygon.at(2).z() - polygon.at(0).z())
      - (polygon.at(1).z() - polygon.at(0).z())*(polygon.at(2).y() - polygon.at(0).y());

    B = (-1) * (polygon.at(1).x() - polygon.at(0).x())*(polygon.at(2).z() - polygon.at(0).z())
      - (polygon.at(1).z() - polygon.at(0).z())*(polygon.at(2).x() - polygon.at(0).x());

    C = (polygon.at(1).x() - polygon.at(0).x())*(polygon.at(2).y() - polygon.at(0).y())
      - (polygon.at(1).y() - polygon.at(0).y())*(polygon.at(2).x() - polygon.at(0).x());

    D = qSqrt(A*A + B*B + C*C);

    if(!D)
        return QVector3D(qreal(),qreal(),qreal());

    return QVector3D(A/D,B/D,C/D);
}

lsl::Object3D::Object3D()
{
}

lsl::Object3D::Object3D(const QString &in_objectName, const QVector<QVector3D> &in_vertices, const QVector<QVector3D> &in_faces):
    objectName(in_objectName),vertices(in_vertices),faces(in_faces)
{
    calculatingNormals();
}

lsl::Object3D::Object3D(const Object3D& rhs)
{
    *this = rhs;
}

lsl::Object3D::~Object3D()
{
}

Object3D& lsl::Object3D::operator=(const Object3D& rhs)
{
    if(this == &rhs)
        return *this;

    setObjectName(rhs.getObjectName());
    setObjectPolygons(rhs.getObjectVerticies(),rhs.getObjectFaces());

    return *this;
}

void lsl::Object3D::setObjectName(const QString& in_objectName)
{
    objectName = in_objectName;
}

void lsl::Object3D::setObjectPolygons(const QVector<QVector3D>& in_vertices,const QVector<QVector3D>& in_faces)
{
    vertices = in_vertices;
    faces = in_faces;

    if(!vertices.empty() && !faces.empty())
    {
        calculatingNormals();
    }
}

QString lsl::Object3D::getObjectName() const
{
    return objectName;
}

QVector<QVector3D> lsl::Object3D::getObjectVerticies() const
{
    return vertices;
}

QVector<QVector3D> lsl::Object3D::getObjectFaces() const
{
    return faces;
}

QVector<QVector3D> lsl::Object3D::getObjectNormals() const
{
    return normals;
}

QVector<QVector3D> lsl::Object3D::polygon(unsigned int index) const
{
    QVector<QVector3D> bufPolygon;
    bufPolygon.reserve(3);

    int f = static_cast<unsigned int>(faces.at(index).x() - 1);
    int s = static_cast<unsigned int>(faces.at(index).y() - 1);
    int t = static_cast<unsigned int>(faces.at(index).z() - 1);

    bufPolygon.push_back(vertices.at(f));
    bufPolygon.push_back(vertices.at(s));
    bufPolygon.push_back(vertices.at(t));

    return bufPolygon;
}

QVector3D lsl::Object3D::normal(unsigned int index) const
{
    return normals.at(index);
}

void lsl::Object3D::calculatingNormals()
{
    if(!normals.isEmpty())
        normals.clear();

    unsigned int nf = faces.size();
    normals.reserve(nf);

    for(unsigned int i = 0; i < nf ; ++i)
    {
        QVector<QVector3D> pol = polygon(i);
        normals.push_back(calculationOfNormal(pol));
    }

    int a;
    Q_UNUSED (a);
}
