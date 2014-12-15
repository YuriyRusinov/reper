#include "model3Dconst.h"

inline bool mslMesh::versionCompare(unsigned int &in_version)
{
    if(mslMesh::version == in_version)
        return true;
    else
        return false;
}

template <class T1,class T2,class T3>
bool mslMesh::operator==(const point3D<T1,T2,T3>& lhs,const point3D<T1,T2,T3>& rhs)
{
    return (lhs.first == rhs.first) && (lhs.second == rhs.second) && (lhs.third == rhs.third);
}

template <class T1,class T2,class T3>
bool mslMesh::operator!=(const point3D<T1,T2,T3>& lhs,const point3D<T1,T2,T3>& rhs)
{
    return (lhs.first != rhs.first) || (lhs.second != rhs.second) || (lhs.third != rhs.third);
}

template<class T1,class T2,class T3>
mslMesh::point3D<T1,T2,T3> mslMesh::make_point3D(const T1& x,const T2& y,const T3& z)
{
    return mslMesh::point3D<T1,T2,T3>(x,y,z);
}

template <class T>
bool mslMesh::operator==(const faceWithNormal3D<T>& lhs,const faceWithNormal3D<T>& rhs)
{
    return lhs.face == rhs.face && lhs.normal == rhs.normal;
}

template <class T>
bool mslMesh::operator!=(const faceWithNormal3D<T>& lhs,const faceWithNormal3D<T>& rhs)
{
    return lhs.face != rhs.face || lhs.normal != rhs.normal;
}

void mslMesh::rotatePoint(mslMesh::point3Ddouble& in_point,const mslMesh::point3Ddouble& in_angle)
{
    double first,second,third;

    first = in_point.first;
    second = in_point.second;
    third = in_point.third;

    in_point.first = first * sincos_table.cos(in_angle.second)*sincos_table.cos(in_angle.third) +
        second * (sincos_table.sin(in_angle.first)*sincos_table.sin(in_angle.second)*sincos_table.cos(in_angle.third) +
        sincos_table.sin(in_angle.third)*sincos_table.cos(in_angle.first)) + third * (sincos_table.sin(in_angle.first)*sincos_table.sin(in_angle.third) -
        sincos_table.sin(in_angle.second) * sincos_table.cos(in_angle.first) * sincos_table.cos(in_angle.third));

    in_point.second = first * sincos_table.cos(in_angle.second)*sincos_table.sin(in_angle.third) * (-1.0) +
        second * ((-1)*sincos_table.sin(in_angle.first)*sincos_table.sin(in_angle.second)*sincos_table.sin(in_angle.third) +
        sincos_table.cos(in_angle.third)*sincos_table.cos(in_angle.first)) + third * (sincos_table.sin(in_angle.first)*sincos_table.cos(in_angle.third) +
        sincos_table.sin(in_angle.second) * sincos_table.cos(in_angle.first) * sincos_table.sin(in_angle.third));

    in_point.third = first * sincos_table.sin(in_angle.second) +
        second * (-1)*sincos_table.sin(in_angle.first)*sincos_table.cos(in_angle.second) +
        third * sincos_table.cos(in_angle.first)*sincos_table.cos(in_angle.second);
}

mslMesh::point3Ddouble mslMesh::calculationOfNormal(const simpleFace3D &in_face)
{
    if(!mslMesh::normalExistenceCondicion(in_face))
        return mslMesh::make_point3D(double(),double(),double());

    double A,B,C,D;
    A = B = C = D = double();

    A = (in_face.at(1).second - in_face.at(0).second)*(in_face.at(2).third - in_face.at(0).third)
      - (in_face.at(1).third - in_face.at(0).third)*(in_face.at(2).second - in_face.at(0).second);

    B = (-1) * ((in_face.at(1).first - in_face.at(0).first)*(in_face.at(2).third - in_face.at(0).third)
      - (in_face.at(1).third - in_face.at(0).third)*(in_face.at(2).first - in_face.at(0).first));

    C = (in_face.at(1).first - in_face.at(0).first)*(in_face.at(2).second - in_face.at(0).second)
      - (in_face.at(1).second - in_face.at(0).second)*(in_face.at(2).first - in_face.at(0).first);

    D = sqrt(A*A + B*B + C*C);

    return mslMesh::make_point3D(A/D,B/D,C/D);
}

inline bool mslMesh::normalExistenceCondicion(const mslMesh::simpleFace3D& in_face)
{
    if(in_face.size() >= mslMesh::minVerticesForNormal)
        return true;
    else
        return false;
}

mslMesh::point3Ddouble mslMesh::mesh3D::center() const
{
    if(mslMesh::mesh3D::faces.empty() || mslMesh::mesh3D::vertices.empty())
        return make_point3D(1000000.0,1000000.0,1000000.0);

    point3Ddouble centerPoint;

    mslMesh::vertices3D::const_iterator vertice_iter = this->vertices.begin();
    for(; vertice_iter != this->vertices.end(); ++vertice_iter)
    {
        centerPoint.first += (*vertice_iter).first;
        centerPoint.second += (*vertice_iter).second;
        centerPoint.third += (*vertice_iter).third;
    }

    centerPoint.first /= this->vertices.size();
    centerPoint.second /= this->vertices.size();
    centerPoint.third /= this->vertices.size();

    return centerPoint;
}

void mslMesh::mesh3D::calculationOfNormal()
{
    if(this->faces.empty() || this->vertices.empty())
        return;

   mslMesh::VectorIndexedFaces::iterator face_iter = this->faces.begin();
   for(;face_iter != this->faces.end(); ++face_iter)
   {
       (*face_iter).normal = mslMesh::calculationOfNormal(mslMesh::mesh3D::getFace(face_iter));
   }
}

mslMesh::simpleFace3D mslMesh::mesh3D::getFace(const VectorIndexedFaces::const_iterator& in_faceIter) const
{
    mslMesh::simpleFace3D meshFace;

    for(std::size_t i = 0; i < (*in_faceIter).face.size(); i++)
    {
        meshFace.push_back(this->vertices.at((*in_faceIter).face.at(i) - 1));
    }

    return meshFace;
}

bool mslMesh::pointInVector3D(const point3Ddouble& point,const vertices3D& vector)
{
    vertices3D::const_iterator point_iter = vector.begin();
    for(; point_iter != vector.end(); ++point_iter)
    {
        if(point == *point_iter)
            return true;
    }

    return false;
}

mslMesh::vertices3D mslMesh::uniquePoints3D(const VectorFaces3D& in_faces)
{
    std::vector<point3D<double,double,double> > vertices;

    VectorFaces3D::const_iterator in_iter = in_faces.begin();
    std::vector<point3D<double,double,double> >::const_iterator face_iter;

    for(; in_iter != in_faces.end(); ++in_iter)
    {
        face_iter = (*in_iter).begin();

        for(; face_iter != (*in_iter).end(); ++face_iter)
        {
            if(!pointInVector3D(*face_iter,vertices))
                vertices.push_back(*face_iter);
        }
    }

    return vertices;
}

std::vector<int> mslMesh::faceIndexing(const vertices3D& vertices,const simpleFace3D& face)
{
    std::vector<int> index;
    index.reserve(face.size());

    simpleFace3D::const_iterator face_iter = face.begin();

    for(; face_iter != face.end(); ++face_iter)
    {
        for(int i = 0; i < static_cast<int>(vertices.size()); i++)
        {
            if(*face_iter == vertices.at(i))
                index.push_back(i+1);
        }
    }

    return index;
}

mslMesh::mesh3D mslMesh::make_mesh3D(const VectorFaces3D& in_faces)
{
    mslMesh::mesh3D mesh;
    mesh.faces.reserve(in_faces.size());

    mesh.vertices = uniquePoints3D(in_faces);

    faceWithNormal3D<int> buffer;
    VectorFaces3D::const_iterator faces_iter = in_faces.begin();
    for(; faces_iter != in_faces.end(); ++faces_iter)
    {
        buffer.normal = calculationOfNormal(*faces_iter);
        buffer.face = faceIndexing(mesh.vertices,*faces_iter);

        mesh.faces.push_back(buffer);
    }

    return mesh;
}

void mslMesh::moveMesh(mslMesh::mesh3D& in_mesh, const point3Ddouble &newCenterPoint)
{
    point3Ddouble offset;
    offset = in_mesh.center();

    offset.first -= newCenterPoint.first;
    offset.second -= newCenterPoint.second;
    offset.third -= newCenterPoint.third;

    vertices3D::iterator vertice_iter = in_mesh.vertices.begin();
    for(; vertice_iter != in_mesh.vertices.end(); ++vertice_iter)
    {
        (*vertice_iter).first -= offset.first;
        (*vertice_iter).second -= offset.second;
        (*vertice_iter).third -= offset.third;
    }

    in_mesh.calculationOfNormal();
}

void mslMesh::scaleMesh(mslMesh::mesh3D& in_mesh,const mslMesh::point3Ddouble& scales)
{
    point3Ddouble center(in_mesh.center());
    mslMesh::moveMesh(in_mesh,make_point3D(double(),double(),double()));

    mslMesh::vertices3D::iterator vertices_iter = in_mesh.vertices.begin();
    for(; vertices_iter != in_mesh.vertices.end(); ++vertices_iter)
    {
        (*vertices_iter).first = (*vertices_iter).first * scales.first;
        (*vertices_iter).second = (*vertices_iter).second * scales.second;
        (*vertices_iter).third = (*vertices_iter).third * scales.third;
    }

    mslMesh::moveMesh(in_mesh,center);
}

void mslMesh::rotateMesh(mslMesh::mesh3D& in_mesh,const mslMesh::point3Ddouble& angles)
{
    point3Ddouble center(in_mesh.center());
    mslMesh::moveMesh(in_mesh,make_point3D(double(),double(),double()));

    mslMesh::vertices3D::iterator vertices_iter = in_mesh.vertices.begin();
    for(; vertices_iter != in_mesh.vertices.end(); ++vertices_iter)
    {
        rotatePoint((*vertices_iter),angles);
    }

    mslMesh::moveMesh(in_mesh,center);

    in_mesh.calculationOfNormal();
}

void mslMesh::rotateMeshInSpace(mslMesh::mesh3D& in_mesh,const mslMesh::point3Ddouble& angles)
{
    point3Ddouble center(in_mesh.center());

    mslMesh::vertices3D::iterator vertices_iter = in_mesh.vertices.begin();
    for(; vertices_iter != in_mesh.vertices.end(); ++vertices_iter)
    {
        rotatePoint((*vertices_iter),angles);
    }

    in_mesh.calculationOfNormal();
}

mslMesh::point3Ddouble mslMesh::minPoint3Delements(const vertices3D& in_vertices)
{
    point3Ddouble minPoint;

    vertices3D::const_iterator vertices_iter = in_vertices.begin();

    minPoint = (*vertices_iter);

    for(; vertices_iter != in_vertices.end(); ++vertices_iter)
    {
        if(minPoint.first > (*vertices_iter).first)
            minPoint.first = (*vertices_iter).first;
        if(minPoint.second > (*vertices_iter).second)
            minPoint.second = (*vertices_iter).second;
        if(minPoint.third > (*vertices_iter).third)
            minPoint.third = (*vertices_iter).third;
    }

    return minPoint;
}

mslMesh::point3Ddouble mslMesh::maxPoint3Delements(const vertices3D& in_vertices)
{
    point3Ddouble maxPoint;

    vertices3D::const_iterator vertices_iter = in_vertices.begin();

    maxPoint = (*vertices_iter);

    for(; vertices_iter != in_vertices.end(); ++vertices_iter)
    {
        if(maxPoint.first < (*vertices_iter).first)
            maxPoint.first = (*vertices_iter).first;
        if(maxPoint.second < (*vertices_iter).second)
            maxPoint.second = (*vertices_iter).second;
        if(maxPoint.third < (*vertices_iter).third)
            maxPoint.third = (*vertices_iter).third;
    }

    return maxPoint;
}

std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> mslMesh::cuboidAroundMesh(const mesh3D& in_mesh)
{
    return std::make_pair(maxPoint3Delements(in_mesh.vertices),minPoint3Delements(in_mesh.vertices));
}

std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> mslMesh::cubAroundMesh(const mesh3D& in_mesh)
{
    std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> cubPair(mslMesh::cuboidAroundMesh(in_mesh));

    double length = std::max(cubPair.first.first - cubPair.second.first,cubPair.first.second - cubPair.second.second);
    length = std::max(length,cubPair.first.third - cubPair.second.third);
    cubPair.second.first = cubPair.first.first - length;
    cubPair.second.second = cubPair.first.second - length;
    cubPair.second.third = cubPair.first.third - length;

    return cubPair;
}

double mslMesh::pointOnPlane(const simpleFace3D& in_face,const point3Ddouble& in_point)
{
    double a1,a2,b1,b2,c1,c2;
    a1 = (in_face.at(1).first - in_face.at(0).first);
    a2 = (in_face.at(2).first - in_face.at(0).first);
    b1 = (in_face.at(1).second - in_face.at(0).second);
    b2 = (in_face.at(2).second - in_face.at(0).second);
    c1 = (in_face.at(1).third - in_face.at(0).third);
    c2 = (in_face.at(2).third - in_face.at(0).third);

    return (b1*c2 - c1*b2)*in_point.first -
           (a1*c2 - c1*a2)*in_point.second +
           (a1*b2 - b1*a2)*in_point.third -
           (b1*c2 - c1*b2)*in_face.at(0).first +
           (a1*c2 - c1*a2)*in_face.at(0).second -
           (a1*b2 - b1*a2)*in_face.at(0).third;
}

double mslMesh::lengthOfUnit(const point3Ddouble & in_max, const point3Ddouble & in_min, const double in_lengthOfObject)
{
    double i;
    i = std::max((in_max.first - in_min.first),(in_max.second - in_min.second));
    i = std::max(i,(in_max.third - in_min.third));

    return i/in_lengthOfObject;
}

mslMesh::mesh3D mslMesh::visibleMesh(mslMesh::point3Ddouble& visionDirection,const mslMesh::mesh3D& mesh)
{
    std::vector<simpleFace3D> bufMesh;
    simpleFace3D bufFace;
    point3Ddouble bufferPoint;

    VectorIndexedFaces::const_iterator iter = mesh.faces.begin();
    for(; iter != mesh.faces.end(); iter++)
    {
        bufFace = mesh.getFace(iter);
        bufferPoint = calculationOfNormal(bufFace);

        if((visionDirection.first*bufferPoint.first + visionDirection.second*bufferPoint.second + visionDirection.third*bufferPoint.third) > 0.0)
        {
            bufMesh.push_back(bufFace);
        }
    }

    return make_mesh3D(bufMesh);
}
