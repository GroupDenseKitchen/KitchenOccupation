#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/Algorithm.hpp"
#include "../Utilities/utilities.hpp"
//#include "../Configuration/ConfigurationManager.hpp"

namespace evaluation {

    struct inOutEvent {
        int in;
        int out;
    };

    class EntryExitEvaluation : public Algorithm
    {
    public:
        /*!
       \brief   Constructor.
    */
        EntryExitEvaluation();

        /*!
       \brief   Destructor.
    */
        ~EntryExitEvaluation();

        /*!
     * \brief           Initializes the entryexitevaluation module
     * \param settings  Configuration-object containing the location of the ground truth files and other relevant settings.
     * \return          Returns true if successful.
     */
        bool initialize(configuration::ConfigurationManager &settings) override;

        void process(FrameList & frames) override;
    private:
        std::vector<std::vector<inOutEvent>> groundTruth;
        //std::vector<inOutEvent> entryExitData;
        int frameCount;
    };

} //namespace evaluation

#endif // ENTRYEXITEVALUATION_HPP
