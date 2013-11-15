#image processing module

add_library(ImageProcessing ImageProcessor.cpp BackgroundModelMoG.cpp ForegroundRegionExtractorDefault.cpp TrackingBruteForce.cpp QueDetector.cpp EntryExitCounter.cpp PersonDetection.cpp)

