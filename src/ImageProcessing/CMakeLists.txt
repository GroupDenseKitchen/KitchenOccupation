#image processing module

add_library(ImageProcessing ImageProcessor.cpp BackgroundModelMoG.cpp ForegroundRegionExtractorDefault.cpp TrackingBruteForce.cpp EntryExitCounter.cpp KinectSegmentation.cpp StereoBlockMatching.cpp CircleDetection.cpp)

