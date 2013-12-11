#include "QueSeverityEstimator.hpp"


namespace statistics
{

    QueSeverityEstimator::QueSeverityEstimator()
    {
    }

    bool QueSeverityEstimator::initialize( configuration::ConfigurationManager &settings )
    {
        CONFIG( settings, historyLength, "historyLength", 30*60 ); //1 minutes (assumes approximately 30fps)
        CONFIG( settings, lowQueThreshold, "lowQueThreshold", 1.0/3);
        CONFIG( settings, highQueThreshold, "highQueThreshold", 2.0/3 );
        CONFIG( settings, lowOccupancyThreshold, "lowOccupancyThreshold", INT_MAX );
        CONFIG( settings, highOccupancyThreshold, "highOccupancyThreshold", INT_MAX );

        return true;
    }

    void QueSeverityEstimator::process(FrameList &frames)
    {
        shiftHistoryWindow( frames.getCurrent() );
        frames.getCurrent().setQueStatus( classifyQueStatus() );
    }

    void QueSeverityEstimator::shiftHistoryWindow( Frame newFrame )
    {
        FrameQueData newQueData;
        for ( CameraObject & camera: newFrame.getCameras() ) {
            newQueData.queIsPresent = newQueData.queIsPresent | camera.getQueVisibility();
        }
        std::string currentRoomID = newFrame.getCameras()[0].getRoomID();
        newQueData.peopleInRoom = newFrame.getPopulationInRoomID(currentRoomID);

        historyWindow.push_back( newQueData );
        if ( historyWindow.size() >= historyLength ) {
            historyWindow.pop_front();
        }
    }

    int QueSeverityEstimator::classifyQueStatus() 
    {
        int queCount = 0;
        for (FrameQueData & queSample: historyWindow) {
            if(queSample.queIsPresent) {
                queCount += 1;
            }
        }

        double queRatio = (double)queCount / historyWindow.size();
        int roomOccupancy = historyWindow.back().peopleInRoom;
        if (queRatio < lowQueThreshold &&
            roomOccupancy < lowOccupancyThreshold) {
            return 0;
        } else if (queRatio < highQueThreshold ||
                   (roomOccupancy >= lowOccupancyThreshold && roomOccupancy < highOccupancyThreshold)) {
            return 1;
        } else {
            return 2;
        }
    }
}
