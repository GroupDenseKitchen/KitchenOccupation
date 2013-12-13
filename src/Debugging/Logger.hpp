#ifndef LOGGER_H
#define LOGGER_H

#include "../Utilities/utilities.hpp"
#include "../Utilities/StringUtilities.hpp"

/*!
 * \brief   Debugging utilities.
 * \details Contains debugging and logging functionality, primarely a logging system and a profiler.
 */
namespace debugging
{
/*!
 * \brief   A log entry is a container of a log information message.
 * \date    2013-10-07
 */
struct LogEntry
{
    /*!
     * \brief                 Constructor.
     * \param tag             Is a string useful for filtering. Could be a name or identifier.
     * \param message         A log message.
     * \param callingFunction The name of the function or method in which the logging is called.
     * \param fileName        The name of the file in which the logging is called.
     */
    LogEntry(std::string tag, std::string message, std::string callingFunction, std::string fileName,
             std::string lineNumber) :
            tag(tag), message(message), callingFunction(callingFunction), fileName(fileName), lineNumber(lineNumber)
            {}

    /*!
     * \brief        Get a string with default format or with specified formating.
     * \details      Format keywords are the field names with a %-prefix (see the default format below).
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
 * \details It contain a tag (name), time for the entire node in milliseconds and subdivisions stored as children which ar other ProfilerEntry objects.
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
     * \param tag    Is a string useful for filtering. May be a name or identifier.
     */
    ProfilerEntry(std::string tag, int64 begun, ProfilerEntry * parent) : tag(tag), begun(begun), parent(parent) {}
};

/*!
 * \brief   LogEntry iterator
 */
typedef std::deque<LogEntry>::iterator LogIterator;

/*!
 * \brief   Logger is a logging manager that is globally accessable
 *          under the alias 'logObject'.
 * \details Logger manages LogEnty's and provides an interface
 *          for easy readouts of log entries.
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
     */
    void append(LogEntry);

    /*!
     * \brief   Pop a log entry.
     */
    LogEntry pop();

    /*!
     * \brief       Get a specific log entry.
     * \warning     index must be between 0 and size()-1
     */
    LogEntry get(int index) { return logFile[index];  }

    /*!
     * \brief   The number of stored log entries.
     */
    int size()              { return (int)logFile.size();  }

    /*!
     * \brief   Weather or not no log entries exist.
     */
    bool isEmpty()          { return logFile.empty(); }

    /*!
     * \brief   Remove all log entries.
     */
    void clear();

    /*!
     * \brief   Calls clear() and resets iteration counter.
     */
    void reset();

    /*!
     * \brief   Get the begin iterator for the log entries.
     */
    LogIterator begin()     { return logFile.begin(); }

    /*!
     * \brief   Get the end iterator for the log entries.
     */
    LogIterator end()       { return logFile.end();   }

    /*!
     * \brief        Getter for the most recent log entry as a string,
     *               with a varity of formating options for the string content.
     * \details      Format keywords are the field names with a %-prefix (see the default format below).
     */
    std::string getLastEntry(std::string format = "[%tag]%msg(%file::%function@%line)[%time]");

    /*!
     * \brief   Empties the log into a vector of strings.
     */
    std::vector<std::string> flushLog();

    /*!
     * \brief   Empties the log and dumps all entries as strings in the console.
     */
    void dumpToConsole();

    /*!
     * \brief   Pop the oldest profiler entry
     */
    ProfilerEntry popProfiler();

    /*!
     * \brief   Get the number of profiler entries.
     */
    int profilerSize();

    /*!
     * \brief   Initiate a new iteration for the profiler.
     */
    void profilerBeginIteration();

    /*!
     * \brief   Finalize an iteration for the profiler.
     */
    void profilerEndIteration();

    /*!
     * \brief     Initiate a new section for the profiler.
     */
    void profilerBeginSection(std::string tag);

    /*!
     * \brief   Initialize a section for the profiler.
     */
    void profilerEndSection();

    /*!
     * \brief   Get the most recent iteration from the profiler.
     */
    ProfilerEntry * getLatestIteration() { return &loopIterations.back(); }

    /*!
     * \brief       Dump a specific iteration from the profiler.
     * \param pe    Using the node pe as root when dumping its entire profiler tree..
     * \param depth The number of subdivisions to dump. 0 means all of them.
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

/*!
 * \brief         MACRO used for starting a new profiler iteration session.
 */
#define PROFILER_ITERATION_START() debugging::logObject.profilerBeginIteration();

/*!
 * \brief         MACRO used for ending a profiler iteration session.
 */
#define PROFILER_ITERATION_END()   debugging::logObject.profilerEndIteration();

/*!
 * \brief         MACRO used for starting a new profiler subsection.
 */
#define PROFILER_START(tag)        debugging::logObject.profilerBeginSection(tag);

/*!
 * \brief         MACRO used for ending a profiler subsection.
 */
#define PROFILER_END()             debugging::logObject.profilerEndSection();

/*!
 * \brief         MACRO used for dumping the latest profiler iteration to console.
 */
#define PROFILER_DUMP()            debugging::logObject.profilerDumpSectionToConsole(debugging::logObject.getLatestIteration());

}

#endif
