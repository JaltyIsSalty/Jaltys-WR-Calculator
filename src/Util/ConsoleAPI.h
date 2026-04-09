#pragma once
#include <iostream>
#include <variant>

//When set to false all Print functions will not output anything
constexpr bool LoggingEnabled = true;


//ANSI Escape Codes for things like color and other text modifiers
namespace ANSI
{
	//Resets all attributes (Color, bold, etc)
	constexpr const char* Reset = "\x1b[0m";

	//Colors
	constexpr const char* Red     = "\x1b[31m";
	constexpr const char* Green   = "\x1b[32m";
	constexpr const char* Yellow  = "\x1b[33m";
	constexpr const char* Blue    = "\x1b[34m";
	constexpr const char* Default = "\x1b[39m";


	constexpr const char* EraseScreen   = "\x1b[2J";
};

using Ansi = const char*;
namespace Console
{
	template<typename T>
	void PrintLn(const T& input, Ansi AnsiCode = ANSI::Reset)
	{
		if (!LoggingEnabled)
			return;

		std::cout << AnsiCode << input << ANSI::Reset << std::endl;
	}

	template<typename T>
	void Print(const T& input, Ansi AnsiCode = ANSI::Reset)
	{
		if (!LoggingEnabled)
			return;

		std::cout << AnsiCode << input << ANSI::Reset << std::endl;
	}

	template<typename T>
	void PrintErr(const T& input)
	{
		if (!LoggingEnabled)
			return;

		std::cout << ANSI::Red << "[ERROR]" << ANSI::Reset << "|" << input << std::endl;
	}

	void EraseAll()
	{
		system("cls");
	}
};