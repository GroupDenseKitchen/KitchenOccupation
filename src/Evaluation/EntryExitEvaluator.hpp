#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/utilities.hpp"


/*  Hej Malin!
 *
 *  Jag har mergat in master-branchen i den här och det fungerade fint.
 *  Hittade dock en bugg i utskrivningsfunktionen som jag inte har lyckats lösa.
 *  När GT-datafilen tar slut skriver programmet ut nånting i stil med 141483456 där
 *  det borde stå samma sak som det gjorde i föregående frame.
 *
 *  En annan sak är att man gärna vill se i procent hur mycket fel vi har (typ i procent av sanna antalet)
 *  inpasserande och utpasserande. Det vore även kul att se hur mycket det driftar, alltså hur
 *  mycket fel vi har på antalet människor i rummet per totalt antal utpasseringar.
 *
 *  Om du har tid i helgen vorde det awesome om du kunde lösa det här.
 */

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
    };

} //namespace evaluation

#endif // ENTRYEXITEVALUATION_HPP
