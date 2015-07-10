#include <QProgressDialog>
#include "cuboid.h"

Cuboid::Cuboid(mslMesh::point3Ddouble& in_maxPoint,mslMesh::point3Ddouble& in_minPoint)
{
    init(in_maxPoint,in_minPoint);
}

Cuboid::Cuboid(const mslMesh::vertices3D& initVector)
{
    cuboidFromVector(initVector);
}

Cuboid::Cuboid()
{
    mslMesh::point3Ddouble maxPoint(mslMesh::make_point3D(1.0,1.0,1.0));
    mslMesh::point3Ddouble minPoint(mslMesh::make_point3D(-1.0,-1.0,-1.0));

    init(maxPoint,minPoint);
}

void Cuboid::setMesh(const mslMesh::mesh3D &in_cuboid)
{
    cuboid = in_cuboid;
}

mslMesh::mesh3D Cuboid::getMesh() const
{
    return cuboid;
}

mslMesh::point3Ddouble Cuboid::getVisionDirection()
{
    return visionNormal;
}

void Cuboid::cuboidFromVector(const mslMesh::vertices3D& initVector)
{
    cuboid.vertices.resize(8);
    cuboid.faces.resize(6);

    mslMesh::simpleFace3D bufferFace;
    mslMesh::VectorFaces3D facesForMesh;

    bufferFace.push_back(initVector.at(0));
    bufferFace.push_back(initVector.at(1));
    bufferFace.push_back(initVector.at(3));
    bufferFace.push_back(initVector.at(4));
    facesForMesh.push_back(bufferFace);
    visionNormal = calculationOfNormal(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(initVector.at(0));
    bufferFace.push_back(initVector.at(1));
    bufferFace.push_back(initVector.at(4));
    bufferFace.push_back(initVector.at(5));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(initVector.at(5));
    bufferFace.push_back(initVector.at(4));
    bufferFace.push_back(initVector.at(6));
    bufferFace.push_back(initVector.at(7));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(initVector.at(3));
    bufferFace.push_back(initVector.at(2));
    bufferFace.push_back(initVector.at(6));
    bufferFace.push_back(initVector.at(7));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(initVector.at(0));
    bufferFace.push_back(initVector.at(5));
    bufferFace.push_back(initVector.at(7));
    bufferFace.push_back(initVector.at(3));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(initVector.at(1));
    bufferFace.push_back(initVector.at(4));
    bufferFace.push_back(initVector.at(6));
    bufferFace.push_back(initVector.at(2));
    facesForMesh.push_back(bufferFace);

    cuboid = mslMesh::make_mesh3D(facesForMesh);
}

void Cuboid::init(const mslMesh::point3Ddouble& in_maxPoint,const mslMesh::point3Ddouble& in_minPoint)
{
    cuboid.vertices.resize(8);
    cuboid.faces.resize(6);

    mslMesh::simpleFace3D bufferFace;
    mslMesh::VectorFaces3D facesForMesh;

    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_minPoint.third));
    facesForMesh.push_back(bufferFace);
    visionNormal = calculationOfNormal(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_minPoint.third));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_minPoint.third));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_minPoint.third));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_minPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_minPoint.third));
    facesForMesh.push_back(bufferFace);
    bufferFace.clear();

    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_minPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_minPoint.first,in_maxPoint.second,in_maxPoint.third));
    bufferFace.push_back(mslMesh::make_point3D(in_maxPoint.first,in_maxPoint.second,in_maxPoint.third));
    facesForMesh.push_back(bufferFace);

    cuboid = mslMesh::make_mesh3D(facesForMesh);
}

void Cuboid::scale(mslMesh::point3Ddouble in_scale)
{
    mslMesh::scaleMesh(this->cuboid,in_scale);
}

void Cuboid::move(mslMesh::point3Ddouble in_move)
{
    mslMesh::moveMesh(cuboid,in_move);
}

void Cuboid::rotate(mslMesh::point3Ddouble in_rotate)
{
    mslMesh::rotateMesh(cuboid,in_rotate);
}

void Cuboid::rotateInSpace(mslMesh::point3Ddouble in_rotate)
{
    mslMesh::rotateMeshInSpace(cuboid,in_rotate);
}

mslMesh::point3Ddouble Cuboid::center() const
{
    return cuboid.center();
}

std::vector<Cuboid> Cuboid::divideCub(double in_length) const
{
    mslMesh::point3Ddouble maxPoint(mslMesh::maxPoint3Delements(cuboid.vertices));
    mslMesh::point3Ddouble minPoint(mslMesh::minPoint3Delements(cuboid.vertices));

    int col = static_cast<int>((maxPoint.first - minPoint.first) / in_length);

    std::vector<Cuboid> cubsVector;
    cubsVector.reserve(col*col*col);
    std::vector<double> lengthNum;
    std::vector<double> lengthNumOne;
    lengthNum.reserve(col);
    lengthNumOne.reserve(col);

    for(int i = 0; i < col; i++)
    {
        lengthNum.push_back(i*in_length);
        lengthNumOne.push_back((i+1)*in_length);
    }

    for(int i = 0; i < col ;i++)//X
    {
        double maxX = maxPoint.first - lengthNum.at(i);
        double minX = maxPoint.first - lengthNumOne.at(i);

        for(int j = 0; j < col ;j++)//Y
        {
            double maxY = maxPoint.second - lengthNum.at(j);
            double minY = maxPoint.second - lengthNumOne.at(j);

            for(int k = 0; k < col ;k++)//Z
            {
                double maxZ = maxPoint.third - lengthNum.at(k);
                double minZ = maxPoint.third - lengthNumOne.at(k);

                mslMesh::point3Ddouble maxP;
                mslMesh::point3Ddouble minP;

                maxP = mslMesh::make_point3D(maxX,maxY,maxZ);
                minP = mslMesh::make_point3D(minX,minY,minZ);

                cubsVector.push_back(Cuboid(maxP,minP));
            }
        }
    }

    return cubsVector;
}

bool Cuboid::pointInCuboid(const mslMesh::point3Ddouble &in_point) const
{
    std::vector<mslMesh::simpleFace3D> facesForPlane;
    facesForPlane.reserve(6);

    mslMesh::VectorIndexedFaces::const_iterator cuboidFaces = cuboid.faces.begin();
    for(; cuboidFaces != cuboid.faces.end(); ++cuboidFaces)
    {
        facesForPlane.push_back(cuboid.getFace(cuboidFaces));
    }

    if(mslMesh::pointOnPlane(facesForPlane.at(0),in_point) < 0)
        return false;
    if(mslMesh::pointOnPlane(facesForPlane.at(1),in_point) > 0)
        return false;
    if(mslMesh::pointOnPlane(facesForPlane.at(2),in_point) > 0)
        return false;
    if(mslMesh::pointOnPlane(facesForPlane.at(3),in_point) < 0)
        return false;
    if(mslMesh::pointOnPlane(facesForPlane.at(4),in_point) > 0)
        return false;
    if(mslMesh::pointOnPlane(facesForPlane.at(5),in_point) < 0)
        return false;

    return true;
}

bool Cuboid::faceInCuboid(const mslMesh::simpleFace3D& in_face) const
{
    mslMesh::simpleFace3D::const_iterator facePoint = in_face.begin();
    for(; facePoint != in_face.end(); ++facePoint)
    {
        if(pointInCuboid(*facePoint))
            return true;
    }

    return false;
}


void Cuboid::toMaxPow2(double in_length)
{
    double lengthOf2 = in_length*2;
    double lengthCub = cuboid.vertices.at(2).first - cuboid.vertices.at(5).first;

    while(lengthOf2 < lengthCub)
    {
        lengthOf2*=2;
    }

    lengthOf2 /= lengthCub;

    scale(mslMesh::make_point3D(lengthOf2,lengthOf2,lengthOf2));
}

std::vector<unsigned char> createImageMatrix(const std::vector<Cuboid>& in_cubs,const mslMesh::mesh3D& in_object, QProgressDialog * pD)
{
    std::vector<unsigned char> imageMatrix(in_cubs.size());
    int lineSize = static_cast<int>(pow(static_cast<double>(imageMatrix.size()),1.0/3.0)) + 1;

    std::pair<mslMesh::point3D<int,int,int>,mslMesh::point3D<int,int,int> > indexCubeVector;
    indexCubeVector.first.first = 0;
    indexCubeVector.first.second = 0;
    indexCubeVector.first.third = 0;
    indexCubeVector.second.first = lineSize-1;
    indexCubeVector.second.second = lineSize-1;
    indexCubeVector.second.third = lineSize-1;

    std::vector<mslMesh::simpleFace3D> objects;

    mslMesh::VectorIndexedFaces::const_iterator iter = in_object.faces.begin();
    for(; iter != in_object.faces.end(); ++iter)
    {
        objects.push_back(in_object.getFace(iter));
    }

	if (pD && pD->wasCanceled())
		return imageMatrix;
    recurseCub(indexCubeVector,in_cubs,objects,imageMatrix,lineSize);
	if (pD && pD->wasCanceled())
		return imageMatrix;

    return imageMatrix;
}

void recurseCub(const std::pair<mslMesh::point3D<int, int, int>, mslMesh::point3D<int, int, int> >& in_indexCubeVector,
                const std::vector<Cuboid>& in_cubs,
                const std::vector<mslMesh::simpleFace3D>& in_object,
                std::vector<unsigned char>& in_imageMatrix,
                int lineSize)
{
    if((in_indexCubeVector.second.first - in_indexCubeVector.first.first) <= 0 ||
       (in_indexCubeVector.second.second - in_indexCubeVector.first.second) <= 0 ||
       (in_indexCubeVector.second.third - in_indexCubeVector.first.third) <= 0)
    {
        int indexMain = in_indexCubeVector.first.first*lineSize*lineSize +
                        in_indexCubeVector.first.second*lineSize +
                        in_indexCubeVector.first.third;

        in_imageMatrix[indexMain] = 0;

        if(!in_object.empty())
        {
            std::vector<mslMesh::simpleFace3D>::const_iterator iter = in_object.begin();
            for(; iter != in_object.end(); ++iter)
            {
                if(in_cubs.at(indexMain).faceInCuboid(*iter))
                {
                    in_imageMatrix[indexMain] = 1;
                    break;
                }
            }
        }

        return;
    }

    //0
    mslMesh::vertices3D pointsForCub;
    mslMesh::point3Ddouble pointM = in_cubs.at(in_indexCubeVector.first.first*lineSize*lineSize +
                                    in_indexCubeVector.second.second*lineSize +
                                    in_indexCubeVector.second.third).getMesh().vertices.at(0);
    pointsForCub.push_back(pointM);
    //1
    pointM = in_cubs.at(in_indexCubeVector.first.first*lineSize*lineSize +
                                    in_indexCubeVector.second.second*lineSize +
                                    in_indexCubeVector.first.third).getMesh().vertices.at(1);
    pointsForCub.push_back(pointM);
    //2
    pointM = in_cubs.at(in_indexCubeVector.first.first*lineSize*lineSize +
                                    in_indexCubeVector.first.second*lineSize +
                                    in_indexCubeVector.first.third).getMesh().vertices.at(2);
    pointsForCub.push_back(pointM);
    //3
    pointM = in_cubs.at(in_indexCubeVector.first.first*lineSize*lineSize +
                                    in_indexCubeVector.first.second*lineSize +
                                    in_indexCubeVector.second.third).getMesh().vertices.at(3);
    pointsForCub.push_back(pointM);
    //4
    pointM = in_cubs.at(in_indexCubeVector.second.first*lineSize*lineSize +
                                    in_indexCubeVector.second.second*lineSize +
                                    in_indexCubeVector.first.third).getMesh().vertices.at(4);
    pointsForCub.push_back(pointM);
    //5
    pointM = in_cubs.at(in_indexCubeVector.second.first*lineSize*lineSize +
                                    in_indexCubeVector.second.second*lineSize +
                                    in_indexCubeVector.second.third).getMesh().vertices.at(5);
    pointsForCub.push_back(pointM);
    //6
    pointM = in_cubs.at(in_indexCubeVector.second.first*lineSize*lineSize +
                                    in_indexCubeVector.first.second*lineSize +
                                    in_indexCubeVector.first.third).getMesh().vertices.at(6);
    pointsForCub.push_back(pointM);
    //7
    pointM = in_cubs.at(in_indexCubeVector.second.first*lineSize*lineSize +
                                    in_indexCubeVector.first.second*lineSize +
                                    in_indexCubeVector.second.third).getMesh().vertices.at(7);
    pointsForCub.push_back(pointM);

    Cuboid cuboid(pointsForCub);
    std::vector<mslMesh::simpleFace3D> new_objects;

    std::vector<mslMesh::simpleFace3D>::const_iterator meshIter = in_object.begin();
    for(; meshIter != in_object.end(); ++meshIter)
    {
        if(cuboid.faceInCuboid(*meshIter))
        {
            new_objects.push_back(*meshIter);
        }
    }

    if(new_objects.empty())
    {
        for(int x = in_indexCubeVector.first.first; x != in_indexCubeVector.second.first; x++)
        {
            for(int y = in_indexCubeVector.first.second; y != in_indexCubeVector.second.second; y++)
            {
                for(int z = in_indexCubeVector.first.third; z != in_indexCubeVector.second.third; z++)
                {
                    in_imageMatrix[x*lineSize*lineSize + y*lineSize + z] = 0;
                }
            }
        }

        return;
    }

    std::pair<mslMesh::point3D<int, int, int>, mslMesh::point3D<int, int, int> > new_indexCubeVector;
    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.second.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2 - 1;
        new_indexCubeVector.second.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2 - 1;
        new_indexCubeVector.second.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third+ 1)/2 - 1;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.second = in_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third;
        new_indexCubeVector.second.first = in_indexCubeVector.second.first;
        new_indexCubeVector.second.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2 - 1;
        new_indexCubeVector.second.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2 - 1;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.first++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third;
        new_indexCubeVector.second.first = in_indexCubeVector.second.first;
        new_indexCubeVector.second.second = in_indexCubeVector.second.second;
        new_indexCubeVector.second.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2 - 1;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.first++;
        new_indexCubeVector.first.second++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third;
        new_indexCubeVector.second.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2 -1;
        new_indexCubeVector.second.second = in_indexCubeVector.second.second;
        new_indexCubeVector.second.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2 - 1;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.second++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2;
        new_indexCubeVector.second.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2 - 1;
        new_indexCubeVector.second.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2 - 1;
        new_indexCubeVector.second.third = in_indexCubeVector.second.third;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.third++;
        new_indexCubeVector.second = in_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2;
        new_indexCubeVector.second.first = in_indexCubeVector.second.first;
        new_indexCubeVector.second.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2 - 1;
        new_indexCubeVector.second.third = in_indexCubeVector.second.third;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.first++;
        new_indexCubeVector.first.third++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2;
        new_indexCubeVector.second = in_indexCubeVector.second;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.first++;
        new_indexCubeVector.first.second++;
        new_indexCubeVector.first.third++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    if(in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1 > 2)
    {
        new_indexCubeVector.first.first = in_indexCubeVector.first.first;
        new_indexCubeVector.first.second = in_indexCubeVector.first.second +
            (in_indexCubeVector.second.second - in_indexCubeVector.first.second + 1)/2;
        new_indexCubeVector.first.third = in_indexCubeVector.first.third +
            (in_indexCubeVector.second.third - in_indexCubeVector.first.third + 1)/2;
        new_indexCubeVector.second.first = in_indexCubeVector.first.first +
            (in_indexCubeVector.second.first - in_indexCubeVector.first.first + 1)/2 - 1;
        new_indexCubeVector.second.second = in_indexCubeVector.second.second;
        new_indexCubeVector.second.third = in_indexCubeVector.second.third;
    }
    else
    {
        new_indexCubeVector.first = in_indexCubeVector.first;
        new_indexCubeVector.first.second++;
        new_indexCubeVector.first.third++;
        new_indexCubeVector.second = new_indexCubeVector.first;
    }
    recurseCub(new_indexCubeVector,in_cubs,new_objects,in_imageMatrix,lineSize);

    return;
}

std::vector<unsigned char> createImage(const std::vector<unsigned char> &imageMatrix)
{
    int lineSize = static_cast<int>(pow(static_cast<double>(imageMatrix.size()),1.0/3.0));
    lineSize++;

    std::vector<unsigned char> image;
    image.reserve(lineSize*lineSize);

    int imageSize;

    for(int x = 0; x < lineSize; x++)
    {
        for(int y = 0; y < lineSize; y++)
        {
            imageSize = image.size();

            for(int z = 0; z < lineSize; z++)
            {
                if(imageMatrix[x*lineSize*lineSize + y*lineSize + z])
                {
                    image.push_back(1);
                    break;
                }
            }

            if(imageSize == image.size())
            {
                image.push_back(0);
            }
        }
    }

    return image;
}

QImage imageTransform(const std::vector<unsigned char>& in_image,const std::string& fileName)
{
    int size = static_cast<int>(sqrt(static_cast<double>(in_image.size())));

    QImage image(size,size,QImage::Format_RGB32);

    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            if(in_image[x*size + y])
            {
                image.setPixel(x,y, qRgb (255,255,255));
            }
            else
            {
                image.setPixel(x,y,Qt::black);
            }
        }
    }

    image.save(fileName.c_str(),"PNG");

    return image;
}
