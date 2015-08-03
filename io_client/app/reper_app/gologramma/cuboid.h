#ifndef CUBOID_H
#define CUBOID_H

#include "model3Dconst.h"
#include "objloader.h"

#include <QImage>

class QProgressDialog;

class Cuboid
{
public:
    Cuboid(mslMesh::point3Ddouble& in_maxPoint,mslMesh::point3Ddouble& in_minPoint);
    Cuboid(const mslMesh::vertices3D& initVector);
    Cuboid();

    void setMesh(const mslMesh::mesh3D& in_cuboid);
    mslMesh::mesh3D getMesh() const;
    mslMesh::point3Ddouble getVisionDirection();

    void scale(mslMesh::point3Ddouble in_scale);
    void move(mslMesh::point3Ddouble in_move);
    void rotate(mslMesh::point3Ddouble in_rotate);
    void rotateInSpace(mslMesh::point3Ddouble in_rotate);

    mslMesh::point3Ddouble center() const;

    std::vector<Cuboid> divideCub(double) const;

    bool pointInCuboid(const mslMesh::point3Ddouble&) const;
    bool faceInCuboid(const mslMesh::simpleFace3D&) const;

    void toMaxPow2(double in_length);

private:
    mslMesh::mesh3D cuboid;
    mslMesh::point3Ddouble visionNormal;

    void init(const mslMesh::point3Ddouble &in_maxPoint, const mslMesh::point3Ddouble &in_minPoint);
    void cuboidFromVector(const mslMesh::vertices3D& initVector);
};

std::vector<unsigned char> createImageMatrix(const std::vector<Cuboid>&,const mslMesh::mesh3D&, QProgressDialog * pD=0);
std::vector<unsigned char> createImage(const std::vector<unsigned char> &);

void recurseCub(const std::pair<mslMesh::point3D<int, int, int>, mslMesh::point3D<int, int, int> > &,
                const std::vector<Cuboid>&,
                const std::vector<mslMesh::simpleFace3D>&,
                std::vector<unsigned char>&,
                int lineSize);

QImage imageTransform(const std::vector<unsigned char>&, const std::string &);

#endif // CUBOID_H
