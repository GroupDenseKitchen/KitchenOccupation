#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/utilities.hpp"
//#include "../Configuration/ConfigurationManager.hpp"

/*!
 *  \brief     Evaluates systems entry/exit counter
 *  \details
 */

namespace evaluation {

    struct inOutEvent {
        int in;
        int out;
    };

    class EntryExitEvaluation : public Algorithm
    {
    public:
        /*!
         * \brief   Constructor.
         */
        EntryExitEvaluation();

        /*!
         * \brief   Destructor.
         */
        ~EntryExitEvaluation();

        /*!
         * \brief           Initializes the entryexitevaluation module
         * \param settings  Configuration-object containing the location of the
         *                  ground truth files and other relevant settings.
         * \return          Returns true if successful.
         */
        bool initialize(configuration::ConfigurationManager &settings) override;

        /*!
         * \brief   Evaluates and updates the results for the system.
         * \param   frames The Frames to be evaluated.
         * \details Is called upon after each iteration in order to
         *          calculat the difference between the systems
         *          entry/exit estimate and the ground truth
         */
        void process(FrameList & frames) override;

    private:
        std::vector<std::vector<inOutEvent>> groundTruth;
        //std::vector<inOutEvent> entryExitData; //använder inte just nu...
        int frameCount;
    };

} //namespace evaluation

#endif // ENTRYEXITEVALUATION_HPP
