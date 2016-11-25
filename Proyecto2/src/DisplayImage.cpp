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
    rook_points[0][4] = Point( 43, 420 );
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
    rook_points[0][18] = Point( 410, 370 );
    rook_points[0][19] = Point( 410, 370 );
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
              Scalar( 0, 0, 0 ),
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
     cv::rectangle( image, cv::Point( 0, 0 ),cv::Point( 640, 200),cv::Scalar(0, 0, 0 ),CV_FILLED,
CV_AA );/*
     cv::threshold(image,image, 101, 255, CV_THRESH_BINARY);

    cv::Mat kernel;
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    //cv::dilate(image, image, kernel);
    //cv::morphologyEx(image,image, cv::MORPH_OPEN, kernel);
    cv::erode(image, image, kernel);
cv::erode(image, image, kernel);
cv::erode(image, image, kernel);
    cv::dilate(image, image, kernel);
    // Canny algorithm
    cv::dilate(image, image, kernel);
     
    Mat contours, cdst;
    Canny(image,contours,50,350);
    Mat contoursInv;
    int houghVote;
    double PI = 3.1415;
    //threshold(contours,contoursInv,128,255,THRESH_BINARY_INV);

    //std::vector<Vec2f> lines;
    vector<Vec4i> lines;

  HoughLinesP(contours, lines, 1, CV_PI/180, 10, 5, 3 );
  cvtColor(contours, cdst, CV_GRAY2BGR);
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    double m = ((double)l[3]-(double)l[1])/((double)l[2]-(double)l[0]); //  y2-y1/x2-x1
    if(m>-.3 && m<.3)
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }

//Below is the result of the Hough transform.

namedWindow("Display Image1", WINDOW_AUTOSIZE );
    imshow("Display Image1", contours);


    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", cdst);
*/
    //waitKey(0);
   Point2f inputQuad[4]; 
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
         
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
    Mat input, output;
     
    //Load the image
    input = image;//imread( argv[1], 1 );
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );
 
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input 
    inputQuad[0] = Point2f( 0,200 );
    inputQuad[1] = Point2f( 640 ,200);
    inputQuad[2] = Point2f( 640,348);
    inputQuad[3] = Point2f( 75,348  );  
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = Point2f( 0,input.rows-1  );
 
    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,output.size() );
 
    //Display input and output
    imshow("Input",input);
    imshow("Output",output);
 
    waitKey(0);
    return 0;
    return 0;
}
/*
 // Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4]; 
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
         
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
    Mat input, output;
     
    //Load the image
    input = imread( "lena.jpg", 1 );
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );
 
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input 
    inputQuad[0] = Point2f( -30,-60 );
    inputQuad[1] = Point2f( input.cols+50,-50);
    inputQuad[2] = Point2f( input.cols+100,input.rows+50);
    inputQuad[3] = Point2f( -50,input.rows+50  );  
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = Point2f( 0,input.rows-1  );
 
    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,output.size() );
 
    //Display input and output
    imshow("Input",input);
    imshow("Output",output);
 
    waitKey(0);
    return 0;
*/