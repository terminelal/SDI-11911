#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
void MyPolygon( Mat img )
{
    int lineType = 8;
    /* Create some points */
    Point rook_points[1][28];
    rook_points[0][0] = Point( 0, 480 );
    rook_points[0][1] = Point( 0, 468 );
    rook_points[0][2] = Point( 13, 455 );
    rook_points[0][3] = Point( 14, 450 );
    //rook_points[0][3] = Point( 115, 390 );
    //rook_points[0][] = Point( 80, 405 );

    //rook_points[0][4] = Point( 14, 455 );
    rook_points[0][4] = Point( 43, 427 );
    rook_points[0][5] = Point( 109, 390 );
    rook_points[0][6] = Point( 141, 381 );
    rook_points[0][7] = Point( 186, 375 );
    rook_points[0][8] = Point( 184, 360 );
    rook_points[0][9] = Point( 216, 360 );
    rook_points[0][10] = Point( 216, 343 );
    rook_points[0][11] = Point( 216, 343 );
    rook_points[0][12] = Point( 233, 343 );
    rook_points[0][13] = Point( 234, 357 );
    rook_points[0][14] = Point( 253, 357 );
    rook_points[0][15] = Point( 253, 357 );
    rook_points[0][16] = Point( 254, 369 );
    rook_points[0][17] = Point( 254, 369 );
    rook_points[0][18] = Point( 420, 370 );
    rook_points[0][19] = Point( 420, 370 );
    rook_points[0][20] = Point( 423, 350 );
    rook_points[0][21] = Point( 439, 359 );
    rook_points[0][22] = Point( 473, 357 );
    rook_points[0][23 ] = Point( 486, 376 );
    rook_points[0][24 ] = Point( 546, 383 );
    rook_points[0][25 ] = Point( 640, 423 );
    rook_points[0][26 ] = Point( 640, 480 );

    /*114  381
    rook_points[0][24 ] = Point( 640, 480 );
    
    rook_points[0][2] = Point( 640, 468 );
    rook_points[0][3] = Point( 640, 480 );

    rook_points[0][2] = Point( 3*640/4.0, 13*640/16.0 );
    rook_points[0][3] = Point( 11*640/16.0, 13*640/16.0 );
    rook_points[0][4] = Point( 19*640/32.0, 3*640/8.0 );
    rook_points[0][5] = Point( 3*640/4.0, 3*640/8.0 );
    rook_points[0][6] = Point( 3*640/4.0, 640/8.0 );
    rook_points[0][7] = Point( 26*640/40.0, 640/8.0 );
    rook_points[0][8] = Point( 26*640/40.0, 640/4.0 );
    rook_points[0][9] = Point( 22*640/40.0, 640/4.0 );
    rook_points[0][10] = Point( 22*640/40.0, 640/8.0 );
    rook_points[0][11] = Point( 18*640/40.0, 640/8.0 );
    rook_points[0][12] = Point( 18*640/40.0, 640/4.0 );
    rook_points[0][13] = Point( 14*640/40.0, 640/4.0 );
    rook_points[0][14] = Point( 14*640/40.0, 640/8.0 );
    rook_points[0][15] = Point( 640/4.0, 640/8.0 );
    rook_points[0][16] = Point( 640/4.0, 3*640/8.0 );
    rook_points[0][17] = Point( 13*640/32.0, 3*640/8.0 );
    rook_points[0][18] = Point( 5*640/16.0, 13*640/16.0 );
    rook_points[0][19] = Point( 640/4.0, 13*640/16.0) ;
    */
    const Point* ppt[1] = { rook_points[0] };
    int npt[] = { 27 };
    fillPoly( img,
              ppt,
              npt,
                  1,
              Scalar( 0, 255, 255 ),
              lineType );
    imshow("Display Image", img);
}


int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    //cv::split(img,ch);
     
    //get image only the range 0 to 85
    //cv::inRange(image,cv::Scalar(100,100, 100), cv::Scalar(255, 255, 255), image);
    
    //cv::inRange(ch[1],cv::Scalar(0, 100, 0), cv::Scalar(255, 200, 255), ch[1]);
    
    //cv::inRange(ch[0],cv::Scalar(0, 100, 0), cv::Scalar(255, 200, 255), ch[0]);
    
        //filters
    /*
    cv::Mat kernel;
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));

    cv::dilate(image, image, kernel);
    //cv::morphologyEx(image,image, cv::MORPH_OPEN, kernel);
    cv::erode(image, image, kernel);
    cv::erode(image, image, kernel);
    //cv::morphologyEx(ch[2], ch[2], cv::MORPH_OPEN, kernel);
    //cv::merge(ch,img);
    cv::threshold(image,image, 101, 255, CV_THRESH_BINARY_INV);
    */
    MyPolygon(image);
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}