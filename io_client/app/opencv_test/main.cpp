#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    if (argc < 1)
    {
        cerr << "Set filename of test image";
        return 1;
    }
    Mat src_gray;
    src_gray = imread(argv[1],0);
    blur( src_gray, src_gray, Size(3,3) );

    Mat bwimg = src_gray.clone();// > 127;
    vector<vector<Point> > contours;

    findContours( bwimg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
    unsigned int n = contours.size();
    cout << n << endl;

    for(unsigned int i=0;i<contours.size();i++)
    {
        approxPolyDP(Mat(contours[i]), contours[i], 10, true);

        if(i > 0)
        {
            cout << "Outer contour points \n";
        }
        else
            cout << "Inner contour points \n";

        //cout << contours[i].size();
        unsigned int nc = contours[i].size();
        cout << nc << endl;
        for(unsigned int j=0;j<contours[i].size();j++)
        {
            cout << "Point(x,y)=" << contours[i][j].x << "," << contours[i][j].y << endl;
            circle( src_gray, contours[i][j], 3, Scalar(0, 0, 255));//, FILLED, LINE_AA );
        }
        imshow( "Result", src_gray );
        waitKey(0);
    }

    return 0;
}
