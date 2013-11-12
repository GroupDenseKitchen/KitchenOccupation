#image processing module

add_library(ImageProcessing ImageProcessor.cpp BackgroundModelMoG.cpp ForegroundRegionExtractorTwo.cpp
			ForegroundRegionExtractorDefault.cpp TrackingBruteForce.cpp EntryExitCounter.cpp)
