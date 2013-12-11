#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/utilities.hpp"


namespace evaluation {

/*!
 * \brief   Describes how many people entered or exited the room in the current frame.
 */
struct inOutEvent {
    int in;
    int out;
};

/*!
 * \brief   Evaluates system counting performance.
 * \details System performance is evaluated here by comparing the total number of people that
 *          have exited the room to a pre-recorded ground truth file.
 */
class EntryExitEvaluator : public Algorithm
{
public:
    /*!
     * \brief   Constructor.
     */
    EntryExitEvaluator();

    /*!
     * \brief   Destructor.
     */
    ~EntryExitEvaluator();

    /*!
     * \brief          Initializes the entryexitevaluation module
     * \details        The module is intitalized by loading all ground truth from files specified by
     *                 the configurationManager object
     * \param settings Configuration-object containing the location of the
     *                 ground truth files and other relevant settings.
     * \return         Returns true if successful, otherwise false.
     */
    bool initialize(configuration::ConfigurationManager &settings);// override;

    /*!
     * \brief        Evaluates and updates the results for the system.
     * \details      Is called upon after each iteration in order to
     *               calculate the difference between the systems
     *               entry/exit estimate and the ground truth.
     * \param frames The Frames to be evaluated.
     */
    void process(FrameList & frames);// override;

    /*!
     * \brief             Prints entry/exit accuracy information to the debug log.
     * \param cameraIndex Index of the camera where information is located.
     */
    void printToLog(unsigned int cameraIndex);

private:
    std::vector<std::vector<inOutEvent>> groundTruth;
    std::vector<std::vector<inOutEvent>> entryExitData; //används inte just nu...
    int frameCount;
    int numberOfFrames;
    std::vector<int> prevEntered;
    std::vector<int> prevExited;
    std::vector<int> sumEntryGT;
    std::vector<int> sumExitGT;
    std::vector<int> diffEntries;
    std::vector<int> diffExits;
    std::vector<int> diffTotalOfPeople;

    float accuracyTot;
    float accuracyIn;
    float accuracyOut;
};

} //namespace evaluation

#endif // ENTRYEXITEVALUATION_HPP
