#ifndef QUESEVERITYESTIMATOR_HPP
#define QUESEVERITYESTIMATOR_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
#include "../Utilities/Algorithm.hpp"
#include "../Utilities/CameraObject.hpp"

#include <queue>

/*!
    \brief      statistics contains functionality for extracting usefull data and meta data after
                the image processing steps have been completed.
 */
namespace statistics
{

struct FrameQueData
{
    explicit FrameQueData(int totalEnteredPeople = 0, bool queIsPresent = false): totalEnteredPeople(totalEnteredPeople), queIsPresent(queIsPresent) {}
public:
 int totalEnteredPeople;
 bool queIsPresent;
};

/*!
    \brief
    \details ... THis class asumes that one instance of the program DenseKitchen is run per room.
 */
class QueSeverityEstimator : public Algorithm
{
public:
    /*!
        \brief
     */
    QueSeverityEstimator();

    /*!
        \brief
        \details
     */
    bool initialize( configuration::ConfigurationManager &settings ) override;

    /*!
        \brief
        \details
     */
    void process( FrameList & frames ) override;


private:
    std::deque<FrameQueData> historyWindow;
    int historyLength;
    double lowQueThreshold;
    double highQueThreshold;

    void shiftHistoryWindow( Frame newFrame );
    int classifyQueStatus();
};

}

#endif // QUESEVERITYESTIMATOR_HPP
