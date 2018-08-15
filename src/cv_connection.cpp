#include "cv_connection.hpp"

#include <vector>
#include <string>


#include <cv_bridge/cv_bridge.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <image_transport/image_transport.h>

#include <opencv2/core.hpp>
#include <opencv2/core/opengl.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/highgui.hpp>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

OpenCVConnector::OpenCVConnector(std::string topic_name,size_t csiPort,uint32_t cameraIdx) :  counter(0),csiPort(csiPort),cameraIdx(cameraIdx)	{
   	
   image_transport::ImageTransport it = image_transport::ImageTransport( ros::NodeHandle() );
   
   pub = it.advertise(topic_name, 1);
   
   ROStime = ros::Time::now();
   ROStimemain = ros::Time::now();
}

void OpenCVConnector::WriteToROS( uint8_t* buffer, int width, int height) {
	sensor_msgs::ImagePtr ptr = boost::make_shared<sensor_msgs::Image>();
    sensor_msgs::Image &img_msg = *ptr; // >> message to be sent
	 
    std_msgs::Header header; // empty header
    header.seq = counter; // user defined counter
    header.stamp = ros::Time::now(); // time
		
	// Formatting directly the message no OpenCV
	img_msg.header = header;
	img_msg.height = height;
	img_msg.width = width;
	img_msg.encoding = sensor_msgs::image_encodings::RGB8;
	
	img_msg.step = width * 3; // 1 Byte per 3 Channels of the RGB format
	
	size_t size = img_msg.step * height;
	//img_msg.data.resize(size);
	
	img_msg.data.insert( img_msg.data.begin() , buffer , buffer + size ) ; // Better than copy?
	//memcpy((char *)( &img_msg.data[0] ) , buffer , size);
	
	pub.publish( ptr );
}

void OpenCVConnector::WriteToOpenCV(unsigned char* buffer, int width, int height) {
	// See reference here : https://devtalk.nvidia.com/default/topic/1010127/driveworks/how-to-convert-dwimagenvmedia-or-nvmediaimage-to-opencvs-cv-mat-/
	
	// This would take a lot of time:  mat_img and cvtColor take 100% of CPU!
	// create a cv::Mat from a dwImageNvMedia rgbaImage
    cv::Mat mat_img(cv::Size(width, height), CV_8UC4, buffer);
    //cv::Mat converted;//=new cv::Mat();
    cv::cvtColor( mat_img  , mat_img,cv::COLOR_RGBA2RGB);   //=COLOR_BGRA2BGR
	
    //cv_bridge::CvImage img_bridge;
    //sensor_msgs::Image img_msg; // >> message to be sent
		
    std_msgs::Header header; // empty header
    header.seq = counter; // user defined counter
    header.stamp = ros::Time::now(); // time
	 
    //img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, mat_img);
    //img_bridge.toImageMsg(img_msg); // from cv_bridge to sensor_msgs::Image
	
	
	// encode image to a jpg
	//cv::imencode(".jpg", mat_img, *encode_buf, encode_params);


    pub.publish(  cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, mat_img).toImageMsg()  ); 
	//counter ++;
	
	/*std::cerr << "  Port: "<<csiPort<<"  Camera: "<<cameraIdx<<" FPS: " << 1.0/(ros::Time::now().toSec() - ROStimemain.toSec())<<std::endl;
	ROStimemain = ros::Time::now(); */
}

cv::cuda::GpuMat gpu_mat_img_out;

void OpenCVConnector::WriteToOpenCV_GPU(unsigned char* buffer, int width, int height) {
	cv::Mat mat_img(cv::Size(width, height), CV_8UC4, buffer);
	cv::cuda::GpuMat gpu_mat_img(mat_img);
	
	cv::cuda::cvtColor(gpu_mat_img, gpu_mat_img_out, cv::COLOR_RGBA2RGB);
	gpu_mat_img_out.download(mat_img);// Get from GPU memory

    std_msgs::Header header; // empty header
    header.seq = counter; // user defined counter
    header.stamp = ros::Time::now(); // time
	
    pub.publish(  cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, mat_img).toImageMsg()  ); 
	//counter ++;
	 
	/*std::cerr << "  Port: "<<csiPort<<"  Camera: "<<cameraIdx<<" FPS: " << 1.0/(ros::Time::now().toSec() - ROStimemain.toSec())<<std::endl;
	ROStimemain = ros::Time::now(); */
}

/* void OpenCVConnector::Publish_img(cv::Mat mat_img) {
    std_msgs::Header header; // empty header
    header.seq = counter; // user defined counter
    header.stamp = ros::Time::now(); // time

    pub.publish(  cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, mat_img).toImageMsg()  ); 
} */

void OpenCVConnector::showFPS() {
	std::cerr << "  Port: "<<csiPort<<"  Camera: "<<cameraIdx<<" FPS: " << 1.0/(ros::Time::now().toSec() - ROStime.toSec())<<std::endl;
	ROStime = ros::Time::now();
}
	
	


