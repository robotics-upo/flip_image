#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

image_transport::Publisher pub;

int flip_type = -1;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	try
	{
		// Flip input image
		cv_bridge::CvImage flipImage;    
		if(flip_type == 2) {
			cv::transpose(cv_bridge::toCvShare(msg, "")->image,flipImage.image );

			cv::flip(flipImage.image, flipImage.image, flip_type);     // -1 means horizontal and vertical flip
			cv::flip(flipImage.image, flipImage.image, -1);   
                flipImage.header = msg->header;
                flipImage.encoding = msg->encoding;
		}else{
			cv::flip(cv_bridge::toCvShare(msg, "")->image, flipImage.image, flip_type);     // -1 means horizontal and vertical flip
                flipImage.header = msg->header;
                flipImage.encoding = msg->encoding;
		}
		// Publish flip image
        pub.publish(flipImage.toImageMsg());
	}
	catch(...)
	{ 
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "flip_image_node");
	ros::NodeHandle nh;
	ros::NodeHandle lnh("~");
        lnh.getParam("flip_type",flip_type);
        
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("input_image", 1, imageCallback);
	pub = it.advertise("flip_image", 1);
	ros::spin();
}




