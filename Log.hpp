#ifndef LOGCLASS_H
#define LOGCLASS_H

#include <iostream>
#include <fstream>

class Log {
public:
	static void Disable();	//turn off logging
	static void	SetOutput(const char *);	//set a target log file and opens it for writing
	static void	SetOutput(std::ostream*); //set a target log stream
  static void Terminate(); //close out open logfiles etc
	static std::ostream	*Output; //ptr to the log output stream
protected:
	static bool 					internalLogging;	//are we logging to our internal log ofstream or using another ostream?
	static std::ofstream	*logFile;					//actual log file (needed for fstream functionality that the generic log ostream can't provide)
};

#endif