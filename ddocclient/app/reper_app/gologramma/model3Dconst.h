#ifndef MODEL3DCONST_H
#define MODEL3DCONST_H

#include <vector>
#include <cmath>

namespace mslMesh
{
    //Constants mslMesh
    const unsigned int version = 1;//Version mslMesh. Format: 000.000
    const int minVerticesForNormal = 3;//Minimum vertices numbers for calculation of normal
    const double pi = 3.14159265 / 180;//Number PI divided on 180

    class SinCosTable
    {
    public:
        SinCosTable()
        {
            for(int i = 0; i < 360; i++)
            {
                sinTable[i] = std::sin(i*pi);
                cosTable[i] = std::cos(i*pi);
            }
        }

        double sin(int angle) const
        {
            if(angle < 0)
            {
                angle = 360 + angle;
            }

            return sinTable[angle];
        }

        double cos(int angle) const
        {
            if(angle < 0)
            {
                angle = 360 + angle;
            }

            return cosTable[angle];
        }

    private:
        double cosTable[360];
        double sinTable[360];
    };

    const SinCosTable sincos_table;

    inline bool versionCompare(unsigned int&);

    //Point in 3D
    template <class T1,class T2,class T3>
    struct point3D
    {
        T1 first;
        T2 second;
        T3 third;

        point3D() : first(T1()),second(T2()),third(T3()) {}
        point3D(const T1& x,const T2& y,const T3& z) : first(x),second(y),third(z) {}

        template<class X,class Y,class Z>
        point3D(const point3D<X,Y,Z> &p):
            first(p.first),second(p.second),third(p.third) {}
    };

    template <class T1,class T2,class T3>
    bool operator==(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);
    template <class T1,class T2,class T3>
    bool operator!=(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);

    //Результат операции в общем случае не определен! Реализации нет.
    template <class T1,class T2,class T3>
    bool operator<(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T1,class T2,class T3>
    bool operator<=(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T1,class T2,class T3>
    bool operator>(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T1,class T2,class T3>
    bool operator>=(const point3D<T1,T2,T3>&,const point3D<T1,T2,T3>&);

    template<class T1,class T2,class T3>
    point3D<T1,T2,T3> make_point3D(const T1&,const T2&,const T3&);

    typedef point3D<double,double,double> point3Ddouble;
    typedef std::vector<point3Ddouble> simpleFace3D;
    typedef std::vector<point3Ddouble> vertices3D;
    typedef std::vector<simpleFace3D> VectorFaces3D;

    void rotatePoint(point3Ddouble& in_point,const point3Ddouble& in_angle);

    template<class T>
    struct faceWithNormal3D
    {
        std::vector<T> face;
        point3Ddouble normal;
    };

    typedef std::vector<faceWithNormal3D<int> > VectorIndexedFaces;

    template <class T>
    bool operator==(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);
    template <class T>
    bool operator!=(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);

    //Результат операции в общем случае не определен! Реализации нет.
    template <class T>
    bool operator<(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T>
    bool operator<=(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T>
    bool operator>(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);
    //Результат операции в общем случае не определен! Реализации нет.
    template <class T>
    bool operator>=(const faceWithNormal3D<T>&,const faceWithNormal3D<T>&);

    point3Ddouble calculationOfNormal(const simpleFace3D&);
    inline bool normalExistenceCondicion(const simpleFace3D&);

    struct mesh3D
    {
        VectorIndexedFaces faces;
        vertices3D vertices;

        point3Ddouble center() const;
        void calculationOfNormal();
        simpleFace3D getFace(const VectorIndexedFaces::const_iterator&) const;
    };

    bool pointInVector3D(const point3Ddouble&,const vertices3D&);

    vertices3D uniquePoints3D(const VectorFaces3D&);

    std::vector<int> faceIndexing(const vertices3D&,const simpleFace3D&);

    mesh3D make_mesh3D(const VectorFaces3D&);

    void moveMesh(mesh3D&,const point3Ddouble&);
    void scaleMesh(mesh3D&,const point3Ddouble&);
    void rotateMesh(mesh3D&,const point3Ddouble&);
    void rotateMeshInSpace(mesh3D&,const point3Ddouble&);

    point3Ddouble minPoint3Delements(const vertices3D&);
    point3Ddouble maxPoint3Delements(const vertices3D&);

    std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> cuboidAroundMesh(const mesh3D&);
    std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> cubAroundMesh(const mesh3D&);

    double pointOnPlane(const simpleFace3D&,const point3Ddouble&);

    double lengthOfUnit(const point3Ddouble&,const point3Ddouble&,const double);

    mesh3D visibleMesh(point3Ddouble &visionDirection, const mesh3D &mesh);
}

#endif // MODEL3DCONST_H
