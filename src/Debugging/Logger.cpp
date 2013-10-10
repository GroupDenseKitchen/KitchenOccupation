#include "Logger.hpp"


namespace debugging
{
Logger logObject;

std::string LogEntry::toString(std::string format)
{
    replaceString(format, "%tag", tag);
    replaceString(format, "%message", message);
    replaceString(format, "%file", fileName);
    replaceString(format, "%function", callingFunction);
    replaceString(format, "%line", lineNumber);
    replaceString(format, "%time", time);
    replaceString(format, "%date", date);
    return format;
}

Logger::Logger()
{
	buildDate = __DATE__;
	buildTime = __TIME__;
}

void Logger::append(LogEntry entry)
{
	time(&rawTime);
    struct tm * timeInfo = localtime(&rawTime);
    entry.time = std::to_string(timeInfo->tm_hour)+":"+std::to_string(timeInfo->tm_min)+":"+std::to_string(timeInfo->tm_sec);
    entry.date = std::to_string(timeInfo->tm_mday)+":"+std::to_string(timeInfo->tm_mon)+":"+std::to_string(timeInfo->tm_year);
	logFile.push_back(entry);
}

std::string Logger::getLastEntry( std::string format )
{

    if(!logFile.empty()) {
        return logFile.front().toString(format);
	}
	return std::string("Error: No log (empty)!");
}

std::vector<std::string> Logger::flushLog()
{
	std::vector<std::string> v;
    while(!logFile.empty()) {
		v.push_back(getLastEntry());
        logFile.pop_front();
    }
	return v;
}

LogEntry Logger::pop()
{
    LogEntry front = logFile.front();
    logFile.pop_front();
    return front;
}

void Logger::dumpToConsole(void)
{
	std::vector<std::string> lines = flushLog();
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it){
		std::cout << *it << std::endl;
	}
}

}
