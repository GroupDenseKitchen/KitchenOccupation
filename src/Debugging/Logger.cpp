#include "Logger.hpp"


namespace debugging{
Logger logObject;

Logger::Logger()
{
	buildDate = __DATE__;
	buildTime = __TIME__;
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
	while(!logFile.empty())
		v.push_back(getLastEntry());
	return v;
}

void Logger::dumpToConsole(void){
	std::vector<std::string> lines = flushLog();
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it){
		std::cout << *it << std::endl;
	}
}

}