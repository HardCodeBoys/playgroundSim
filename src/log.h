#pragma once

#include <iostream>
#include <time.h>
#include <windows.h> 
#include <algorithm>

#define LOG_ASSERT(x) if(!(x)) __debugbreak()

namespace LogNS {
	static HANDLE console;

	static void Init() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	static void PrintFile(const char* file) {
		SetConsoleTextAttribute(console, 2);
		std::cout << file << std::endl;
		SetConsoleTextAttribute(console, 7);
	}
}

class Log;
#define INFO(x) Log::SimpleInfo(x, __FILE__, __LINE__)

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
private:
	template <typename... Args>
	static void Print(const std::string& message, Args&& ...args) {
		uint16_t delimPosition = 0;
		size_t n = std::count(message.begin(), message.end(), '\\');
		LOG_ASSERT(n == sizeof...(args));
		std::string temp = message;
		auto f = [&](const auto& value) {
			auto s = temp.substr(0, temp.find('\\'));
			s += std::to_string(value);
			std::cout << s;
			delimPosition += s.size();
			temp = message.substr(delimPosition, std::string::npos);
		};
		((f(std::forward<Args>(args))), ...);
		// when \\ is last character, TODO
		if(message.back() != '\\')
			std::cout << message.back();
		std::cout << std::endl;
	}
};