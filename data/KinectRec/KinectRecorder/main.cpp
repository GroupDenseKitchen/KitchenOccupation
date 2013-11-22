#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <OpenNI.h>

#include <queue>

int main()
{
	// ------------- Setup OpenCV ----------------------
	std::string cameraIP = "169.254.234.112";

	cv::VideoCapture* networkCamera = new cv::VideoCapture("http://root:pass@169.254.234.112/axis-cgi/mjpg/video.cgi?resolution=640x480&.mjpg");
	//cv::VideoCapture* networkCamera = new cv::VideoCapture(0);

	if (!networkCamera->isOpened()){
        std::cout << "Could not open Network Camera at :" << cameraIP << std::endl;
        exit(-1);
    } else {
		std::cout << "Successfully initialized Camera through OpenCV" << std::endl;
	}

	// ------------- Setup OpenNI ----------------------
	// Initiallize OpenNI
	openni::OpenNI::initialize();
	std::string openNIError = openni::OpenNI::getExtendedError();
	if(openNIError.empty()){
		std::cout << "Successfully initialized OpenNI" << std::endl;
	} else {
		std::cout << openNIError << std::endl;
	}

	// Look for devices
	openni::Array<openni::DeviceInfo>* devices = new openni::Array<openni::DeviceInfo>;
	openni::OpenNI::enumerateDevices(devices);

	std::cout << "Available devices: " << devices->getSize() << std::endl;

	// If any interestinge device exists
	if(devices->getSize() > 0){

		openni::Device device;
		openni::VideoStream depth, color;
		const char* deviceURI = openni::ANY_DEVICE;
		openni::Status rc;

		// Try to open device
		rc = device.open(deviceURI);
		if (rc != openni::STATUS_OK)
		{
			printf("SimpleViewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
			openni::OpenNI::shutdown();
			return 1;
		}

		rc = depth.create(device, openni::SENSOR_DEPTH);
		if (rc == openni::STATUS_OK)
		{
			rc = depth.start();
			if (rc != openni::STATUS_OK)
			{
				printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
				depth.destroy();
			}
		}
		else
		{
			printf("SimpleViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
		}

		rc = color.create(device, openni::SENSOR_COLOR);
		if (rc == openni::STATUS_OK)
		{
			rc = color.start();
			if (rc != openni::STATUS_OK)
			{
				printf("SimpleViewer: Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
				color.destroy();
			}
		}
		else
		{
			printf("SimpleViewer: Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
		}

		if (!depth.isValid() || !color.isValid())
		{
			printf("SimpleViewer: No valid streams. Exiting\n");
			openni::OpenNI::shutdown();
			return 2;
		}

		// --------- Read & Record from VideoStream -------------

		openni::VideoStream** streams = new openni::VideoStream*[2];
		streams[0] = &depth;
		streams[1] = &color;
		openni::VideoFrameRef depthFrame, colorFrame;
		cv::Mat cvColorImage, cvDepthImage, cvDepthStoreImage, cvNetworkCameraImage;
		int changedIndex;
		
		std::string depthWritePath = "depthStream.avi";
		std::string colorWritePath = "colorStream.avi";
		std::string networkWritePath = "networkStream.avi";
		double fps = 30;
		cv::Size size(640,480);
		cv::VideoWriter* depthVideoWriter = new cv::VideoWriter(depthWritePath, CV_FOURCC('D','I','V','X'), fps, size);
		cv::VideoWriter* colorVideoWriter = new cv::VideoWriter(colorWritePath, CV_FOURCC('D','I','V','X'), fps, size);
		cv::VideoWriter* networkVideoWriter = new cv::VideoWriter(networkWritePath, CV_FOURCC('D','I','V','X'), fps, size);
		
		bool isRunning = true;
		bool isRecording = false;
		std::cout << "The program is NOT recording" << std::endl;
		std::cout << "Press R to start recording while focusing a cv::Window" << std::endl;

		/*
		// Manipulate - initialize
		// --------------------------------------------------------------------------------
		cv::Mat thresholded, spatialLowPass, temporalLowPass, temporalAverage, morphologicallyCleaned;
		std::deque<cv::Mat> history;
		int minThreshold = 128;
		int spatialKernelSize = 5;
		float spatialVariance = 0.2;
		float temporalLearningRate = 0.2;
		int historyLength = 1;
		int openings = 1;
		int closings = 1;

		cv::namedWindow("Depth thresholded");
		cv::namedWindow("Depth spatialLowPass");
		cv::namedWindow("Depth temporalLowPass");
		cv::namedWindow("Depth temporalAverage");
		cv::namedWindow("Depth morphologicallyCleaned");
		cv::createTrackbar("Threshold", "Depth thresholded", &minThreshold, 255, [](int, void*){});
		cv::createTrackbar("KernelSize", "Depth spatialLowPass", &spatialKernelSize, 41, [](int, void*){});
		int spatialVarianceTemp = 2;
		cv::createTrackbar("Variance", "Depth spatialLowPass", &spatialVarianceTemp, 410, [](int, void*){});
		int temporalLearningRateTemp = 100;
		cv::createTrackbar("LearningRate", "Depth temporalLowPass", &temporalLearningRateTemp, 100, [](int, void*){});
		cv::createTrackbar("HistoryLength", "Depth temporalAverage", &historyLength, 100, [](int, void*){});
		cv::createTrackbar("Openings", "Depth morphologicallyCleaned", &openings, 30, [](int, void*){});
		cv::createTrackbar("Closings", "Depth morphologicallyCleaned", &closings, 30, [](int, void*){});
		*/



		while(isRunning){
			// ------------ Record Network -------------
			networkCamera->read(cvNetworkCameraImage);

			if(isRecording == true){
				networkVideoWriter->write(cvNetworkCameraImage);
			}

			cv::imshow("Network", cvNetworkCameraImage);
			//cv::waitKey(1);


			// ------------ Record Depth -------------
			depth.readFrame(&depthFrame); 
			//std::cout << "Depth frame read" << std::endl;
			cvDepthImage = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(),
					CV_16U, (char*)depthFrame.getData());

			// The max value from the kinect sensor is somewhare around 5000 
			// based on empirical experiments
			cvDepthImage.convertTo(cvDepthImage, CV_16U, 255*255/5000);
			cvDepthImage.convertTo(cvDepthImage, CV_8U, 1.0/255);
			cv::cvtColor(cvDepthImage, cvDepthImage, CV_GRAY2BGR);

			/*
			// Manipulate
			// --------------------------------------------------------------------------------
			cv::threshold(cvDepthImage, thresholded, minThreshold, 255, 2);
			thresholded *= 2;

			// Pre formating
			spatialVariance = spatialVarianceTemp/10.0;
			temporalLearningRate = temporalLearningRateTemp/100.0;
			if(spatialKernelSize % 2 == 0)
				spatialKernelSize += 1;			

			cv::GaussianBlur(thresholded, spatialLowPass, cv::Size(spatialKernelSize, spatialKernelSize), spatialVariance);

			if(temporalLowPass.size().height == 0)
				temporalLowPass = spatialLowPass.clone();

			temporalLowPass = (1-temporalLearningRate)*temporalLowPass + temporalLearningRate*spatialLowPass;

			// History----------
			while(history.size() >= historyLength)
				history.pop_back();

			temporalAverage = spatialLowPass.clone()/(history.size()+1);
			cv::Mat temporalAverageSave = temporalAverage.clone();

			for(cv::Mat & image : history)
				temporalAverage += image;
			
			history.push_front(temporalAverageSave);
			// <----------------
			cv::erode(spatialLowPass,			morphologicallyCleaned, cv::Mat(), cv::Point(-1,-1), openings);
			cv::dilate(morphologicallyCleaned,	morphologicallyCleaned, cv::Mat(), cv::Point(-1,-1), openings);

			cv::dilate(morphologicallyCleaned,	morphologicallyCleaned, cv::Mat(), cv::Point(-1,-1), closings);
			cv::erode(morphologicallyCleaned,	morphologicallyCleaned, cv::Mat(), cv::Point(-1,-1), closings);


			cv::imshow("Depth thresholded", thresholded);
			cv::imshow("Depth spatialLowPass", spatialLowPass);
			cv::imshow("Depth temporalLowPass", temporalLowPass);
			cv::imshow("Depth temporalAverage", temporalAverage);
			cv::imshow("Depth morphologicallyCleaned", morphologicallyCleaned);
			// --------------------------------------------------------------------------------
			*/

			if(isRecording == true){
				depthVideoWriter->write(cvDepthImage);
			}

			cv::imshow("Depth", cvDepthImage);
			//cv::waitKey(1);

			// ------------ Record Color -------------
			color.readFrame(&colorFrame); 
			//std::cout << "Color frame read" << std::endl;
			cvColorImage = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(),
					CV_8UC3, (char*)colorFrame.getData());

			cv::cvtColor(cvColorImage, cvColorImage, CV_RGB2BGR);

			if(isRecording == true){
				colorVideoWriter->write(cvColorImage);
			}

			cv::imshow("Color", cvColorImage);
			//cv::waitKey(1);

			// ------------ Event Handling -------------
			int key = cv::waitKey(1);
			if(key == 'q'){
				isRunning = false;
				std::cout << "Releasing resources" << std::endl;
				networkCamera->release();
				depthVideoWriter->release();
				colorVideoWriter->release();
				networkCamera->release();
				color.destroy();
				depth.destroy();
			} else if(key == 'r') {
				isRecording = !isRecording;
				if(isRecording){
					std::cout << "Recording started" << std::endl;
				} else {
					std::cout << "Recording stopped" << std::endl;
				}
			}
		}

		std::cout << "Successfully finished" << std::endl;
	}

	std::cout << "Shutting down OpenNI" << std::endl;
	openni::OpenNI::shutdown();

	return 0;
}