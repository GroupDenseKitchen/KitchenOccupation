#ifndef LOGGER_H
#define LOGGER_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/StringUtilities.hpp"

/*!
 * \brief   Debugging utilities.
 * \details TODO
 */
namespace debugging
{
/*!
 * \brief   A log entry is a container of log information.
 * \details TODO
 * \version 0.1
 * \date    2013-10-07
 */
struct LogEntry
{
    /*!
     * \brief                 Constructor.
     * \param tag             TODO
     * \param message         TODO
     * \param callingFunction TODO
     * \param fileName        TODO
     */
    LogEntry(std::string tag, std::string message, std::string callingFunction, std::string fileName,
             std::string lineNumber) :
            tag(tag), message(message), callingFunction(callingFunction), fileName(fileName), lineNumber(lineNumber)
            {}

    /*!
     * \brief        Get a string with default format or with specified formating.
     * \details      TODO
     * \param format TODO
     * \return       TODO
     */
    std::string toString(std::string format = "[%tag]%message\n(%file::%function@%line)[%time]");

    std::string tag;
    std::string message;
    std::string callingFunction;
    std::string fileName;
    std::string lineNumber;
    std::string time;
    std::string date;
};

/*!
 * \brief   A profiler entry is a container of profiler information.
 * \details TODO
 * \version 0.1
 * \date    2013-10-11
 */
struct ProfilerEntry
{
    std::string tag;
    double milliseconds;
    int64 begun, ended;
    std::list<ProfilerEntry> children;
    ProfilerEntry * parent;

    /*!
     * \brief        Contructor.
     * \param tag    TODO
     * \param begun  TODO
     * \param parent TODO
     */
    ProfilerEntry(std::string tag, int64 begun, ProfilerEntry * parent) : tag(tag), begun(begun), parent(parent) {}
};

/*!
 * \brief   LogEntry iterator
 * \details TODO
 */
typedef std::deque<LogEntry>::iterator LogIterator;

/*!
 * \brief   Logger is a logging manager that is globally accessable
 *          under the alias 'logObject'.
 * \details Logger manages LogEnty's and provides an interface
 *          for easy readouts of log entries.
 * \version 0.1
 * \date    2013-10-07
 */
class Logger
{
public:

    /*!
     * \brief   Constructor.
     */
    Logger();

    /*!
     * \brief   Destructor.
     */
    ~Logger() {}

    /*!
     * \brief   Add a log entry and give it a time stamp.
     * \details TODO
     * \param   TODO
     */
    void append(LogEntry);

    /*!
     * \brief   Pop a log entry.
     * \details TODO
     * \return  TODO
     */
    LogEntry pop();

    /*!
     * \brief       Get a specific log entry.
     * \details     TODO
     * \param index TODO
     * \return      TODO
     * \warning     index must be between 0 and size()-1
     */
    LogEntry get(int index) { return logFile[index];  }

    /*!
     * \brief   The number of stored log entries.
     * \details TODO
     * \return  TODO
     */
    int size()              { return (int)logFile.size();  }

    /*!
     * \brief   Weather or not no log entries exist.
     * \details TODO
     * \return  TODO
     */
    bool isEmpty()          { return logFile.empty(); }

    /*!
     * \brief   Remove all log entries.
     * \details TODO
     */
    void clear();

    /*!
     * \brief   TODO
     * \details TODO
     */
    void reset();

    /*!
     * \brief   Get the begin iterator for the log entries.
     * \details TODO
     * \return  TODO
     */
    LogIterator begin()     { return logFile.begin(); }

    /*!
     * \brief   Get the end iterator for the log entries.
     * \details TODO
     * \return  TODO
     */
    LogIterator end()       { return logFile.end();   }

    /*!
     * \brief        Getter for the most recent log entry as a string,
     *               with a varity of formating options for the string content.
     * \details      TODO
     * \param format TODO
     * \return       TODO
     */
    std::string getLastEntry(std::string format = "[%tag]%msg(%file::%function@%line)[%time]");

    /*!
     * \brief   Empties the log into a vector of strings.
     * \details TODO
     * \return  TODO
     */
    std::vector<std::string> flushLog();

    /*!
     * \brief   Empties the log and dumps all entries as strings in the console.
     * \details TODO
     */
    void dumpToConsole();

    /*!
     * \brief   Pop the oldest profiler entry
     * \details TODO
     * \return  TODO
     */
    ProfilerEntry popProfiler();

    /*!
     * \brief   Get the number of profiler entries.
     * \details TODO
     * \return  TODO
     */
    int profilerSize();

    /*!
     * \brief   Initiate a new iteration for the profiler.
     * \details TODO
     */
    void profilerBeginIteration();

    /*!
     * \brief   Finalize an iteration for the profiler.
     * \details TODO
     */
    void profilerEndIteration();

    /*!
     * \brief     Initiate a new section for the profiler.
     * \details   TODO
     * \param tag TODO
     */
    void profilerBeginSection(std::string tag);

    /*!
     * \brief   Inalize a section for the profiler.
     * \details TODO
     */
    void profilerEndSection();

    /*!
     * \brief   Get the most recent iteration from the profiler.
     * \details TODO
     * \return  TODO
     */
    ProfilerEntry * getLatestIteration() { return &loopIterations.back(); }

    /*!
     * \brief       Dump a specific iteration from the profiler.
     * \details     TODO
     * \param pe    TODO
     * \param depth TODO
     */
    void profilerDumpSectionToConsole(ProfilerEntry * pe, int depth = 0);

private:
    // Logging
    std::deque<LogEntry> logFile;
    std::string buildDate;
    std::string buildTime;
    time_t rawTime;
    int maxLogEntrySize;

    // Profiler
    std::deque<ProfilerEntry> loopIterations;
    ProfilerEntry * parent;
    int iteration;
    unsigned int historyLength;
};

// Global Logger
extern Logger logObject;

// Macro interface
/*!
 * \brief         MACRO used for logging anywhere in the project.
 * \param TAG     Is a string useful for filtering.
 *                Use "settings" for settings,
 *                    "settings error" for settings errors,
 *                    "settings defails" for additional details...
 * \param message Is a stream, so about enything can be piped using <<.
 *                Example: LOG("settings", "Successfully loaded " << nSuccess << " configurations!")
 */
#define LOG(TAG, message) {                             \
    std::stringstream ss;                               \
    ss << message;                                      \
    debugging::logObject.append(debugging::LogEntry(    \
        TAG, ss.str(), __FUNCTION__, __FILE__, std::to_string(__LINE__)) \
    );                                                  \
}

#define PROFILER_ITERATION_START() debugging::logObject.profilerBeginIteration();
#define PROFILER_ITERATION_END()   debugging::logObject.profilerEndIteration();
#define PROFILER_START(tag)        debugging::logObject.profilerBeginSection(tag);
#define PROFILER_END()             debugging::logObject.profilerEndSection();
#define PROFILER_DUMP()            debugging::logObject.profilerDumpSectionToConsole(debugging::logObject.getLatestIteration());

}

#endif
