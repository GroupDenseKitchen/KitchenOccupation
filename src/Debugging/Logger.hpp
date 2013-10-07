#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <stdio.h>
#include <iostream>


//This is the project wide logging class

/*!
 *  \brief    Loggining manager
 *  \details  Manages all logging (debugging or non-debugging) and provides easy access.
 */
namespace debugging
{
struct LogEntry
{
    std::string tag;
    std::string msg;
    std::string callingFunction;
    std::string filename;
    std::string lineNumber;
    std::string timestamp;
    LogEntry(std::string tag, std::string msg, std::string callingFunction, std::string filename,
             std::string lineNumber) :
            msg(msg), callingFunction(callingFunction), tag(tag), filename(filename), lineNumber(lineNumber)
            {}
};

/*!
 *  \brief     Logger is a logging manager that is globally accessable under the alias 'logObject'.
 *  \details   Logger manages LogEnty's and provides an interface for easy readouts of log entries.
 *  \version   0.1
 *  \date      2013-10-07
 */
class Logger {
public:
    Logger();
    ~Logger() {}

    void append(LogEntry);

    std::string getLastEntry(std::string format = "[%tag]%msg(%file::%function@%line)[%time]");
    std::vector<std::string> flushLog();
    void dumpToConsole(void);
private:
    std::deque<LogEntry> logFile;
    std::string buildDate;
    std::string buildTime;
    time_t rawTime;
};

// Global Logger
extern Logger logObject;

// Macro interface
#define LOG(TAG, message) {std::stringstream ss; ss << message; debugging::logObject.append(debugging::LogEntry(TAG, ss.str(), __FUNCTION__, __FILE__, std::to_string(__LINE__))); }

}

#endif
