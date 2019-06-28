//loadfile.cpp
//包含所有与源文件读入有关的函数
//作者：郑嘉佑、张子辰
#include "declaration.h"
using namespace std;

//作者：张子辰
string FileType(const string &filename)
{
	int p=filename.length()-1;
	while(p>=0&&filename[p]!='.')p--;
	if(p<0)return "";
	string tmp=filename.substr(p,filename.length()-p+1);
	for(auto &i:tmp)
	{
		if(i>='A'&&i<='Z')i+=32;
	}
	return tmp;
}

//作者：郑嘉佑
void LoadFile(string FileName)
{
	if(FileType(FileName)!=".bxs")
		FileName+=".bxs";
	ifstream in(FileName);
	if(!in)
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="void LoadFile(string FileName)";
		mes.description="no such file";
		mes.other.push_back("FileName="+FileName);
		mes.line=0;
		error.push_back(mes);
		throw error;
	}
	Lines.resize(1);
	string tmp;
	while(getline(in,tmp))
	{
		Lines.push_back(tmp);
	}
}
