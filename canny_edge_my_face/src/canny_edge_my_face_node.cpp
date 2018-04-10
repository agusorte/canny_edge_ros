#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

/*
 * canny_edge_my_face_node.cpp
 *
 *  Created on: April, 2018
 *  Author: Agustin Ortega 
 *  Email: aortega.jim@gmail.com
 */

image_transport::Publisher pub;
namespace enc = sensor_msgs::image_encodings;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{


  cv_bridge::CvImageConstPtr cv_ptr;

  try{
   
   if (enc::isColor(msg->encoding)) 
     cv_ptr = cv_bridge::toCvShare(msg, enc::BGR8);
   else
     cv_ptr = cv_bridge::toCvShare(msg, enc::MONO8);
  }
  catch (cv_bridge::Exception& e){
   
   ROS_ERROR("cv_bridge exception: %s", e.what());
   
   return ;
   
  }

  //imCurr = cv_ptr->image;
  cv::Mat im = cv_ptr->image;

  cv::Mat im_gray,im_edges, dst;
  cv::cvtColor( im, im_gray, CV_BGR2GRAY );

  // canny here
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 2;
  int kernel_size = 3;

   /// Reduce noise with a kernel 3x3
  blur( im_gray, im_edges, cv::Size(3,3) );

  /// Canny detector
  Canny( im_edges, im_edges, lowThreshold, lowThreshold*ratio, kernel_size );


  im.copyTo( dst, im_edges);
  cv::cvtColor(im_edges, dst, cv::COLOR_GRAY2BGR);
 

  sensor_msgs::ImagePtr msg_out = cv_bridge::CvImage(std_msgs::Header(), "bgr8", dst).toImageMsg();
  pub.publish(msg_out);

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "canny_edge_my_face_node");
  ros::NodeHandle nh;
  // cv::namedWindow("view");
  // cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  pub = it.advertise("camera/image_canny", 1);
  ros::spin();
  // cv::destroyWindow("view");
}
