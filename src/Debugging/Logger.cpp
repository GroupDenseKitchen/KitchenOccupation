#include "Logger.hpp"


namespace debugging{
Logger logObject;

Logger::Logger()
{
	buildDate = __DATE__;
	buildTime = __TIME__;
    iteration = 0;
    historyLength = 500;
}

void Logger::append(LogEntry entry) {
	time(&rawTime);
	entry.timestamp = asctime(localtime(&rawTime));
	logFile.push_back(entry);
}

void replaceString(std::string & str, std::string toReplace, std::string replaceWith){

	str.replace(str.find(toReplace), toReplace.length(), replaceWith);
}

std::string Logger::getLastEntry( std::string format ) {

	if(!logFile.empty()) {
		LogEntry l = logFile.front();

		std::string logString = format;

		replaceString(logString, "%tag", l.tag);
		replaceString(logString, "%msg", l.msg);
		replaceString(logString, "%file", l.filename);
		replaceString(logString, "%function", l.callingFunction);
		replaceString(logString, "%line", l.lineNumber);
		replaceString(logString, "%time", l.timestamp);

		return logString;
	}
	return std::string("Error: No log (empty)!");
}

std::vector<std::string> Logger::flushLog() {
	std::vector<std::string> v;
    while(!logFile.empty()) {
		v.push_back(getLastEntry());
        logFile.pop_front();
    }
	return v;
}

void Logger::dumpToConsole(void){
	std::vector<std::string> lines = flushLog();
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it){
		std::cout << *it << std::endl;
	}
}
/*
ProfilerEntry Logger::popProfiler()
{
    ProfilerEntry entry = loopIterations.get_
}*/

int Logger::profilerSize()
{
    return loopIterations.size();
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
    for(int n = 0; n < depth; n++)
        std::cout << " ";
    std::cout << pe->tag << " [" << pe->milliseconds << "]\n";
    depth++;
    for(std::list<ProfilerEntry>::iterator entry = pe->children.begin(); entry != pe->children.end(); entry++)
        profilerDumpSectionToConsole(&*entry, depth);
}


}
