#pragma once

#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <algorithm>

#define LOG_ASSERT(x) if(!(x)) __debugbreak()

class Log;
#define PL_INFO(x) Log::SimpleInfo(x, __FILE__, __LINE__)
#define PL_ERROR(x) Log::SimpleErr(x, __FILE__, __LINE__)

class Log
{
private:
	inline static HANDLE console;
public:
	static void PrintFile(const char* file, int line) {
		SetConsoleTextAttribute(console, 2);
		std::cout << file << " at line " << line << std::endl;
		SetConsoleTextAttribute(console, 7);
	}
	static void Init() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	// INFO
	template <typename... Args>
	static void Info(const std::string& message, Args&& ...args) {
		SetConsoleTextAttribute(console, 2);
		Print(message, args...);
		SetConsoleTextAttribute(console, 7);
	}
	static void Info(const std::string& message, const char* file, const char* line) {
		SetConsoleTextAttribute(console, 2);
		std::cout << message << "in file " << file << " at line " << line << std::endl;
		SetConsoleTextAttribute(console, 7);
	}
	static void SimpleInfo(const char* message, const char* file, int line) {
		SetConsoleTextAttribute(console, 2);
		std::cout << message << " in file " << file << " at line " << line << std::endl;
		SetConsoleTextAttribute(console, 7);
	}

	// WARNINGS
	template <typename... Args>
	static void Warn(const std::string& message, Args&& ...args) {
		SetConsoleTextAttribute(console, 6);
		Print(message, args...);
		SetConsoleTextAttribute(console, 7);
	}
	static void Warn(const std::string& message = "warning") {
		SetConsoleTextAttribute(console, 6);
		std::cout << message << std::endl;
		SetConsoleTextAttribute(console, 7);
	}

	// ERRORS
	template <typename... Args>
	static void Err(const std::string& message, Args&& ...args) {
		SetConsoleTextAttribute(console, 4);
		Print(message, args...);
		SetConsoleTextAttribute(console, 7);
	}
	static void Err(const std::string& message = "error") {
		SetConsoleTextAttribute(console, 4);
		std::cout << message << std::endl;
		SetConsoleTextAttribute(console, 7);
	}
	static void SimpleErr(const std::string& message, const char* file, int line) {
		SetConsoleTextAttribute(console, 4);
		std::cout << message << " in file " << file << " at line " << line << std::endl;
		SetConsoleTextAttribute(console, 7);
	}
private:
	template <typename... Args>
	static void Print(const std::string& message, Args&& ...args) {
		uint16_t delimPosition = 0;
		size_t n = std::count(message.begin(), message.end(), '\\');
		LOG_ASSERT(n == sizeof...(args));
		std::string temp = message;
		std::string msg = "";

		auto f = [&](const auto& value) {
			//std::cout << "temp = " << temp << std::endl;
			delimPosition = temp.find('\\');
			//std::cout << "delimpos = " << delimPosition << std::endl;
			auto s = temp.substr(0, delimPosition);
			s += std::to_string(value);
			//std::cout << s << std::endl;
			msg += s;
			temp = temp.substr(delimPosition + 1);
		};
		((f(std::forward<Args>(args))), ...);
		std::cout << msg << std::endl;
	}
};