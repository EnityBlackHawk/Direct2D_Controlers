#pragma once
#include <Windows.h>
#include <sstream>

class Exception
{
public:
	static void ErrorMessage(HRESULT hResult, int line, const char* file)
	{
		LPTSTR erroMessage = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&erroMessage, 0, nullptr);

		std::ostringstream os;
		os << "Code: " << hResult << "\n";
		os << "Line: " << line << "\n";
		os << "File: " << file << "\n";
		os << erroMessage;
		LocalFree(erroMessage);

		MessageBox(NULL, os.str().c_str(), "Error", 0);
	}
	static void ErrorMessage(const char* text, int line, const char* file)
	{
		std::ostringstream os;
		//os << "Code: " << hResult << "\n";
		os << "Line: " << line << "\n";
		os << "File: " << file << "\n";
		os << text;

		MessageBox(NULL, os.str().c_str(), "Error", 0);
	}

};

#define ERROR_MESSAGE(x) Exception::ErrorMessage(x, __LINE__, __FILE__)
//#define ErrorMessageText(x) Exception::ErrorMessage(x, __LINE__, __FILE__)
#define ERROR_MESSAGE_LAST_ERROR() Exception::ErrorMessage(GetLastError(), __LINE__, __FILE__)

