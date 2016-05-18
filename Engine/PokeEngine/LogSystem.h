#ifndef _LOGSYSTEM_H_
#define _LOGSYSTEM_H_

#define MAX_OUTPUT_LENGTH 4096

class LogSystem
{
public:
	static LogSystem& GetInstance()
	{
		static LogSystem instance;
		return instance;
	}

public:
	/**
	 @brief 输出到控制台并保存到日志文件
	 */
	void Log(char *log, ...);
	/**
	 @brief 只输出到控制台，不保存到文件
	 */
	void Trace(char *str, ...);

private:
	LogSystem();
	~LogSystem();

	void InitFile();
	void SaveToFile(char *log); // save log to file
};

#endif