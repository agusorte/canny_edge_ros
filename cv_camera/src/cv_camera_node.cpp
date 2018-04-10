#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cv_camera");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);
  //cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  //cv::waitKey(30);
  cv::VideoCapture cap(0); // open the default camera
  //sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

  if(!cap.isOpened())  // check if we succeeded
    return -1;

  ros::Rate loop_rate(5);
  while (nh.ok()) {

    cv::Mat frame;
    cap >> frame; // get a new frame from camera
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
    pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
