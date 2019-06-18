//RAM.cpp
//包含所有与内存访问有关的函数
//作者：张子辰
#include "declaration.h"
using namespace std;

namespace
{
	map<string,BasicVar> Global;// 储存全局变量
	stack<map<string,stack<BasicVar>>> Local;// 储存局部变量
	stack<stack<set<string>>> DeclareList;// 在当前复合语句中声明的变量的列表，方便符合语句结束时的清理工作
}

// 声明全局变量
void DeclareGlobalVar(const string &str,const int &type)
{
	if(Keywordlist.count(str))
	{
		oneMessage mes;
		mes.fun="void DeclareGlobalVar(const string &str,const int &type)";
		mes.description="a keyword cannot be declared as variable";
		mes.line=0;
		mes.other.push_back("variablename="+str);
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
	if(Global.count(str))
	{
		oneMessage mes;
		mes.fun="void DeclareGlobalVar(const string &str,const int &type)";
		mes.description="the variable has already existed";
		mes.line=0;
		mes.other.push_back("variablename="+str);
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
	pointer p=New[type]();
	Global[str]=make_pair(type,p);
}

// 声明局部变量
void DeclareLocalVar(const string &str,const int &type)
{
	if(Keywordlist.count(str))
	{
		oneMessage mes;
		mes.fun="void DeclareLocalVar(const string &str,const int &type)";
		mes.description="a keyword cannot be declared as variable";
		mes.line=0;
		mes.other.push_back("variablename="+str);
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
	if(DeclareList.top().top().count(str))
	{
		oneMessage mes;
		mes.fun="void DeclareLocalVar(const string &str,const int &type)";
		mes.description="the variable has already existed";
		mes.other.push_back("variablename="+str);
		mes.line=0;
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
	pointer p=New[type]();
	Local.top()[str].push(make_pair(type,p));
	DeclareList.top().top().insert(str);
}

// 调用函数时调用
void InitFuctionCall()
{
	Local.push(map<string,stack<BasicVar>>());
	DeclareList.push(stack<set<string>>());
	InitCompoundBegin();
}

// 函数结束时调用
void InitFunctionReturn()
{
	InitCompoundEnd();
	Local.pop();
	DeclareList.pop();
}

// 复合语句开始时调用
void InitCompoundBegin()
{
	DeclareList.top().push(set<string>());
}

// 复合语句结束时调用
void InitCompoundEnd()
{
	for(auto i:DeclareList.top().top())
	{
		Local.top()[i].pop();
		if(Local.top()[i].empty())
			Local.top().erase(i);
	}
	DeclareList.top().pop();
}


// 获得变量的值
smart_ptr<BasicVar> GetVar(const string &name)
{
	if(Local.top().count(name))
		return &(Local.top()[name].top());
	else if(Global.count(name))
		return &(Global[name]);
	else
	{
		oneMessage mes;
		mes.fun="smart_ptr<BasicVar> GetVar(const string &name)";
		mes.description="the variable does not exist";
		mes.line=0;
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
}

smart_ptr<BasicVar> GetVarLeft(const string &name)
{
	if(Local.top().count(name))
		return &(Local.top()[name].top());
	else if(Global.count(name))
		return &(Global[name]);
	else
	{
		DeclareLocalVar(name,DEFAULT_Vartype);
		return &Local.top()[name].top();
	}
}

// 为变量赋值
void SetVar(const string &name,const BasicVar &var)
{
	if(Local.top().count(name))
		Local.top()[name].top()=ImplicitTypeConversion(var,Local.top()[name].top().first);
	else if(Global.count(name))
		Global[name]=ImplicitTypeConversion(var,Global[name].first);
	else
	{
		DeclareLocalVar(name,var.first);
		Local.top()[name].top()=ImplicitTypeConversion(var,Local.top()[name].top().first);
	}
}

BasicVar ImplicitTypeConversion(const BasicVar &var,const unsigned int &type)
{
	if(type==var.first)
	{
		return make_pair(type,MemCopy(var.second));
	}
	//类型相同或都是数字
	if(TypeList[type].typetype==TypeList[var.first].typetype||TypeList[type].typetype<=4&&TypeList[var.first].typetype<=4)
	{
		BasicVar v;
		v.first=type;
		if(TypeConversion.count(make_pair(var.first,type)))
		{
			v.second=TypeConversion[make_pair(var.first,type)](var.second);
			return v;
		}
		else
		{
			//找不到适当的转化函数
			oneMessage mes;
			mes.fun="BasicVar ImplicitTypeConversion(const BasicVar &var,const unsigned int &type)";
			mes.description="a proper conversion-function cannot be found";
			mes.line=0;
			mes.other.push_back("source=[T"+to_string(var.first)+"]");
			mes.other.push_back("aim=[T"+to_string(type)+"]");
			ErrorMessage err;
			err.push_back(mes);
			throw err;
		}
	}
	//不同类型的变量之间无法隐性转换
	oneMessage mes;
	mes.fun="BasicVar ImplicitTypeConversion(const BasicVar &var,const unsigned int &type)";
	mes.description="you cannot implicitly convert a variable to another variable in different typetypes, \
please try using \"explicit type conversion\" (which is temporarily unsupported)";
	mes.line=0;
	mes.other.push_back("source=[T"+to_string(var.first)+"](typetype="+to_string(TypeList[var.first].typetype)+")");
	mes.other.push_back("aim=[T"+to_string(type)+"](typetype="+to_string(TypeList[type].typetype)+")");
	ErrorMessage err;
	err.push_back(mes);
	throw err;
}

BasicVar ExplicitTypeConversion(const BasicVar &var,const unsigned int &type)
{
	if(type==var.first)
		return make_pair(type,MemCopy(var.second));
	BasicVar v;
	v.first=type;
	if(TypeConversion.count(make_pair(var.first,type)))
	{
		v.second=TypeConversion[make_pair(var.first,type)](var.second);
		return v;
	}
	else
	{
		oneMessage mes;
		mes.fun="BasicVar ExplicitTypeConversion(const BasicVar &var,const unsigned int &type)";
		mes.description="a proper conversion-function cannot be found";
		mes.line=0;
		mes.other.push_back("source=[T"+to_string(var.first)+"]");
		mes.other.push_back("aim=[T"+to_string(type)+"]");
		ErrorMessage err;
		err.push_back(mes);
		throw err;
	}
}
