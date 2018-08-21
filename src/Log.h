#ifndef _LOG_H
#define _LOG_H

#include "Common.h"

// ��־������
#define LOG_FATAL       0     // ���ش���
#define LOG_ERROR       1     // һ�����
#define LOG_WARN        2     // ����
#define LOG_INFO        3     // һ����Ϣ
#define LOG_TRACE       4     // ������Ϣ
#define LOG_DEBUG       5     // ������Ϣ
#define LOG_ALL         6     // ������Ϣ

//void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel, const char *pszContent);
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pformat, const char *pszContent);
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pformat,int pszContent) ;
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pszContent);

#endif
