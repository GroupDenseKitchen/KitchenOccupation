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

        return true;
    }

    void QueSeverityEstimator::process(FrameList &frames)
    {
        shiftHistoryWindow( frames.getCurrent() );


    }

    void QueSeverityEstimator::shiftHistoryWindow( Frame newFrame )
    {
        FrameQueData newQueData;
        for ( CameraObject & camera: newFrame.getCameras() ) {
            newQueData.queIsPresent = newQueData.queIsPresent | camera.queIsVisible;
            newQueData.totalEnteredPeople += camera.getEntered();
        }
        historyWindow.push_back( newQueData );
        if ( historyWindow.size() >= historyLength ) {
            historyWindow.pop_front();
        }
    }



}
