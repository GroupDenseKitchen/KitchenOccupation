#include "QueSeverityEstimator.hpp"

/*!
    \brief      statistics contains functionality for extracting usefull data and meta data after
                the image processing steps have been completed.
 */
namespace statistics
{

    QueSeverityEstimator::QueSeverityEstimator()
    {
    }

    bool QueSeverityEstimator::initialize( configuration::ConfigurationManager &settings )
    {
        CONFIG( settings, historyLength, "historyLength", 30*60*5 ); //5 minutes (assumes approximately 30fps)
        CONFIG( settings, lowQueThreshold, "lowQueThreshold", 1.0/3);
        CONFIG( settings, highQueThreshold, "highQueThreshold", 2.0/3 );

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
            newQueData.totalEnteredPeople += camera.getEntered();
        }
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
        if (queRatio < lowQueThreshold) {
            return 0;
        } else if (queRatio < highQueThreshold) {
            return 1;
        } else {
            return 2;
        }
    }
}