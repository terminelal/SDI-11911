#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <geometry_msgs/Twist.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

std::vector<cv::Point> old;
ros::Publisher pub_pose;
double rate_hz = 5;

void dibujarPoligonoCarro( cv::Mat img )
{
    int lineType = 8;
    /* Create some points */
    cv::Point rook_points[1][28];
    rook_points[0][0] = cv::Point( 0, 480 );
    rook_points[0][1] = cv::Point( 0, 468 );
    rook_points[0][2] = cv::Point( 13, 455 );
    rook_points[0][3] = cv::Point( 14, 450 );
    rook_points[0][4] = cv::Point( 43, 420 );
    rook_points[0][5] = cv::Point( 109, 390 );
    rook_points[0][6] = cv::Point( 141, 381 );
    rook_points[0][7] = cv::Point( 186, 375 );
    rook_points[0][8] = cv::Point( 184, 360 );
    rook_points[0][9] = cv::Point( 216, 360 );
    rook_points[0][10] = cv::Point( 216, 343 );
    rook_points[0][11] = cv::Point( 216, 343 );
    rook_points[0][12] = cv::Point( 233, 343 );
    rook_points[0][13] = cv::Point( 234, 357 );
    rook_points[0][14] = cv::Point( 253, 357 );
    rook_points[0][15] = cv::Point( 253, 357 );
    rook_points[0][16] = cv::Point( 254, 369 );
    rook_points[0][17] = cv::Point( 254, 369 );
    rook_points[0][18] = cv::Point( 410, 370 );
    rook_points[0][19] = cv::Point( 410, 370 );
    rook_points[0][20] = cv::Point( 423, 350 );
    rook_points[0][21] = cv::Point( 439, 359 );
    rook_points[0][22] = cv::Point( 473, 357 );
    rook_points[0][23 ] = cv::Point( 486, 376 );
    rook_points[0][24 ] = cv::Point( 546, 383 );
    rook_points[0][25 ] = cv::Point( 640, 423 );
    rook_points[0][26 ] = cv::Point( 640, 480 );
    const cv::Point* ppt[1] = { rook_points[0] };
    int npt[] = { 27 };
    cv::fillPoly( img,
              ppt,
              npt,
                  1,
              cv::Scalar( 0, 0, 0 ),
              lineType );
}

template<typename T> 
inline std::string toString( const T& ao_Obj )
{
std::stringstream lo_stream;

lo_stream << ao_Obj;

return lo_stream.str();
}

cv::Mat histograma(cv::Mat src)
{
	std::vector<cv::Mat> bgr_planes;
          split( src, bgr_planes );

	  int histSize = 256;
	  float range[] = { 0, 256 } ;
	  const float* histRange = { range };
	  bool uniform = true; bool accumulate = false;
	  cv::Mat b_hist, g_hist, r_hist;
	  cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	  cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	  cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
	  // Draw the histograms for B, G and R
	  int hist_w = 640; int hist_h = 480;
	  int bin_w = cvRound( (double) hist_w/histSize );
	  cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
	  cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
	  cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
	  cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
	  for( int i = 1; i < histSize; i++ )
	  {
	      cv::line(histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
		               cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
		               cv::Scalar( 255, 0, 0), 2, 8, 0  );
	      cv::line(histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
		               cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
		               cv::Scalar( 0, 255, 0), 2, 8, 0  );
	      cv::line(histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
		               cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
		               cv::Scalar( 0, 0, 255), 2, 8, 0  );
	  }
	return histImage;
}


cv::Mat puntoAMoverse(cv::Mat& entrada){
	// threshold para quedarse solo con lo blanco
	 cv::Mat dstthreshold, dilate_dst, erosion_dst;
  	 cv::inRange(entrada,cv::Scalar(160,160,160), cv::Scalar(255,255,255),dstthreshold);

	 // erosion
	 int erosion_size = 1; //20;
	 // tipos de erosion: MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
	 int erosion_type = cv::MORPH_RECT;
	 cv::Mat element = cv::getStructuringElement( erosion_type,
                       cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       cv::Point( erosion_size, erosion_size ) );
  	  
	 // eliminar ruido (revisar)
	 cv::erode( dstthreshold, erosion_dst, element );
	 cv::dilate( erosion_dst, dilate_dst, element );
	 
	 // mostrar histograma
	 // cv::Mat histImage = histograma(Result); // crear histograma de la imagen
	 
	 cv::Mat contours, cdst;
	 cv::Canny(dilate_dst,contours,50,350);
	 cv::Mat contoursInv;
	 int houghVote;
	 cv::vector<cv::Vec4i> lines;


	 cv::HoughLinesP(contours, lines, 1, CV_PI/180, 10, 5, 3 );
	 cvtColor(contours, cdst, CV_GRAY2BGR);
	 for( size_t i = 0; i < lines.size(); i++ )
	 {
	    cv::Vec4i l = lines[i];
	    double m = ((double)l[3]-(double)l[1])/((double)l[2]-(double)l[0]); //  y2-y1/x2-x1
	    // if(abs(m)>0.2)
	    cv::line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,50), 2, CV_AA);	     
	 }
	 
	 std::vector<std::vector<cv::Point> > c_contours;
	 std::vector<cv::Vec4i> hierarchy;
	 cv::findContours(contours, c_contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	double centrox = 320;
	double centroy = 0;

	double maxDX = 0.0;
	double maxDY = 0.0;

	double maxIX = 0.0;
	double maxIY = 0.0;

	int sized=0, sizei=0;

	double xf = 0.0;
	double yf = 0.0;
	for( size_t i = 0; i < c_contours.size(); i++ )
	  {
 		 std::vector<cv::Point> o = c_contours[i];
		double x =0.0;
		double y =0.0;
		for( size_t j = 0; j < o.size(); j++ )
	  	{
			x+=o[j].x;
			y+=o[j].y;
		}
		x=x/o.size();
		y=y/o.size();
		cv::line(cdst, cv::Point(x, y), cv::Point(x, y), cv::Scalar(0,i*30,255), 3, CV_AA);

		if(x>centrox){
			maxDX+=x;
			maxDY+=y;
sized++;
		}
		else{
			maxIX+=x;
maxIY+=y;
sizei++;
		}
		xf+=x;
		yf+=y;
	  }
	xf=(((maxDX/sized)+(maxIX/sizei))/2);
	yf=(((maxDY/sized)+(maxIY/sizei))/2); //pixeles, falta distancia

	// mostrar el punto en la imagen
	cv::line(cdst, cv::Point(xf, yf), cv::Point(xf, yf), cv::Scalar(100,30,255), 10, CV_AA);
	
	double xreal = (xf - 320)/100;
	double yreal = (480 - yf)/100;
	double angulo = atan(yreal/xreal) * 180/PI;

	// aqui se tiene que enviar la posicion con respecto al robot del punto	
	/*
	geometry_msgs::Twist desired_pose;	
	desired_pose.linear.x = xreal;
	desired_pose.linear.y = yreal;
	desired_pose.angular.z = angulo;
	pub_pose.publish(desired_pose);
	*/
	
	printf("\nPunto a moverse (imagen): (%f,%f)", xf,yf);
	printf("\nPunto a moverse (procesado): x: %f, angulo: %f", xreal, yreal);

	return cdst;
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	
	//cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
	// cv::Mat _img = cv_bridge::toCvShare(msg , "bgr8")->image;
	// cv::waitKey(30);
	// cv_bridge::CvImagePtr cv_ptr;

        try
        {
         // cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	  
         // histograma de la imagen
	 cv::Mat src = cv_bridge::toCvShare(msg , "bgr8")->image;
	 cv::Mat Result = cv_bridge::toCvShare(msg , "bgr8")->image;
	 
	 // crea un rectangulo en la parte superior
	 // cv::rectangle( Result, cv::Point( 0, 0 ),cv::Point( 640, 200),cv::Scalar(0, 0, 0 ),CV_FILLED,
         // CV_AA );
	 
	 dibujarPoligonoCarro(Result);
	 
	 
	 


    cv::Point2f inputQuad[4]; 
    // Output Quadilateral or World plane coordinates
    cv::Point2f outputQuad[4];
         
    // Lambda Matrix
    cv::Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
    cv::Mat input, output;
     
    //Load the image
    input = Result;//imread( argv[1], 1 );
    // Set the lambda matrix the same type and size as input
    lambda = cv::Mat::zeros( input.rows, input.cols, input.type() );
 
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input 
    inputQuad[0] = cv::Point2f( 0,200 );
    inputQuad[1] = cv::Point2f( 640 ,200);
    inputQuad[2] = cv::Point2f( 640,400);
    inputQuad[3] = cv::Point2f( 0,400  );  
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = cv::Point2f( 0,0 );
    outputQuad[1] = cv::Point2f( input.cols-1,0);
    outputQuad[2] = cv::Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = cv::Point2f( 0,input.rows-1  );
 
    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = cv::getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    cv::warpPerspective(input,output,lambda,output.size() );

	printf("\nPunto Result: ");
	cv::Mat cdst = puntoAMoverse(Result);
	
	printf("\nPunto output: ");
	cv::Mat cdst2 = puntoAMoverse(output);

	 
	
	cv::namedWindow("Result" );
  	cv::imshow( "Result", cdst);

	cv::namedWindow("Result 2" );
  	cv::imshow( "Result 2", cdst2);

	cv::namedWindow("Original" );
	cv::imshow("Original", src );

	/*
	   http://docs.opencv.org/trunk/d9/db0/tutorial_hough_lines.html
	*/


        }

        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
	
	// para guardar la imagen
	/* int key =cv::waitKey(0);	
	if ( key == 32 ) // espacio
	{
	    static int count = 0;
	    ROS_ASSERT(cv::imwrite( std::string( "file" )  + toString(count++) + std::string( ".png" ), cv_ptr->image ));
	}
	*/
}




int main(int argc, char** argv )
{
	ros::init(argc, argv, "image_node");
  	ros::NodeHandle nh;
  	ros::Rate loop_rate(1);

	cv::startWindowThread();
	
	ros::Subscriber sub = nh.subscribe<sensor_msgs::Image>("/app/camera/rgb/image_raw", 10, imageCallback);

	pub_pose = nh.advertise<geometry_msgs::Twist>("/target_pose", rate_hz);

	ros::spin();
	cv::destroyWindow("view");

}
