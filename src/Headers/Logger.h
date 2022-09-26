#ifndef LOGGER_H
#define LOGGER

#include <string>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <iostream>

class Logger
{

public:
	static void Log(const std::string& message);
	static void Err(const std::string& message);
	static std::string GetMonth(int monthInNumber);
	static void PrintCurrentDate();
	static void PrintCurrentTime();

};

#endif // !LOGGER_H





