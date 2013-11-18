#ifndef ENTRYEXITEVALUATION_HPP
#define ENTRYEXITEVALUATION_HPP

#include "../Utilities/utilities.hpp"
#include "../Utilities/FrameList.hpp"
//#include "../Configuration/ConfigurationManager.hpp"
#include "../Utilities/Algorithm.hpp"

#include <vector>

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
        bool initialize(configuration::ConfigurationManager & settings) override;

        void process(FrameList &frames) override;
    private:
        std::vector<std::vector<inOutEvent>> groundTruth;
    };

} //namespace evaluation

#endif // ENTRYEXITEVALUATION_HPP
