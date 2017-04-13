#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

int main(int argc, char** argv)
{
//Inicjalizacja node'a, publishera itp
	ros::init(argc, argv, "image_publisher");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Publisher pub = it.advertise("camera/image", 1);

//otworzenie kamery 0, cap chyba jest uchwytem do niej
	cv::VideoCapture cap(CV_CAP_ANY);

//sprawdzanie, czy otwarte, ewentualna komunikacja o błędzie
	if(!cap.isOpened())
	{
		std::cout<<"Can not open the video cam" << std::endl;
	}
//Pobranie wysokości i szerokości obrazu
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Frame size: " << dWidth << " x " << dHeight << std::endl;
//Tworzenie okna do wyświetlenia obrazu
	cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
//frame jest typem Mat - wielowymiarowa macierz przechowująca obraz
	cv::Mat frame;
//msg jest "ramką" wiadomości. To do niej zapisuję wszystko wysyłane do topicu i to ją publikuję
	sensor_msgs::ImagePtr msg;
	ros::Rate loop_rate(5);
//pętla działająca do zamknięcia
	while (nh.ok()) {
//zapisanie danych z kamery do macierzy "frame", wyświetlenie tego na ekranie w oknie My video
		cap >> frame;
		cv::imshow( "My video", frame );
// Jeśli przejęta ramka nie jest pusta - opublikowanie jej
		if(!frame.empty()) {
			msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
			pub.publish(msg);
			cv::waitKey(1);
		}

		ros::spinOnce();
		loop_rate.sleep();
	}
}
