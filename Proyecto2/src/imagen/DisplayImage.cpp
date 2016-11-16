#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>
#include <string>
#include <stdio.h>

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
	 cv::rectangle( Result, cv::Point( 0, 0 ),cv::Point( 640, 200),cv::Scalar(0, 0, 0 ),CV_FILLED,
         CV_AA );	  
	  
	  // threshold para quedarse solo con lo blanco
	  cv::Mat dstthreshold, erosion_dst;
  	  cv::inRange(Result,cv::Scalar(160,160,160), cv::Scalar(255,255,255),dstthreshold);
	  
	  // erosion
	  int erosion_size = 1;
	  // tipos de erosion: MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
	  int erosion_type = cv::MORPH_RECT;
	  cv::Mat element = cv::getStructuringElement( erosion_type,
                       cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       cv::Point( erosion_size, erosion_size ) );
  	  
	cv::erode( dstthreshold, erosion_dst, element );
	  cv::namedWindow("Erosion Demo" );
  	  cv::imshow( "Erosion Demo", erosion_dst );
	
	cv::Mat histImage = histograma(Result);
	  // mostrar imagenes
	  cv::namedWindow("calcHist Demo" );
	  cv::imshow("calcHist Demo", histImage );
		
	  cv::namedWindow("threshold Demo" );
	  cv::imshow("threshold Demo", dstthreshold );

	  cv::namedWindow("Mascara Demo" );
	  cv::imshow("Mascara Demo", Result );
/*
   http://docs.opencv.org/trunk/d9/db0/tutorial_hough_lines.html
	    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
#if 0
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
#else
    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
#endif
    imshow("source", src);
    imshow("detected lines", cdst);
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
	
	ros::Subscriber sub = nh.subscribe<sensor_msgs::Image>("/app/camera/rgb/image_raw", 1, imageCallback);
	ros::spin();
	cv::destroyWindow("view");

}
