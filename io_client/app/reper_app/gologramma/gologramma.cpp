#include "gologramma.h"

void loadModel(mslLoader::OBJloader& loader,const std::string& str)
{
    loader.load(str);
}

cubPair buildCub(const mslLoader::OBJloader& loader,const double lengthOfShip,const double numberOfUnit)
{
    std::pair<mslMesh::point3Ddouble,mslMesh::point3Ddouble> cubusPair = cubAroundMesh(loader.getMesh());
    Cuboid cubus(cubusPair.first,cubusPair.second);
    cubus.move(loader.getMesh().center());

    double length = lengthOfUnit(cubusPair.first,cubusPair.second,lengthOfShip);
    cubus.toMaxPow2(length*numberOfUnit);
    std::vector<Cuboid> cubusCubus = cubus.divideCub(length*numberOfUnit);

    cubPair retCubs;
    retCubs.initialCub = cubus;
    retCubs.cubs = cubusCubus;

    return retCubs;
}

std::string createFileNameDAT(const constDataStruct &data, const std::string& folder)
{
    std::string name = folder;
    std::stringstream ss;

    name += "ship_image";
    ss << "_" << data.XY_plane << "_" << data.XZ_plane;
    name += ss.str();
    name += ".dat";

    return name;
}

std::string createFileNamePNG(const constDataStruct &data, const std::string& folder)
{
    std::string name = folder;
    std::stringstream ss;

    name += "ship_image";
    ss << "_" << data.XY_plane << "_" << data.XZ_plane;
    name += ss.str();
    name += ".png";

    return name;
}

void saveFileDAT(constDataStruct data, std::vector<unsigned char> &image, const std::string& str)
{
    unsigned int bufAngle;
    char* buf;

    std::ofstream outfile(str.c_str(),std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    bufAngle = static_cast<unsigned int>(data.XY_plane);
    buf = (char*)&bufAngle;
    outfile.write(buf,sizeof(bufAngle));
    bufAngle = static_cast<unsigned int>(data.XZ_plane);
    buf = (char*)&bufAngle;
    outfile.write(buf,sizeof(bufAngle));
    bufAngle = data.imageSize;
    buf = (char*)&bufAngle;
    outfile.write(buf,sizeof(bufAngle));
    outfile.write(buf,sizeof(bufAngle));
    bufAngle = static_cast<unsigned int>(data.numberOfUnit);
    buf = (char*)&bufAngle;
    outfile.write(buf,sizeof(bufAngle));

    unsigned char bufImage;
    for(unsigned int i = 0; i < image.size(); i++)
    {
        bufImage = image.at(i);
        buf = (char*)&bufImage;
        outfile.write(buf,sizeof(unsigned char));
    }

    outfile.close();
}

void saveFilePNG(std::vector<unsigned char> &image, const std::string& str)
{
    imageTransform(image,str);
}

void swap_STDtoQT_vector(QVector<unsigned char>& lhs,std::vector<unsigned char>& rhs)
{
    for(std::vector<unsigned char>::iterator iter = rhs.begin(); iter != rhs.end(); ++iter)
    {
        lhs.push_back(*iter);
    }
}

QVector<returningData> generateImages(const generatingData &data, mslLoader::OBJloader &loader, const std::string &folder)
{
    cubPair cubs = buildCub(loader,data.lengthOfShip,data.numberOfUnit);
    std::vector<Cuboid> cubusCubus;
    Cuboid cubBig;

    QVector<returningData> imagesData;

    for(double XY_plane = data.XY_angleMin; XY_plane < data.XY_angleMax; XY_plane += data.XY_angleStep)
    {
        for(double XZ_plane = data.XZ_angleMin; XZ_plane <= data.XZ_angleMax; XZ_plane += data.XZ_angleStep)
        {
            cubusCubus = cubs.cubs;
            cubBig = cubs.initialCub;

            cubBig.rotateInSpace(mslMesh::make_point3D(XZ_plane,XY_plane,0.0));

            std::vector<Cuboid>::iterator cubusCubusIter = cubusCubus.begin();
            for(; cubusCubusIter != cubusCubus.end(); ++cubusCubusIter)
            {
                (*cubusCubusIter).rotateInSpace(mslMesh::make_point3D(XZ_plane,XY_plane,0.0));
            }

            mslMesh::point3Ddouble bufPoint = cubBig.getVisionDirection();
            mslMesh::mesh3D bufMesh = mslMesh::visibleMesh(bufPoint,loader.getMesh());

            std::vector<unsigned char> cIm = createImageMatrix(cubusCubus,bufMesh);
            std::vector<unsigned char> image = createImage(cIm);

            unsigned int imageSize = static_cast<unsigned int>(sqrt(static_cast<double>(image.size())));

            returningData buf;
            buf.XY_angle = static_cast<unsigned int>(XY_plane);
            buf.XZ_angle = static_cast<unsigned int>(XZ_plane);
            buf.rowNumber = buf.columnNumber = imageSize;
            buf.numberOfUnit = data.numberOfUnit;
            swap_STDtoQT_vector(buf.data,image);

//****

            //constDataStruct bufDat;
            //bufDat.XY_plane = buf.XY_angle;
            //bufDat.XZ_plane = buf.XZ_angle;
            //bufDat.imageSize = imageSize;
            //bufDat.numberOfUnit = data.numberOfUnit;

            //saveFilePNG(image,createFileNamePNG(bufDat,"c:/image/pix/"));

//****

            imagesData.push_back(buf);
        }
    }

    return imagesData;
}

ImageGenerator::ImageGenerator(const generatingDataPlus& data,QObject* parent):
    QObject(parent),imageData(data)
{
}

void ImageGenerator::loadModel()
{
    loader.load(imageData.filename.toStdString());
}

QVector<returningData> ImageGenerator::generateImages()
{
    int imagesNum = ((imageData.data.XY_angleMax - imageData.data.XY_angleMin)/imageData.data.XY_angleStep) *
            ((imageData.data.XZ_angleMax - imageData.data.XZ_angleMin)/imageData.data.XZ_angleStep - 1);
	
    QProgressDialog * progressD = new QProgressDialog (tr("Creating images..."), tr("&Cancel"), 0, imagesNum);
//    progressD->setWindowModality(Qt::WindowModal);
    connect (progressD, SIGNAL (canceled()), this, SLOT (prCancel()));
    progressD->show();
	
    if (progressD->wasCanceled())
    {
        delete progressD;
        return QVector<returningData>();//imagesData;
    }
    cubPair cubs = buildCub(loader,imageData.data.lengthOfShip,imageData.data.numberOfUnit);
    std::vector<Cuboid> cubusCubus;
    Cuboid cubBig;

    QVector<returningData> imagesData;

    int value = 0;
    if (progressD->wasCanceled())
    {
        delete progressD;
        return imagesData;
    }
    for(double XY_plane = imageData.data.XY_angleMin; XY_plane < imageData.data.XY_angleMax; XY_plane += imageData.data.XY_angleStep)
    {
        //progressD->update();
		
        for(double XZ_plane = imageData.data.XZ_angleMin; XZ_plane <= imageData.data.XZ_angleMax; XZ_plane += imageData.data.XZ_angleStep)
        {
            value++;
            progressD->setValue(value);

            if (progressD->wasCanceled())
            {
                delete progressD;
                return imagesData;
            }

            cubusCubus = cubs.cubs;
            cubBig = cubs.initialCub;

            cubBig.rotateInSpace(mslMesh::make_point3D(XZ_plane,XY_plane,0.0));
            if (progressD->wasCanceled())
            {
                delete progressD;
                return imagesData;
            }

            std::vector<Cuboid>::iterator cubusCubusIter = cubusCubus.begin();
            for(; cubusCubusIter != cubusCubus.end(); ++cubusCubusIter)
            {
                cubusCubusIter->rotateInSpace(mslMesh::make_point3D(XZ_plane,XY_plane,0.0));
                if (progressD->wasCanceled())
                {
                    delete progressD;
                    return imagesData;
                }
            }

            mslMesh::point3Ddouble bufPoint = cubBig.getVisionDirection();
            mslMesh::mesh3D bufMesh = mslMesh::visibleMesh(bufPoint,loader.getMesh());
            if (progressD->wasCanceled())
            {
                delete progressD;
                return imagesData;
            }
            std::vector<unsigned char> cIm = createImageMatrix(cubusCubus,bufMesh, progressD);
            std::vector<unsigned char> image = createImage(cIm);
            if (progressD->wasCanceled())
            {
                delete progressD;
                return imagesData;
            }

            unsigned int imageSize = static_cast<unsigned int>(sqrt(static_cast<double>(image.size())));

            returningData buf;
            buf.XY_angle = static_cast<unsigned int>(XY_plane);
            buf.XZ_angle = static_cast<unsigned int>(XZ_plane);
            buf.rowNumber = buf.columnNumber = imageSize;
            buf.numberOfUnit = imageData.data.numberOfUnit;
            swap_STDtoQT_vector(buf.data,image);

//****

            //constDataStruct bufDat;
            //bufDat.XY_plane = buf.XY_angle;
            //bufDat.XZ_plane = buf.XZ_angle;
            //bufDat.imageSize = imageSize;
            //bufDat.numberOfUnit = imageData.data.numberOfUnit;

            //saveFilePNG(image,createFileNamePNG(bufDat,"c:/image/pix/"));

//****

            imagesData.push_back(buf);

			//value++;
        }
    }

    progressD->setValue(imagesNum);
    delete progressD;

    return imagesData;
}

void ImageGenerator::prCancel (void)
{
    deleteLater ();
}
