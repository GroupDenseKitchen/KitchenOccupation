#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <stdio.h>
#include <iostream>

#include "../Utilities/utilities.hpp"
#include "../Utilities/StringUtilities.hpp"

/*!
 *  \brief    Debugging utilities.
 */
namespace debugging
{
/*!
 *  \brief     A log entry is a container of log information.
 *  \version   0.1
 *  \date      2013-10-07
 */
struct LogEntry
{
    std::string tag;
    std::string message;
    std::string callingFunction;
    std::string fileName;
    std::string lineNumber;
    std::string time;
    std::string date;

    /*!
       \brief   Constructor.
    */
    LogEntry(std::string tag, std::string message, std::string callingFunction, std::string fileName,
             std::string lineNumber) :
            message(message), callingFunction(callingFunction), tag(tag), fileName(fileName), lineNumber(lineNumber)
            {}

    /*!
       \brief   Get a string with default format or with specified formating.
    */
    std::string toString(std::string format = "[%tag]%msg(%file::%function@%line)[%time]");
};

/*!
 *  \brief     A profiler entry is a container of profiler information.
 *  \version   0.1
 *  \date      2013-10-11
 */
struct ProfilerEntry
{
    std::string tag;
    double milliseconds;
    int64 begun, ended;
    std::list<ProfilerEntry> children;
    ProfilerEntry * parent;

    ProfilerEntry(std::string tag, int64 begun, ProfilerEntry * parent) : tag(tag), begun(begun), parent(parent) {}
};

/*!
 *  \brief     LogEntry iterator
 */
typedef std::deque<LogEntry>::iterator LogIterator;

/*!
 *  \brief     Logger is a logging manager that is globally accessable
 *             under the alias 'logObject'.
 *  \details   Logger manages LogEnty's and provides an interface
 *             for easy readouts of log entries.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Logger
{
public:

    /*!
       \brief   Constructor.
    */
    Logger();

    /*!
       \brief   Destructor.
    */
    ~Logger() {}

    /*!
       \brief   Add a log entry and give it a time stamp.
    */
    void append(LogEntry);

    /*!
       \brief   Pop a log entry.
    */
    LogEntry pop();

    /*!
       \brief   Get a specific log entry.
       \warning index must be between 0 and size()-1
    */
    LogEntry get(int index) { return logFile[index];  }

    /*!
       \brief   The number of stored log entries.
    */
    int size()              { return logFile.size();  }

    /*!
       \brief   Weather or not no log entries exist.
    */
    bool isEmpty()          { return logFile.empty(); }

    /*!
       \brief   Remove all log entries.
    */
    void clear()            { logFile.clear();        }

    /*!
       \brief   Get the begin iterator for the log entries.
    */
    LogIterator begin()     { return logFile.begin(); }

    /*!
       \brief   Get the end iterator for the log entries.
    */
    LogIterator end()       { return logFile.end();   }

    /*!
       \brief   Getter for the most recent log entry as a string,
                with a varity of formating options for the string content.
    */
    std::string getLastEntry(std::string format = "[%tag]%msg(%file::%function@%line)[%time]");

    /*!
       \brief   Empties the log into a vector of strings.
    */
    std::vector<std::string> flushLog();

    /*!
       \brief   Empties the log and dumps all entries as strings in the console.
    */
    void dumpToConsole();

    /*!
       \brief   Pop the oldest profiler entry
    */
    ProfilerEntry popProfiler();

    /*!
       \brief   Get the number of profiler entries.
    */
    int profilerSize();

    /*!
       \brief   Initiate a new iteration for the profiler.
    */
    void profilerBeginIteration();

    /*!
       \brief   Finalize an iteration for the profiler.
    */
    void profilerEndIteration();

    /*!
       \brief   Initiate a new section for the profiler.
    */
    void profilerBeginSection(std::string tag);

    /*!
       \brief   FInalize a section for the profiler.
    */
    void profilerEndSection();

    /*!
       \brief   Get the most recent iteration from the profiler.
    */
    ProfilerEntry * getLatestIteration() { return &loopIterations.back(); }

    /*!
       \brief   Dump a specific iteration from the profiler.
    */
    void profilerDumpSectionToConsole(ProfilerEntry * pe, int depth = 0);
private:
    std::deque<LogEntry> logFile;
    std::string buildDate;
    std::string buildTime;
    time_t rawTime;

    // Profiler
    std::deque<ProfilerEntry> loopIterations;
    ProfilerEntry * parent;
    int iteration;
    int historyLength;
};

// Global Logger
extern Logger logObject;

// Macro interface
/*!
   \brief   MACRO used for logging anywhere in the project.
   \param   TAG is a string useful for filtering.
                Use "Config" for config,
                    "Config error" for config errors,
                    "Config defails" for additional details...
   \param   message is a stream, so about enything can be piped using <<.
                Example: LOG("Config", "Successfully loaded " << nSuccess << " configurations!")
*/
#define LOG(TAG, message) {std::stringstream ss; ss << message; debugging::logObject.append(debugging::LogEntry(TAG, ss.str(), __FUNCTION__, __FILE__, std::to_string(__LINE__))); }

#define PROFILER_ITERATION_START() debugging::logObject.profilerBeginIteration();
#define PROFILER_ITERATION_END()   debugging::logObject.profilerEndIteration();
#define PROFILER_START(tag)        debugging::logObject.profilerBeginSection(tag);
#define PROFILER_END()             debugging::logObject.profilerEndSection();
#define PROFILER_DUMP()            debugging::logObject.profilerDumpSectionToConsole(debugging::logObject.getLatestIteration());

}

#endif
