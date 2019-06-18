//function.cpp
//包含与函数调用相关的函数
//作者：张子辰
#include "declaration.h"
using namespace std;

map<string, int> SysFunctionList = 
{ 
	{"SQR",Fun_SQR},
	{"ABS",Fun_ABS},
	{"SIN",Fun_SIN},
	{"COS",Fun_COS},
	{"TAN",Fun_TAN},
	{"ARCSIN",Fun_ARCSIN},
	{"ARCCOS",Fun_ARCCOS},
	{"ARCTAN",Fun_ARCTAN},
	{"LN",Fun_LN},
	{"LG",Fun_LG},
	{"EXP",Fun_EXP},
	{"FLOOR",Fun_FLOOR},
	{"CEIL",Fun_CEIL},
	{"ROUND",Fun_ROUND},
	{"FACT",Fun_FACT},
	{"PI",Fun_PI},
	{"ROOT" ,Fun_ROOT},
	{"MAX",Fun_MAX},
	{"MIN",Fun_MIN}
};

//调用一个函数
smart_ptr<BasicVar> CallFunction(const string &name,const vector<smart_ptr<BasicVar>> &par)
{
	if(SysFunctionList.count(name))
	{
		int tmp=GetParameterCnt(name);
		if(tmp==-1||par.size()==tmp)
			return SystemFunction[SysFunctionList[name]](par);
		else
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="smart_ptr<BasicVar> CallFunction(const string &name,const vector<smart_ptr<BasicVar>> &par)";
			mes.description="parameter count does not match the function";
			mes.other.push_back("fuction="+name);
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
		
	else
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> CallFunction(const string &name,const vector<smart_ptr<BasicVar>> &par)";
		mes.description="the function is not found";
		mes.other.push_back("fuction="+name);
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
}

//获取一个函数的参数个数，返回-1表示参数个数可变
int GetParameterCnt(const string &name)
{
	if (SysFunctionList.count(name))
		return SystemFunctionParameterCount[SysFunctionList[name]];
	else
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="int GetParameterCnt(const string &name)";
		mes.description="the function is not found";
		mes.other.push_back("fuction="+name);
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
}

bool IsFunction(const string &name)
{
	return SysFunctionList.count(name);
}