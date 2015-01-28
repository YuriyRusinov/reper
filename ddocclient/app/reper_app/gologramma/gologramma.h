#ifndef GOLOGRAMMA_H
#define GOLOGRAMMA_H

#include "gologramma_global.h"
#include "cuboid.h"

#include <QProgressDialog>

//programm
void _GOL_EXPORT loadModel(mslLoader::OBJloader& loader,const std::string& str);

struct _GOL_EXPORT cubPair
{
    Cuboid initialCub;
    std::vector<Cuboid> cubs;
};

cubPair _GOL_EXPORT buildCub(const mslLoader::OBJloader& loader,const double lengthOfShip,const double numberOfUnit);

struct _GOL_EXPORT constDataStruct
{
    unsigned int XY_plane;
    unsigned int XZ_plane;
    unsigned int imageSize;
    unsigned int numberOfUnit;
};

std::string _GOL_EXPORT createFileNameDAT(const constDataStruct& data,const std::string& folder);
std::string _GOL_EXPORT createFileNamePNG(const constDataStruct& data,const std::string& folder);

void _GOL_EXPORT saveFileDAT(constDataStruct data,std::vector<unsigned char>& image, const std::string& str);
void _GOL_EXPORT saveFilePNG(std::vector<unsigned char>& image, const std::string& str);

struct _GOL_EXPORT generatingData
{
    double lengthOfShip;
    double numberOfUnit;

    double XY_angleMax;
    double XY_angleMin;
    double XY_angleStep;

    double XZ_angleMax;
    double XZ_angleMin;
    double XZ_angleStep;
};

struct _GOL_EXPORT returningData
{
    unsigned int XY_angle;
    unsigned int XZ_angle;
    unsigned int rowNumber;
    unsigned int columnNumber;
    unsigned int numberOfUnit;

    QVector<unsigned char> data;
};

void _GOL_EXPORT swap_STDtoQT_vector(QVector<unsigned char>& lhs,std::vector<unsigned char>& rhs);

struct _GOL_EXPORT generatingDataPlus
{
    QString filename;
    generatingData data;
};

QVector<returningData> _GOL_EXPORT generateImages(const generatingData &data,mslLoader::OBJloader &loader,const std::string &folder);

class _GOL_EXPORT ImageGenerator: public QObject
{
    Q_OBJECT

public:
    explicit ImageGenerator(const generatingDataPlus&,QObject* parent = 0);

    void loadModel();
    QVector<returningData> generateImages();

private:
    generatingDataPlus imageData;
    mslLoader::OBJloader loader;

signals:
    void startCreating();
    void createOneImage(int value);
    void createAllImages();
};

#endif // GOLOGRAMMA_H
