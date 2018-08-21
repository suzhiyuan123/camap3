#ifndef _LOG_H
#define _LOG_H

#include "Common.h"

// 日志级别定义
#define LOG_FATAL       0     // 严重错误
#define LOG_ERROR       1     // 一般错误
#define LOG_WARN        2     // 警告
#define LOG_INFO        3     // 一般信息
#define LOG_TRACE       4     // 跟踪信息
#define LOG_DEBUG       5     // 调试信息
#define LOG_ALL         6     // 所有信息

//void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel, const char *pszContent);
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pformat, const char *pszContent);
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pformat,int pszContent) ;
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pszContent);

#endif
