#ifndef _CONFIG_H
#define _CONFIG_H
#include "Common.h"
//void GetConfigFileStringValue(char *pszSectionName, char *pszKeyName, char *pDefaultVal, char *pszOutput, UINT32 iOutputLen, char *pszConfigFileName);
//INT32 GetConfigFileIntValue(char *pszSectionName, char *pszKeyName, UINT32 iDefaultVal, char *pszConfigFileName);
//void ReadGlobalParam(Gloabl_Param *gParam);
//void WriteGlobalConfig(Gloabl_Param *gParam);

class LocalConfig{
	public:
		LocalConfig();
		static void ReadGlobalParam(Gloabl_Param *gParam);
		static void WriteGlobalConfig(Gloabl_Param *gParam);
		~LocalConfig();

	private:
		static void UpdateGlobalConfig(Gloabl_Param *gParam);
		static void UpdateGlobalParam(Gloabl_Param *gParam);
		static INT32 GetConfigFileIntValue(char *pszSectionName, char *pszKeyName, UINT32 iDefaultVal, char *pszConfigFileName);      
		static void GetConfigFileStringValue(char *pszSectionName, char *pszKeyName, char *pDefaultVal, char *pszOutput, UINT32 iOutputLen, char *pszConfigFileName);
		static void GetStringContentValue(FILE *fp, char *pszSectionName, char *pszKeyName, char *pszOutput, UINT32 iOutputLen);
		static void GetCompletePath(char *pszConfigFileName, char *pszWholePath);                          
};

#endif
