#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/utilities.hpp"

/*!
 *  \brief     Evaluates systems entry/exit counter
 *  \details
 */

namespace evaluation {

    struct inOutEvent {
        int in;
        int out;
    };

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
         * \brief           Initializes the entryexitevaluation module
         * \param settings  Configuration-object containing the location of the
         *                  ground truth files and other relevant settings.
         * \return          Returns true if successful.
         */
        bool initialize(configuration::ConfigurationManager &settings);// override;

        /*!
         * \brief   Evaluates and updates the results for the system.
         * \param   frames The Frames to be evaluated.
         * \details Is called upon after each iteration in order to
         *          calculat the difference between the systems
         *          entry/exit estimate and the ground truth
         */
        void process(FrameList & frames);// override;

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
