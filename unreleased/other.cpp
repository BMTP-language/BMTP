//other.cpp
//其他函数
//作者：郑嘉佑、张子辰
#include "declaration.h"

using namespace std;

//作者：郑嘉佑
void INPUT_(string &hint,vector<Expression> &var)
{
	WriteStringToConsole(hint);
	for(auto i:var)
	{
		string x0=ReadStringFromConsole();
		pointer p=New[VarType_string]();
		p.get<string>()=x0;
		auto tmp=CalcExprLeft(i);
		if(tmp->first==VarType_void)
			*tmp=ExplicitTypeConversion(make_pair(VarType_string,p),DEFAULT_Vartype);
		else
			*tmp=ExplicitTypeConversion(make_pair(VarType_string,p),tmp->first);
	}
}

//作者：郑嘉佑
void PRINT_(string &hint,vector<Expression> &var)
{
	WriteStringToConsole(hint);
	for(auto i=var.begin();i!=var.end();i++)
	{
		if(i!=var.begin())putchar(DEFAULT_NumberBreak);
		OutputVar(*CalcExpr(*i));
	}
	if(DEFAULT_Newline)cout<<endl;
}

//作者：张子辰
void SET_(std::string & option,std::string & val)
{
	oneMessage mes;
	ErrorMessage err;
	if(option=="digit")
	{
		if(DEFAULT_Vartype!=VarType_TLargeFloat)
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="this option request something";
			mes.other.push_back("request:\tnumbertype=float");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		DEFAULT_DigitsArraySize=stoi(val);
		DEFAULT_DigitsArraySize2=long(DEFAULT_DigitsArraySize*(DEFAULT_Extradigit+1)+1e-5);
		DEFAULT_Epsilon=DEFAULT_DigitsArraySize;
		DEFAULT_Epsilon_F=TLargeFloat("1e-"+to_string(DEFAULT_Epsilon));
	}
	else if(option=="extradigit")
	{
		if(DEFAULT_Vartype!=VarType_TLargeFloat)
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="this option request something";
			mes.other.push_back("request:\tnumbertype=float");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		DEFAULT_Extradigit=stod(val);
		DEFAULT_DigitsArraySize2=long(DEFAULT_DigitsArraySize*(DEFAULT_Extradigit+1)+1e-5);
	}
	else if(option=="epsilon")
	{
		if(DEFAULT_Vartype!=VarType_TLargeFloat)
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="this option request something";
			mes.other.push_back("request:\tnumbertype=float");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		DEFAULT_Epsilon=stoi(val);
		DEFAULT_Epsilon_F=TLargeFloat("1e-"+to_string(DEFAULT_Epsilon));
		DEFAULT_Epsilon_f=stold("1e-"+to_string(DEFAULT_Epsilon));
	}
	else if(option=="charset")
	{
		if(val=="UTF8")
		{
			DEFAULT_chcp=65001;
		}
		else if(val=="GBK")
		{
			DEFAULT_chcp=936;
		}
		else
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="unknown variable for option \"charset\"";
			mes.other.push_back("your variable is "+val);
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
	else if(option=="valbreak")
	{
		if(val=="line")
		{
			DEFAULT_NumberBreak='\n';
		}
		else if(val=="space")
		{
			DEFAULT_NumberBreak=' ';
		}
		else if(val=="tab")
		{
			DEFAULT_NumberBreak='\t';
		}
		else if(val=="comma")
		{
			DEFAULT_NumberBreak=',';
		}
		else
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="unknown variable for option \"valbreak\"";
			mes.other.push_back("your variable is "+val);
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
	else if(option=="newline")
	{
		if(val=="true")
		{
			DEFAULT_Newline=true;
		}
		else if(val=="false")
		{
			DEFAULT_Newline=false;
		}
		else
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="unknown variable for option \"newline\"";
			mes.other.push_back("your variable is "+val);
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
	else if(option=="numbertype")
	{
		if(val=="float")DEFAULT_Vartype=VarType_TLargeFloat;
		else if(val=="float32")DEFAULT_Vartype=VarType_float;
		else if(val=="float64")DEFAULT_Vartype=VarType_double;
		else if(val=="int32")DEFAULT_Vartype=VarType_int;
		else if(val=="uint32")DEFAULT_Vartype=VarType_uint;
		else if(val=="int64")DEFAULT_Vartype=VarType_ll;
		else if(val=="uint64")DEFAULT_Vartype=VarType_ull;
		else if(val=="int")
		{
			WriteStringToConsole("抱歉，该解释器暂时不支持高精度整数。\n");
		}
	}
	else if(option=="fixed")
	{
		if(DEFAULT_Vartype==VarType_float||DEFAULT_Vartype==VarType_double||DEFAULT_Vartype==VarType_ldouble)
		{
			if(val=="true")
			{
				cout<<fixed;
			}
			else if(val=="false")
			{
				cout<<scientific;
			}
			else if(val=="auto")
			{
				cout<<defaultfloat;
			}
			else
			{
				mes.fun="void SET_(std::string & option,std::string & val)";
				mes.description="unknown variable for option \"fixed\"";
				mes.other.push_back("your variable is "+val);
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
		}
		else if(DEFAULT_Vartype==VarType_TLargeFloat)
		{
			WriteStringToConsole("抱歉，该解释器暂时不支持设置高精度浮点数的输出格式。");
		}
		else
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="this option request something";
			mes.other.push_back("request:\tnumbertype=float/float32/float64");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
	else if(option=="precision")
	{
		if(DEFAULT_Vartype==VarType_float||DEFAULT_Vartype==VarType_double||DEFAULT_Vartype==VarType_ldouble)
		{
			cout<<setprecision(stoi(val));
		}
		else
		{
			mes.fun="void SET_(std::string & option,std::string & val)";
			mes.description="this option request something";
			mes.other.push_back("request:\tnumbertype=float32/float64");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
	}
	else
	{
		mes.fun="void SET_(std::string & option,std::string & val)";
		mes.description="unknown option";
		mes.other.push_back("your option is "+option);
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
}

//输出变量
//作者：张子辰
void OutputVar(const BasicVar &var)
{
	switch (var.first)
	{
	case VarType_TLargeFloat:
		cout<<var.second.get<TLargeFloat>();break;
	case VarType_int:
		cout<<var.second.get<int>();break;
	case VarType_uint:
		cout<<var.second.get<unsigned int>();break;
	case VarType_ll:
		cout<<var.second.get<long long>();break;
	case VarType_ull:
		cout<<var.second.get<unsigned long long>();break;
	case VarType_float:
		cout<<var.second.get<float>();break;
	case VarType_double:
		cout<<var.second.get<double>();break;
	case VarType_ldouble:
		cout<<var.second.get<long double>();break;
	case VarType_char:
		cout<<var.second.get<char>();break;
	case VarType_string:
		WriteStringToConsole(var.second.get<string>());break;
	case VarType_vector:
	{
		cout<<"{";
		auto endit=var.second.get<vector<BasicVar>>().end();
		auto endit2=endit-1;
		for(auto i=var.second.get<vector<BasicVar>>().begin();
		i!=endit;i++)
		{
			OutputVar(*i);
			if(i!=endit2)cout<<",";
		}
		cout<<"}";
		break;
	}
	case VarType_void:
		cout<<"(void)";break;
	case VarType_bool:
		cout<<(var.second.get<bool>()?"true":"false");break;
	default:
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="void OutputVar(const BasicVar &var)";
		mes.description="this variable cannot be printed";
		mes.line=0;
		mes.other.push_back("[T"+to_string(var.first)+"]");
		err.push_back(mes);
		throw err;
	}
	}
}
