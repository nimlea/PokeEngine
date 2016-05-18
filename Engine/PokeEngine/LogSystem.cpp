#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#include "LogSystem.h"

LogSystem::LogSystem()
{
	InitFile();
}

LogSystem::~LogSystem()
{
}

void LogSystem::Log(char *log, ...)
{
	if (!log){
		return;
	}

	va_list args;
	char buffer[MAX_OUTPUT_LENGTH];
	va_start(args, log);
	int count = vsnprintf_s(buffer, sizeof(buffer), log, args);
	if (count == -1 || count >= sizeof(buffer))
	{
		buffer[sizeof(buffer)-1] = '\0';
	}
	va_end(args);

	printf(buffer);
	printf("\n");
	SaveToFile(buffer);
}

void LogSystem::Trace(char *str, ...)
{
	if (!str){
		return;
	}

	va_list args;
	char buffer[MAX_OUTPUT_LENGTH];
	va_start(args, str);
	int count = vsnprintf_s(buffer, sizeof(buffer), str, args);
	if (count == -1 || count >= sizeof(buffer))
	{
		buffer[sizeof(buffer)-1] = '\0';
	}
	va_end(args);

	printf(buffer);
}

void LogSystem::InitFile()
{
	FILE *file = nullptr;
	fopen_s(&file, "Log.log", "w+");
	if (file != NULL)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		char str[MAX_OUTPUT_LENGTH];
		memset(str, '\0', sizeof(str));
		// YYYY-MM-DD HH:MM
		sprintf_s(str, "Date:%d-%02d-%02d %02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
		fwrite(str, strlen(str), 1, file);
		fwrite("\n", strlen("\n"), 1, file);
		fflush(file);
		fclose(file);
	}
}

void LogSystem::SaveToFile(char *log)
{
	FILE *file = nullptr;
	fopen_s(&file, "Log.log", "a+");
	if (file != NULL)
	{
		char str[MAX_OUTPUT_LENGTH];
		memset(str, '\0', sizeof(str));
		sprintf_s(str, log);
		fwrite(str, strlen(str), 1, file);
		fwrite("\n", strlen("\n"), 1, file);
		fflush(file);
		fclose(file);
	}
}