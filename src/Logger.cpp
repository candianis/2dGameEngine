#include "Headers/Logger.h"

// Current time
time_t tStart;
//  Current time structure
struct tm tmStart;

void Logger::Log(const std::string& message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	std::cout << "LOG | ";
	PrintCurrentDate();
	std::cout << " ";
	PrintCurrentTime();
	std::cout << " - " << message << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

}

void Logger::Err(const std::string& message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	std::cout << "ERR | ";
	PrintCurrentDate(); 
	std::cout << " "; 
	PrintCurrentTime();
	std::cout << " - " << message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

std::string Logger::GetMonth(int monthInNumber)
{
	switch (monthInNumber) {
		case 1:
			return "Jan";
			break;
		case 2:
			return "Feb";
			break;
		case 3:
			return "Mar";
			break;
		case 4:
			return "Apr";
			break;
		case 5:
			return "May";
			break;
		case 6:
			return "Jun";
			break;
		case 7:
			return "Jul";
			break;
		case 8:
			return "Aug";
			break;
		case 9:
			return "Sep";
			break;
		case 10:
			return "Oct";
			break;
		case 11:
			return "Nov";
			break;
		case 12:
			return "Dec";
			break;
				
	}
}

void Logger::PrintCurrentDate()
{
	tStart = time(0);
	localtime_s(&tmStart, &tStart);
	std::cout << tmStart.tm_mday << "-" << GetMonth(tmStart.tm_mon + 1) << "-" << tmStart.tm_year + 1900;
}

void Logger::PrintCurrentTime()
{
	tStart = time(0);
	localtime_s(&tmStart, &tStart);
	std::cout << tmStart.tm_hour << ":" << tmStart.tm_min << ":" << tmStart.tm_sec;
}
