#include "Log.hpp"

//define Null device path
#if defined _WIN32 || _WIN64
#define LOG_NULL_PATH "\\Device\\Null"
#else
#define LOG_NULL_PATH "/dev/null"
#endif

//init statics
std::ofstream *Log::logFile = new std::ofstream(LOG_NULL_PATH);
std::ostream *Log::Output = logFile;
bool Log::internalLogging = true;

void Log::Terminate() {
	//clean up logging ptrs
	if (internalLogging && logFile) {
		if(logFile->is_open()) logFile->close();
		delete logFile;
		logFile = NULL;
	}
	if(Output) Output = NULL;
}

//Logging
void Log::Disable() {
	internalLogging = true;
	SetOutput(LOG_NULL_PATH);
}
void Log::SetOutput(const char *path) {
	if (internalLogging && logFile && logFile->is_open()) {
		logFile->close();
		delete logFile;
	}
	logFile = new std::ofstream(path);
	Output = logFile;
	internalLogging = true;
}
void 
Log::SetOutput(std::ostream *logstream) {
	if (internalLogging && logFile && logFile->is_open()) {
		logFile->close();
		delete logFile;
		logFile = NULL;
	}
	Output = logstream;
	internalLogging = false;
}