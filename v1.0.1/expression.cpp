//expression.cpp
//包含与表达式计算有关的函数
//作者：山佩永
#include "declaration.h"
using namespace std;

namespace
{
const int prio[]={0,
5,5,4,4,3,
6,6,6,6,6,
6,4,2,7,9,
8,4,20,20,2,
2,1,1};

const int mu[]={0,
2,2,2,2,2,
2,2,2,2,2,
2,2,2,2,2,
2,2,2,2,1,
1,2,2};

inline int priority(int x)
{
	return prio[x];
}
// +1 -2 *3 /4 ^5 =6 !=7 <8 <=9 >10 >=11 \12 NOT13 AND14 OR15 XOR16 MOD17 (18 )19 -f20 +z21
inline smart_ptr<BasicVar> singlecalc(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y,int z)
{
	return CalcTwoVar[z](x,y);
}

}

Expression MakeExpression(vector<string> &exp)
{
	static stack<ExprNode> opr;
	static stack<int> parcnt;
	if(!exp.size())
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="Expression MakeExpression(vector<string> &exp)";
		mes.description="the expresion is empty";
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
	ExprNode node;
	Expression result;
	bool lastTokenisNum=false;
	//程序正常运行时不可能发生
	while(!opr.empty())opr.pop();
	for(auto i=exp.begin();i!=exp.end();i++)
	{
		//获得节点类型
		node=ExprNode();
		if(isalpha((*i)[0]))
		{
			if(IsFunction(*i))node.type=ExprType_Fun,node.variable=*i;
			else if(*i=="AND") node.type=ExprType_Opr,node.opr=Opr_And;
			else if(*i=="MOD")node.type=ExprType_Opr,node.opr=Opr_Mod;
			else if(*i=="OR")node.type=ExprType_Opr,node.opr=Opr_Orr;
			else if(*i=="XOR")node.type=ExprType_Opr,node.opr=Opr_Xor;
			else if(*i=="NOT")node.type=ExprType_Opr,node.opr=Opr_Not;
			else if(*i=="TRUE")node.type=ExprType_Num,node.number="@1";
			else if(*i=="FALSE")node.type=ExprType_Num,node.number="@0";
			else node.type=ExprType_Var,node.variable=*i;
		}
		else if(isdigit((*i)[0]))
		{
			node.type=ExprType_Num;
			node.number=*i;
		}
		else if((*i)[0]=='"')
		{
			node.type=ExprType_Num;
			node.number=*i;
		}
		else
		{
			node.type=ExprType_Opr;
			if(*i=="+")
			{
				if(i!=exp.begin()&&lastTokenisNum)
					node.opr=Opr_Add;
				else node.opr=Opr_Pos;
			}
			else if(*i=="-")
			{
				if(i!=exp.begin()&&lastTokenisNum)
					node.opr=Opr_Sub;
				else node.opr=Opr_Neg;
			}
			else if(*i=="*")node.opr=Opr_Mul;
			else if(*i=="/")node.opr=Opr_Div;
			else if(*i=="^")node.opr=Opr_Pow;
			else if(*i=="=")node.opr=Opr_Eql;
			else if(*i=="!=")node.opr=Opr_Neq;
			else if(*i=="<")node.opr=Opr_Les;
			else if(*i=="<=")node.opr=Opr_Leq;
			else if(*i==">")node.opr=Opr_Grt;
			else if(*i==">=")node.opr=Opr_Geq;
			else if(*i=="\\")node.opr=Opr_Idv;
			else if(*i=="(")node.opr=Opr_Lbr;
			else if(*i==")")node.opr=Opr_Rbr;
			else if(*i=="_")node.opr=Opr_Idx;
			else if(*i==",")node.opr=Opr_Cma;
			else
			{
				oneMessage mes;
				mes.fun="Expression MakeExpression(vector<string> &exp)";
				mes.description="unkown operator";
				mes.line=0;
				ErrorMessage err;
				err.push_back(mes);
				throw err;
			}
		}
		//////////////////////////
		//输出后缀表达式
		
		if(node.type==ExprType_Num||node.type==ExprType_Var)
		{
			result.push_back(node);
		}
		else if(node.type==ExprType_Opr)
		{
			if(node.opr==Opr_Lbr)
			{
				opr.push(node);
			}
			else if(node.opr==Opr_Rbr)
			{
				while(!opr.empty()&&opr.top().opr!=Opr_Lbr)
				{
					result.push_back(opr.top());
					opr.pop();
				}
				opr.pop();
				if(!opr.empty()&&opr.top().type==ExprType_Fun)
				{
					opr.top().opr=parcnt.top();
					parcnt.pop();
					//判断函数的参数个数是否为0，即是否有相连的括号
					if(*(i-1)!="(")opr.top().opr++;
					result.push_back(opr.top());
					opr.pop();
				}
			}
			else if(node.opr==Opr_Cma)
			{
				while(!opr.empty()&&opr.top().opr!=Opr_Lbr)
				{
					result.push_back(opr.top());
					opr.pop();
				}
				parcnt.top()++;
			}
			else if(node.opr==Opr_Pow)//唯一的结合性从右向左的运算符
			{
				while(!opr.empty()&&prio[opr.top().opr]<prio[node.opr])
				{
					result.push_back(opr.top());
					opr.pop();
				}
				opr.push(node);
			}
			else
			{
				while(!opr.empty()&&prio[opr.top().opr]<=prio[node.opr])
				{
					result.push_back(opr.top());
					opr.pop();
				}
				opr.push(node);
			}
		}
		else if(node.type==ExprType_Fun)
		{
			opr.push(node);
			parcnt.push(0);
		}
		//一个运算符是双目运算符的条件
		//1：它的上一个标记是右括号
		//2：它的上一个标记是数值
		//3：它的上一个标记是变量
		lastTokenisNum=((node.type==ExprType_Opr&&node.opr==Opr_Rbr||
			node.type==ExprType_Num||
			node.type==ExprType_Var)?
			true:false);
	}
	while(!opr.empty())
	{
		result.push_back(opr.top());
		opr.pop();
	}
	return result;
}

smart_ptr<BasicVar> CalcExpr(Expression &expr)
{
	smart_ptr<BasicVar> x;
	static vector<smart_ptr<BasicVar>> parlist;
	static stack<smart_ptr<BasicVar>> num;
	while(num.size())num.pop();
	for(auto i:expr)
	{
		switch(i.type)
		{
		case ExprType_Num:
		{
			smart_ptr<BasicVar> tmp;
			tmp.malloc();
			if(i.number[0]=='"')
			{
				pointer p=New[VarType_string]();
				p.get<string>()=i.number.substr(1);
				*tmp=make_pair(VarType_string,p);
			}
			else if(i.number[0]=='@')
			{
				pointer p=New[VarType_bool]();
				p.get<bool>()=(i.number[1]=='1'?true:false);
				*tmp=make_pair(VarType_bool,p);
			}
			else
			{
				pointer p=New[VarType_string]();
				p.get<string>()=i.number;
				*tmp=ExplicitTypeConversion(
					make_pair(VarType_string,p),DEFAULT_Vartype);
			}
			num.push(tmp);
		}
			//num.push(&i.number);
			break;
		case ExprType_Var:
			num.push(GetVar(i.variable));
			break;
		case ExprType_Opr:
			if(mu[i.opr]==1)
			{
				num.top()=CalcTwoVar[i.opr](nullptr,num.top());
			}
			else
			{
				x=num.top();
				num.pop();
				num.top()=CalcTwoVar[i.opr](num.top(),x);
			}
			break;
		case ExprType_Fun:
			parlist.resize(i.opr);
			for(int j=i.opr-1;j>=0;j--)
			{
				parlist[j]=num.top();
				num.pop();
			}
			num.push(CallFunction(i.variable,parlist));
		}
	}
	return num.top();
}

smart_ptr<BasicVar> CalcExprLeft(Expression &expr)
{
	smart_ptr<BasicVar> x;
	static vector<smart_ptr<BasicVar>> parlist;
	static stack<smart_ptr<BasicVar>> num;
	while(num.size())num.pop();
	for(auto i:expr)
	{
		switch(i.type)
		{
		case ExprType_Num:
		{
			smart_ptr<BasicVar> tmp;
			tmp.malloc();
			if(i.number[0]=='"')
			{
				pointer p=New[VarType_string]();
				p.get<string>()=i.number.substr(1);
				*tmp=make_pair(VarType_string,p);
			}
			else if(i.number[0]=='@')
			{
				pointer p=New[VarType_bool]();
				p.get<bool>()=(i.number[1]=='1'?true:false);
				*tmp=make_pair(VarType_bool,p);
			}
			else
			{
				pointer p=New[VarType_string]();
				p.get<string>()=i.number;
				*tmp=ExplicitTypeConversion(
					make_pair(VarType_string,p),DEFAULT_Vartype);
			}
			num.push(tmp);
		}
			break;
		case ExprType_Var:
			num.push(GetVarLeft(i.variable));
			break;
		case ExprType_Opr:
			if(mu[i.opr]==1)
			{
				num.top()=CalcTwoVar[i.opr](nullptr,num.top());
			}
			else
			{
				x=num.top();
				num.pop();
				num.top()=CalcTwoVar[i.opr==Opr_Idx?Opr_Idx_left:i.opr](num.top(),x);
			}
			break;
		case ExprType_Fun:
			parlist.resize(i.opr);
			for(int j=i.opr-1;j>=0;j--)
			{
				parlist[j]=num.top();
				num.pop();
			}
			num.push(CallFunction(i.variable,parlist));
		}
	}
	return num.top();
}