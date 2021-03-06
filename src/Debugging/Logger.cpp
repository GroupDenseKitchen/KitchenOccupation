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
    iteration = 0;
    historyLength = 500;
    maxLogEntrySize = 10000;
}

void Logger::append(LogEntry entry)
{
    time(&rawTime);
    struct tm * timeInfo = localtime(&rawTime);
    std::string hour, minute, second;

    hour = std::to_string(timeInfo->tm_hour);
    minute = std::to_string(timeInfo->tm_min);
    second = std::to_string(timeInfo->tm_sec);

    if (hour.length() < 2)
        hour = "0" + hour;
    if (minute.length() < 2)
        minute = "0" + minute;
    if (second.length() < 2)
        second = "0" + second;

    entry.time = hour + ":" + minute + ":" + second;
    entry.date = std::to_string(timeInfo->tm_mday)+":"+std::to_string(timeInfo->tm_mon)+":"+std::to_string(timeInfo->tm_year);

    if(logFile.size() > maxLogEntrySize)
        logFile.pop_front();
    logFile.push_back(entry);
    printf("%s \n",entry.toString().c_str());
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

void Logger::clear()
{
    logFile.clear();
    loopIterations.clear();
}

void Logger::reset()
{
    clear();
    iteration = 0;
}

void Logger::dumpToConsole(void)
{
    for(std::deque<LogEntry>::iterator it = logFile.begin(); it != logFile.end(); ++it) {
        std::cerr << it->toString() << std::endl << std::endl;
    }
}

ProfilerEntry Logger::popProfiler()
{
    ProfilerEntry entry = loopIterations.front();
    loopIterations.pop_front();
    return entry;
}

int Logger::profilerSize()
{
    return (int)loopIterations.size();
}

void Logger::profilerBeginIteration()
{
    if(loopIterations.size() > historyLength)
        loopIterations.pop_front();
    loopIterations.push_back(ProfilerEntry("Iteration " + std::to_string(iteration++), cv::getTickCount(), 0));
    parent = &loopIterations.back();
    loopIterations.back().parent = parent;  // Handle if something goes wrong in profilerEndSection
}

void Logger::profilerEndIteration()
{
    loopIterations.back().ended = cv::getTickCount();
    loopIterations.back().milliseconds = (double(parent->ended - parent->begun)*1e3)/cv::getTickFrequency();
}

void Logger::profilerBeginSection(std::string tag)
{
    parent->children.push_back(ProfilerEntry(tag, cv::getTickCount(), parent));
    parent = &parent->children.back();
}

void Logger::profilerEndSection()
{
    parent->ended = cv::getTickCount();
    parent->milliseconds = (double(parent->ended - parent->begun)*1e3)/cv::getTickFrequency();
    parent = parent->parent;
}

void Logger::profilerDumpSectionToConsole(ProfilerEntry * pe, int depth)
{
    std::string spaces;
    for(int n = 0; n < depth; n++)
        spaces += " ";
    std::cerr << spaces + pe->tag + " [" + std::to_string(pe->milliseconds) + "]\n";
    depth++;
    for(std::list<ProfilerEntry>::iterator entry = pe->children.begin(); entry != pe->children.end(); entry++)
        profilerDumpSectionToConsole(&*entry, depth);
}


}
