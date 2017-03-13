#pragma once
#include <windows.h>
#include <string>
#include <iostream>

class Logger
{
public:
	enum LEVEL{
		DBG, INFO, ERR, FATAL
	};

public:
	static Logger& Logger::getInstance() {
		static Logger logger;
		return logger;
	}

	void Logger::setLevel(LEVEL level) {
		_level = level;
	}


	void Logger::info(const std::string& msg) {
		if (_level >= INFO) {
			std::cout << msg << std::endl;
		}
	}

	void Logger::error(const std::string& msg) {
		if (_level >= ERR) {
			std::cout << msg << std::endl;
			::MessageBoxA(NULL, msg.c_str(), "", 0);
		}
	}

private:
	Logger::Logger() {}

private:
	LEVEL _level = INFO;
};