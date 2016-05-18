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
	 @brief ���������̨�����浽��־�ļ�
	 */
	void Log(char *log, ...);
	/**
	 @brief ֻ���������̨�������浽�ļ�
	 */
	void Trace(char *str, ...);

private:
	LogSystem();
	~LogSystem();

	void InitFile();
	void SaveToFile(char *log); // save log to file
};

#endif