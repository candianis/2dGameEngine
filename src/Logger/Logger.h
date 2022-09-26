#ifndef LOGGER_H
#define LOGGER

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iostream>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry {
	LogType type;
	std::string message;	
};

class Logger
{

public:
	static std::vector<LogEntry> messages;
	static void Log(const std::string& message);
	static void Err(const std::string& message);


};

#endif // !LOGGER_H
