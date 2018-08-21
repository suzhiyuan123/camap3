#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "Common.h"
#include "Log.h"

// 全局变量
UINT32  g_iLogLevel      = 8;    // 日志等级
UINT32  g_iLogPosition   = 1;    // 日志位置
//char   g_szLogFile[100] = {0};  // 带路径的日志文件名
const char *g_szLogFile = "/camap/log/Log.log";

extern Gloabl_Param gParam;;

/**********************************************************************
 * 功能描述： 获取对应的日志等级
 * 输入参数： iLogLevel-日志等级
 * 输出参数： 无
 * 返 回 值： 日志等级信息字符串
 * 其它说明： 无
 * 修改日期        版本号         修改人          修改内容
 * -------------------------------------------------------------------
 * 20150522        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
const char *LogLevel(UINT32 iLogLevel)
{
    switch (iLogLevel)
    {
        case LOG_FATAL:
        {
            return "FATAL";
        }

        case LOG_ERROR:   
        {
            return "ERROR";
        }

        case LOG_WARN :
        {
            return "WARN";
        }

        case LOG_INFO :
        {
            return "INFO";
        }

        case LOG_TRACE:   
        {
            return "TRACE";
        }

        case LOG_DEBUG:   
        {
            return "DEBUG";
        }

        case LOG_ALL:   
        {
            return "ALL";
        }

        default: 
        {
            return "OTHER";
        }
    }
}





/**********************************************************************
 * 功能描述： 将内容写到日志文件中
 * 输入参数： pszFileName-代码文件名
             pszFunctionName-代码所在函数名
             iCodeLine-代码行
             iLogLevel-日志等级
             pszContent-每条日志的具体内容
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号         修改人          修改内容
 * -------------------------------------------------------------------
 * 20150522        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/    
 //WriteLog(__FILE__,__LINE__,gParam.loglevel,"CapPicture ok!");
FILE  *fp                 = NULL;
void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,const char *pszContent) 
{
    
    char  szLogContent[2048] = {0};
    char  szTimeStr[128]     = {0};

    if (pszFileName == NULL || pszContent == NULL)
    {
        return;
    }

    // 过滤日志等级
    if (iLogLevel > g_iLogLevel)
    {
        return;
    }

	if(fp == NULL)
    		fp = fopen(g_szLogFile, "at+");      // 打开文件, 每次写入的时候在后面追加
    if (fp == NULL)
    {
        return;
    }
//printf("szLogContent:%s fp:%u\n",szLogContent,fp);
    // 写入日志时间
    GetTime(szTimeStr);
    //fputs(szTimeStr, fp);
	if(fp)
		fwrite(szTimeStr,1,strlen(szTimeStr),fp);

    // 写入日志内容
    if (g_iLogPosition == 1)    // 在日志信息中显示"文件名/函数名/代码行数"信息
    {
        snprintf(szLogContent, sizeof(szLogContent)-1, "[%s][%04d][%s]%s\n", pszFileName, iCodeLine, LogLevel(iLogLevel), pszContent);
	printf(szLogContent);
    }
    else    // 不用在日志信息中显示"文件名/代码行数"信息
    {
        snprintf(szLogContent, sizeof(szLogContent)-1, "[%s]%s\n", LogLevel(iLogLevel), pszContent);
    }
	//printf("szLogContent:%s len:%d fp:%u\n",szLogContent,strlen(szLogContent),fp);
    //fputs(szLogContent, fp);
	if(fp)
		fwrite(szLogContent,1,strlen(szLogContent),fp);

    fflush(fp);     // 刷新文件
//    fclose(fp);     // 关闭文件
 //   fp = NULL;      // 将文件指针置为空

    return;
}

void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel, const  char *pformat,const  char *pszContent) 
{
	char buf[1024] = {'\0'};
	snprintf(buf,sizeof(buf),"%s %s",pformat,pszContent);
	return WriteLog(pszFileName, iCodeLine, iLogLevel,buf) ;
}

void WriteLog(char *pszFileName, UINT32 iCodeLine, UINT32 iLogLevel,  const char *pformat,int pszContent) 
{
	char buf[1024] = {'\0'};
	snprintf(buf,sizeof(buf),"%s %d",pformat,pszContent);
	return WriteLog(pszFileName, iCodeLine, iLogLevel,buf) ;	
}


