#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
cv::Mat imgOld;

cv::Mat src;
cv::Mat result;
bool bol=false;
//int intImgRecord=0;


std::vector<cv::Point> old;


void dibujarPoligonoCofre( cv::Mat img )
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


template<typename T>  inline std::string toString( const T& ao_Obj ){
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
	try
	{
		//read image
		src =  cv_bridge::toCvShare(msg, "bgr8")->image;
		result =  cv_bridge::toCvShare(msg, "bgr8")->image;
		/* Save images
		char str[10];
		sprintf(str, "str%d.png", intImgRecord);
		intImgRecord++;
		cv::imwrite(str,image);
		*/
		//cofre filter
		dibujarPoligonoCofre(src);
		//horizont filter
		cv::rectangle( src, cv::Point( 0, 0 ),cv::Point( 640, 250),cv::Scalar(0, 0, 0 ),CV_FILLED, CV_AA );
		//gray filter
		cv::inRange(src,cv::Scalar(100,100, 100), cv::Scalar(255, 255, 255), src);
		cv::threshold(src,src, 101, 255, CV_THRESH_BINARY);
		//morphological filter
		cv::Mat kernel;
		kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		//cv::dilate(image, image, kernel);
		//cv::morphologyEx(image,image, cv::MORPH_OPEN, kernel);
		cv::erode(src, src, kernel);
		cv::erode(src, src, kernel);
		cv::erode(src, src, kernel);
		cv::dilate(src, src, kernel);
		// Canny algorithm
		cv::dilate(src, src, kernel);
		cv::Mat contours, cdst;
		Canny(src,contours,50,350);
		cv::Mat contoursInv;
		int houghVote;
		double PI = 3.1415;
		//threshold(contours,contoursInv,128,255,THRESH_BINARY_INV);
		//std::vector<Vec2f> lines;
		cv::vector<cv::Vec4i> lines;
		HoughLinesP(contours, lines, 1, CV_PI/180, 10, 5, 3 );
		cvtColor(contours, cdst, CV_GRAY2BGR);
		for( size_t i = 0; i < lines.size(); i++ ){
			cv::Vec4i l = lines[i];
			double m = ((double)l[3]-(double)l[1])/((double)l[2]-(double)l[0]); //  y2-y1/x2-x1
			if(m>-.3 && m<.3)
				line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
		}
		//Below is the result of the Hough transform.
		//namedWindow("Display Image1", WINDOW_AUTOSIZE );
		//    imshow("Display Image1", contours);
		//namedWindow("Display Image", WINDOW_AUTOSIZE );
		//imshow("Display Image", cdst);
		cv::imshow("view", cdst);
		cv::imshow("view2", result);
		//waitKey(0);
	}catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
		bol =false;
	}
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::namedWindow("view2");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/app/camera/rgb/image_raw", 1, imageCallback);

  if (!bol)
  {
      bol =true;
      //imgOld =img;
      //split image in layer (channels

     /*code */
  }
  ros::spin();

  //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  //cv::waitKey(30);
  cv::destroyWindow("view");
  cv::destroyWindow("view2");
}