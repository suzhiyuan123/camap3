#include "SaveBL.h"
using namespace std;
int SaveBL::savebl(char *file,BlockList bl){
	map<std::string,BlockCode>::iterator iter = bl.m.begin();

	ofstream out(file,ios::binary);
	if (!out.is_open()) {
		return -1;
	}

	char key[64];
	iter = bl.m.begin();
	while (iter != bl.m.end()) {
		strcpy(key,iter->first.c_str());
		key[iter->first.length()] ='\0' ;
		out.write(key,64);
		out.write((char*)&iter->second,sizeof(BlockCode));
 		iter++;
	}
	out.close();
	return 0;
}

BlockList SaveBL::readbl(char *file){
	BlockList bl;
	BlockCode bc;

	ifstream in(file,ios::binary);
	if (!in.is_open()) {
 		return bl;
	}
	char key[64];
   	while (!in.eof()) {
      	//in.read((char*)&num,sizeof(int));
		in.read(key,64);
      	in.read((char*)&bc,sizeof(bc));
      	bl.m.insert(make_pair(key,bc));
   	}
	in.close();
	return bl;
}

