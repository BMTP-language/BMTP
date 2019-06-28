//declaration.h
//包含所有函数、类型的声明

#ifndef _INCLUDE_DECLARATION_H_
#define _INCLUDE_DECLARATION_H_
#include "include.h"
#include "option.h"

#define StateType_Cmpnd 0//复合语句
#define StateType_If 1//IF语句
#define StateType_While 2//WHILE语句
#define StateType_Do 3//DO...LOOP UNTIL语句
#define StateType_Input 4//INPUT语句
#define StateType_Print 5//PRINT语句
#define StateType_Move 6//赋值语句
#define StateType_Set 7//设置语句
#define StateType_Empty 8//空行
#define StateType_Def 9//定义函数
#define StateType_Fun 9
#define StateType_Return 10//返回语句
#define StateType_Var 11//定义变量
#define StateType_End 127//终止语句

#define ExprType_Opr 1
#define ExprType_Num 2
#define ExprType_Var 3
#define ExprType_Fun 4

#define VarType_TLargeFloat 0
#define VarType_int 1
#define VarType_uint 2
#define VarType_ll 3
#define VarType_ull 4
#define VarType_short 5
#define VarType_ushort 6
#define VarType_float 7
#define VarType_double 8
#define VarType_ldouble 9
#define VarType_char 10
#define VarType_string 11
#define VarType_vector 12
#define VarType_bool 13
#define VarType_void 14

#define TypeType_int 1
#define TypeType_float 2
#define TypeType_complex 3
#define TypeType_quaternion 4
#define TypeType_string 5
#define TypeType_char 6
#define TypeType_array 7
#define TypeType_other 8

#define Opr_Add 1// +
#define Opr_Sub 2// -
#define Opr_Mul 3// *
#define Opr_Div 4// /
#define Opr_Pow 5// ^
#define Opr_Eql 6// =
#define Opr_Neq 7// !=
#define Opr_Les 8// <
#define Opr_Leq 9// <=
#define Opr_Grt 10// >
#define Opr_Geq 11// >=
#define Opr_Idv 12// \ (向下取整的除)
#define Opr_Not 13// NOT
#define Opr_And 14// AND
#define Opr_Orr 15// OR
#define Opr_Xor 16// XOR
#define Opr_Mod 17// MOD
#define Opr_Lbr 18// (
#define Opr_Rbr 19// )
#define Opr_Neg 20// +(单目)
#define Opr_Pos 21// -(单目)
#define Opr_Idx 22// _(数值寻址)
#define Opr_Idx_left 23
#define Opr_Cma 100//逗号

#define Fun_SQR 1
#define Fun_ABS 2
#define Fun_SIN 3
#define Fun_COS 4
#define Fun_TAN 5
#define Fun_ARCSIN 6
#define Fun_ARCCOS 7
#define Fun_ARCTAN 8
#define Fun_LN 9
#define Fun_LG 10
#define Fun_EXP 11
#define Fun_FLOOR 12
#define Fun_CEIL 13
#define Fun_ROUND 14
#define Fun_FACT 15
#define Fun_PI 16
#define Fun_MAX 17
#define Fun_MIN 18
#define Fun_ROOT 19

/////////////////////////////////////////////
//类型声明
struct oneMessage
{
	std::string fun;//发生错误的函数
	std::string description;//发生错误描述
	std::vector<std::string> other;//其他错误信息
	int line;
};

typedef	std::vector<oneMessage> ErrorMessage;

//半类型安全的智能指针
//“半类型安全”：同一个指针类型可以指向多种数据类型，但除非被重新赋值，同一个对象只能指向一种数据类型
//如：
//pointer p=New[VarType_TLargeFloat](),q=New[VarType_string]();
//cin>>q.get<string>();
//p.get<TLargeFloat>()=TLargeFloat("123456789");
//p.get<TLargeFloat>()/=TLargeFloat("987654321");
//cout<<q.get<string><<":"<<p.get<TLargeFloat>()<<endl;//合法
//q=p;//合法
//q.get<string>()="";//非法
//q.get<TLargeFloat>()+=p.get<TLargeFloat>();//合法
class pointer
{
public:
	pointer();
	~pointer();
	pointer(const pointer&);
	pointer& operator=(const pointer&);
	template<typename tp>
	inline tp& get()const
	{
		return *((tp*)m_data);
	}
	//分配一定长度的空间
	friend pointer New_tlargefloat();
	friend pointer New_string();
	friend pointer New_vector();
	friend pointer New_size1();
	friend pointer New_size2();
	friend pointer New_size4();
	friend pointer New_size8();
	friend pointer New_size16();
	friend pointer MemCopy(const pointer&);
	//回收内存
	friend void Delete(const pointer&);
	
private:
	void* m_data;
	int* shared_count;
	int ptr_type;
};

//存储变量的通用数据类型
typedef std::pair<unsigned int,pointer> BasicVar;

//动态分配内存的函数
typedef pointer (*New_t)();

//半动态智能指针
//半动态指该指针的对象指向的内存空间既可以是动态分配的
//如int *p=new int(2);
//也可以是非动态分配的
//如vector<int> a={1,2,3};
//int *p=&a[1];
//作者：张子辰
template<typename tp>
class smart_ptr
{
public:
	smart_ptr():m_data(nullptr),shared_cnt(nullptr),deleteable(false)
	{}
	smart_ptr(const smart_ptr &other)
	{
		m_data=other.m_data;
		shared_cnt=other.shared_cnt;
		deleteable=other.deleteable;
		if(deleteable&&shared_cnt!=nullptr)
		{
			(*shared_cnt)>0?++(*shared_cnt):--(*shared_cnt);
		}
	}
	smart_ptr(tp* other)
	{
		m_data=other;
		shared_cnt=new int(1);
		deleteable=false;
	}
	~smart_ptr()
	{
		if(deleteable&&shared_cnt)
		{
			if(*shared_cnt<0)
			{
				if(++(*shared_cnt)==0)
					delete shared_cnt;
			}
			else
			{
				if(--(*shared_cnt)==0)
				{
					delete m_data;
					delete shared_cnt;
				}
			}
		}
	}
	smart_ptr& operator=(const smart_ptr &other)
	{
		if(other.deleteable&&other.shared_cnt)
		{
			(*other.shared_cnt)>0?++(*other.shared_cnt):--(*other.shared_cnt);
		}
		if(deleteable&&shared_cnt)
		{
			if(*shared_cnt<0)
			{
				if(++(*shared_cnt)==0)
					delete shared_cnt;
			}
			else
			{
				if(--(*shared_cnt)==0)
				{
					delete m_data;
					delete shared_cnt;
				}
			}
		}
		m_data=other.m_data;
		shared_cnt=other.shared_cnt;
		deleteable=other.deleteable;
		return *this;
	}
	smart_ptr& operator=(tp* other)
	{
		if(deleteable&&shared_cnt)
		{
			if(*shared_cnt<0)
			{
				if(++(*shared_cnt)==0)
					delete shared_cnt;
			}
			else
			{
				if(--(*shared_cnt)==0)
				{
					delete m_data;
					delete shared_cnt;
				}
			}
		}
		m_data=other;
		shared_cnt=nullptr;
		deleteable=false;
	}
	void malloc(const tp &default_var=tp())
	{
		if(deleteable&&shared_cnt)
		{
			if(*shared_cnt<0)
			{
				if(++(*shared_cnt)==0)
					delete shared_cnt;
			}
			else
			{
				if(--(*shared_cnt)==0)
				{
					delete m_data;
					delete shared_cnt;
				}
			}
		}
		m_data=new tp(default_var);
		shared_cnt=new int(1);
		deleteable=true;
	}
	void free()
	{
		if(deleteable)
		{
			if(shared_cnt&&(*shared_cnt)<0)
				return;
			delete m_data;
			(*shared_cnt)=-(*shared_cnt);
		}
	}
	tp& operator*()const
	{
		return *m_data;
	}
	tp* operator->()const
	{
		return m_data;
	}
private:
	tp *m_data;
	int *shared_cnt;
	bool deleteable;//表示该对象对应的内存空间是否可以被delete释放
};

struct TypeList_t
{
	TypeList_t(){size=-1,typetype=-1,subtypetype=-1;}
	TypeList_t(size_t size_,int typetype_,int subtypetype_):
		size(size_),typetype(typetype_),subtypetype(subtypetype_)
		{}
	size_t size;//类型大小
	//类型的类型（包括整数、浮点数、复数、四元数、数组、字符、字符串、其他）
	int typetype;
	//类型的类型的子类型，按照数值的增大有功能加强
	//整数：ushort<short<uint<int<ulong long<long long<uint128<int128<高精度
	//浮点数：float<double<long double<高精度
	//复数：complex<float> < complex<double> < complex<long double> < complex<高精度>
	//四元数：与复数类似
	//数组：一维<二维<三维<...
	//字符：char<wchar_t
	//字符串：string
	int subtypetype;
};

//变量转换函数
typedef pointer (*TypeConversion_t)(const pointer&);
//变量计算函数
typedef smart_ptr<BasicVar>(*CalcTwoVar_t)(smart_ptr<BasicVar>,smart_ptr<BasicVar>);

//调用系统函数的函数
typedef smart_ptr<BasicVar>(*SystemFunction_t)(const std::vector<smart_ptr<BasicVar>>&);
//表达式节点
struct ExprNode
{
	int type;
	//tpye==ExprNode_Opr时，表示运算符的类型
	//type==ExprNode_Fun时，表示函数的参数个数
	int opr;
	//type==ExprNode_Var时，表示变量名
	//type==ExprNode_Fun时，表示函数名
	std::string variable;
	std::string number;//将立即数推迟的计算时转换，可以防止中途numbertype改变而导致类型出错
};

//（中缀）表达式
typedef std::vector<ExprNode> Expression;

//语法分析树的节点
struct SyntaxNode
{
	SyntaxNode();
	char type;
	int line_id;
	std::vector<SyntaxNode*> Cmpnd;//子语句列表，复合语句参数
	Expression expr;//IF/WHILE/DO语句的条件 或 赋值语句的表达式
	Expression left;//赋值语句的变量
	SyntaxNode *body;//循环体或THEN后执行的语句
	SyntaxNode *body2;//ELSE后执行的语句（可以为空）
	std::string name;//设置语句的参数 或 函数定义语句的函数名 或 输出输入语句输出的内容
	int number;//函数的参数个数
	std::string option;//设置语句的选项
	std::vector<Expression> var_list;//输出输入语句的变量列表
};
////////////////////////////////////////////////////

//变量声明

//储存分词后的所有代码，
//sentences[i]表示第i行的所有单词，
//sentences[i][j]为第i行的第j个单词。
//i的编号从1开始，j的编号从0开始。
extern std::vector<std::vector<std::string>> SourceCode;

//读取源文件后，将源文件按行储存在Lines中
extern std::vector<std::string> Lines;

extern std::map<std::string,SyntaxNode*> SyntaxForest;//一个函数对应的语法分析树的节点

extern std::vector<TypeList_t> TypeList;//变量类型列表，储存各个类型变量的大小

//BasicType TypeConversion[{from,to}](BasicType)可以将类型from转换为类型to
extern std::map<std::pair<unsigned int,unsigned int>,TypeConversion_t> TypeConversion;  

extern std::vector<New_t> New;

extern std::vector<CalcTwoVar_t> CalcTwoVar;

extern std::vector<SystemFunction_t> SystemFunction;
extern std::vector<int> SystemFunctionParameterCount;

extern std::set<std::string> Keywordlist;

////////////////////////////////////////////////
//函数声明

///////////////////////

//获取文件拓展名
//filename:文件名 
//返回：
//含有. 
//不含大写字母 
//eg.
//FileType("example.eG")=".eg"
std::string FileType(const std::string &filename);


//加载文件，将结果储存在全局变量Lines中
void LoadFile(std::string FileName);
///////////////////////


//处理INPUT语句
//hint:提示语句
//var:待输入的变量
void INPUT_(std::string &hint,std::vector<Expression> &var);

//处理PRINT语句
void PRINT_(std::string &hint,std::vector<Expression> &var);

//输出一个变量
void OutputVar(const BasicVar&);

//处理一条设置语句
//option：设置参数
//val：参数的值
void SET_(std::string &option,std::string &val);

//////////////////////

//将一行源代码分割为词素，结果储存在全局变量SourceCode中。
//////////////
//特别说明：//
//////////////
//1.当词素类型为字符串时，分词结果只保留前双引号
//2.字符串中的转义符已处理
void Lexer(int line_id);

//建立语法分析树
void Parser();

//执行
BasicVar ProcessStaments(SyntaxNode *node);
BasicVar ProcessFunction(SyntaxNode *node);

//////////////////////
void DeclareGlobalVar(const std::string&,const int&);//声明全局变量
void DeclareLocalVar(const std::string&,const int&);//声明局部变量
void InitFuctionCall();//调用函数时调用
void InitFunctionReturn();//函数结束时调用
void InitCompoundBegin();//复合语句开始时调用
void InitCompoundEnd();//复合语句结束时调用
BasicVar ImplicitTypeConversion(const BasicVar&,const unsigned int&);//隐性转化变量类型
BasicVar ExplicitTypeConversion(const BasicVar&,const unsigned int&);//显式转化变量类型
smart_ptr<BasicVar> GetVar(const std::string&);//获得变量的值
smart_ptr<BasicVar> GetVarLeft(const std::string&);
void SetVar(const std::string&,const BasicVar&);//为变量赋值
void Delete(const pointer&);//回收内存
pointer New_tlargefloat();
pointer New_string();
pointer New_vector();
pointer New_size1();
pointer New_size2();
pointer New_size4();
pointer New_size8();
pointer New_size16();
pointer MemCopy(const pointer&);


/////////////////////

inline bool VarToBool(const BasicVar &n)
{
	if(n.first==VarType_bool)
	{
		return n.second.get<bool>();
	}
	oneMessage mes;
	ErrorMessage err;
	mes.fun="bool TLargeFloatToBool(const TLargeFloat &n)";
	mes.description="expect a bool variable";
	mes.line=0;
	err.push_back(mes);
	throw err;
}

//计算表达式
smart_ptr<BasicVar> CalcExpr(Expression&);
smart_ptr<BasicVar> CalcExprLeft(Expression&);

//将标识符数组转化为表达式
Expression MakeExpression(std::vector<std::string>&);

//退出程序
void Exit(int id);

//初始化基本变量
void InitTypeList();
//初始化pointer的内存动态分配
void InitNew();
//初始化变量间两两的运算
void InitCalcTwoVar();
//初始化系统函数
void InitSystemFunction();

//调用一个函数
smart_ptr<BasicVar> CallFunction(const std::string&,const std::vector<smart_ptr<BasicVar>>&);
//获取一个函数的参数个数
int GetParameterCnt(const std::string&);
bool IsFunction(const std::string&);

//从控制台读入一个字符
char GetCharFromConsole();
//从控制台读入一个字符串
std::string ReadStringFromConsole();
//向控制台输出一个字符串
void WriteStringToConsole(const std::string&);
#endif