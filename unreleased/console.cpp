//cosole.cpp
//包含与控制台输出输出相关的函数
//作者：张子辰
#include "declaration.h"
using namespace std;

#ifdef WIN32

namespace
{
char buffer_utf8[16];
wchar_t buffer_utf16[2];
int p_buffer_utf8=0;
HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hStdin=GetStdHandle(STD_INPUT_HANDLE);
}

char GetCharFromConsole()
{
	if(buffer_utf8[p_buffer_utf8])return buffer_utf8[p_buffer_utf8++];
	DWORD dwSize=0;
	while(dwSize==0)
	{
		ReadConsoleW(hStdin,buffer_utf16,1,&dwSize,NULL);
	}
	int n=WideCharToMultiByte(DEFAULT_chcp,0,buffer_utf16,-1,buffer_utf8,16,NULL,NULL);
	buffer_utf8[n]=0;
	p_buffer_utf8=1;
	return buffer_utf8[0];
}

string ReadStringFromConsole()
{
	static char buffer_readstring[65536];
	int i=0;
	char ch=GetCharFromConsole();
	while(0<=ch&&ch<=' ')ch=GetCharFromConsole();
	while(ch>' '||ch<0)
	{
		buffer_readstring[i++]=ch;
		ch=GetCharFromConsole();
	}
	buffer_readstring[i]=0;
	return buffer_readstring;
}

void WriteStringToConsole(const string &str)
{
	int n=MultiByteToWideChar(DEFAULT_chcp,0,str.c_str(),-1,NULL,0);
	wchar_t *buffer=new wchar_t[n+1];
	MultiByteToWideChar(DEFAULT_chcp,0,str.c_str(),-1,buffer,n);
	DWORD dwSize;
	WriteConsoleW(hStdout,buffer,n-1,&dwSize,NULL);
	delete[] buffer;
}
#endif // WIN32
#ifdef linux
char GetCharFromConsole()
{
	return getchar();
}

string ReadStringFromConsole()
{
	static char buffer_readstring[65536];
	int i=0;
	char ch=GetCharFromConsole();
	while(0<=ch&&ch<=' ')ch=GetCharFromConsole();
	while(ch>' '||ch<0)
	{
		buffer_readstring[i++]=ch;
		ch=GetCharFromConsole();
	}
	buffer_readstring[i]=0;
	return buffer_readstring;
}

void WriteStringToConsole(const string &str)
{
	cout<<str;
}
#endif// linux
