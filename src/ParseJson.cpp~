#include<iostream>
#include<json/json.h>
#include<stdio.h>
#include <stdlib.h>

#include "ParseJson.h"
#include "BlockList.h"

using namespace std;

ParseJson::ParseJson(){
	
}
ParseJson::~ParseJson(){

}
int ParseJson::analysisjson(char *cmd,BlockList *bl1,BlockList *bl2){

	//test
/*	cmd = "{\
		\"id\":1,\"name\":\"kurama\"}";
*/
/*	cmd = "{\
			\"flag\":\"1\",\
			\"block\":[ \
				{ \
					\"name\":\"空白标定-吸光度\",\
					\"code\":\"%02mcr1111\",\
					\"value\":\"3.0\"\
				},\
				{ \
					\"name\":\"量程选择\",\
					\"code\":\"%02mcr1234\",\
					\"value\":\"0\"	}\
			]}";
*/
	std::cout<<"Json:"<<cmd<<endl;
	if(NULL == cmd)
		return 0;
	char c[2*1024];
	sprintf(c,"{\"block\":%s}",cmd);

	Json::Reader  reader;
	Json::Value   value;
	Json::Value val;
	Json::Value arrayNum;

	if(reader.parse(c,value)){
		if(!value["block"].isNull()){

			if(bl1 != NULL)	
				bl1->m.erase(bl1->m.begin(),bl1->m.end());
			if(bl2 != NULL)	
				bl2->m.erase(bl2->m.begin(),bl2->m.end());	

			arrayNum = value["block"];
			std::cout<<"size:"<<arrayNum.size()<<endl;
			for(int i = 0;i < arrayNum.size();i++){
				/*std::cout<<" "<<arrayNum[i]["device"]<<endl;
				std::cout<<" "<<arrayNum[i]["flag"]<<endl;
				std::cout<<" "<<arrayNum[i]["paramCommand"]<<endl;
				std::cout<<" "<<arrayNum[i]["paramName"]<<endl;
				std::cout<<" "<<arrayNum[i]["paramValue"]<<endl;*/
				
				BlockCode c;
				char t[128];
				strcpy(t,arrayNum[i]["paramName"].asString().c_str());
				c.setname(t);
				strcpy(t,arrayNum[i]["paramCommand"].asString().c_str());
				c.setcode(t);

				strcpy(t,arrayNum[i]["paramValue"].asString().c_str());
				char *p = strstr(t,"-");
				int ni = 0;
				int ma = 0;
				float min = 0.0;
				float max = 0.0;
				if(p != NULL){
					p[0] = '\0';
					min = ni = atoi(t);
					max = ma = atoi(p + 1);
					c.setmin(ni);
					c.setmax(ma);
					c.setmin(min);
					c.setmax(max);
				}else{
					ni = atoi(t);
					c.setvalue(ni);
				}
				strcpy(t,arrayNum[i]["device"].asString().c_str());

				
				if(strcmp(t,"1") == 0){
					strcpy(t,arrayNum[i]["paramCommand"].asString().c_str());
					bl1->insertBC(t,c);
				}else if(strcmp(t,"1") == 0){
					strcpy(t,arrayNum[i]["paramCommand"].asString().c_str());
					bl2->insertBC(t,c);
				}
				
			}

		}
	}

/*
	if(reader.parse(cmd,value))
	{
		if(!value["id"].isNull())
		{
			cout<<value["id"].asInt()<<endl;
			cout<<value["name"].asString()<<endl;
		}
	}
*/
	return 0;
}
