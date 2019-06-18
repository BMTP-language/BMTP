//analyse.cpp
//包含词法分析器、语法分析器和程序执行器
//作者：张子辰
#include "declaration.h"
using namespace std;

namespace
{
const set<char> signlist={'+','-','*','/','^','(',')','!','\\','<','>','=',',',';','_'};
bool issign(const char &ch)
{
	return signlist.count(ch);
}
}

void Lexer(int line_id)
{
	oneMessage mes;
	ErrorMessage error;
	const string &line=Lines[line_id];
	vector<string>result;
	string now_str="";//当前词素
	bool metdot=false;//在数字模式下是否已经遇到过小数点
	const int len=line.length();
	for(int i=0;i<len;)
	{
		//不可见字符一定造成分词
		if(isblank(line[i]))
		{
			if(now_str!="")
			{
				if(now_str!="")
					result.push_back(now_str);
				now_str="";
			}
			i++;
		}
		//单词
		else if(isalpha(line[i]))
		{
			while(i<len&&isalnum(line[i]))
			{
				now_str+=line[i];
				i++;
			}
			if(now_str!="")
				result.push_back(now_str);
			now_str="";
		}
		//数字
		else if(isdigit(line[i]))
		{
			metdot=false;
			while(i<len&&(isdigit(line[i])||line[i]=='.'))
			{
				if(line[i]=='.')
				{
					if(!metdot)metdot=true;
					else
					{
						//一个数字中含有多个小数点，引起错误
						mes.fun="void Lexer(const string &line)";
						mes.description="meet two dots in a number";
						mes.line=line_id;
						error.push_back(mes);
						throw error;
					}
				}
				now_str+=line[i];
				i++;
			}
			if(now_str!="")
				result.push_back(now_str);
			now_str="";
		}
		//字符串
		else if(line[i]=='\"')
		{
			now_str+='\"';
			i++;
			while(i<len&&line[i]!='\"')
			{
				if(line[i]=='\\')
				{
					i++;
					//处理转义符
					switch(line[i])
					{
					case 'n':now_str.push_back('\n');break;
					case 't':now_str.push_back('\t');break;
					case '\\':now_str.push_back('\\');break;
					case '\"':now_str.push_back('\"');break;
					default:
						//未知的转义符
						mes.fun="void Lexer(const string &line)";
						mes.description=string("unknown escape sequence: \\")+line[i];
						mes.line=line_id;
						error.push_back(mes);
						throw error;
					}
				}
				else
				{
					now_str.push_back(line[i]);
				}
				i++;
			}
			if(line[i]=='\"')
			{
				//不把后引号放入分词结束后的字符串中
				if(now_str!="")
					result.push_back(now_str);
				now_str="";
				i++;
			}
			else
			{
				//未找到字符串结束符
				mes.fun="void Lexer(const string &line)";
				mes.description=string("fail to find the stop mark of the string");
				mes.line=line_id;
				error.push_back(mes);
				throw error;
			}
		}
		else if(line[i]=='>'||line[i]=='<'||line[i]=='!')
		{
			now_str+=line[i];
			i++;
			if(line[i]=='=')
			{
				now_str+=line[i];
				i++;
			}
			if(now_str!="")
				result.push_back(now_str);
			now_str="";
		}
		else if(issign(line[i]))
		{
			now_str+=line[i];
			i++;
			if(now_str!="")
				result.push_back(now_str);
			now_str="";
		}
		else if(line[i]=='\'')
		{
			break;//行内注释
		}
		else
		{
			mes.fun="void Lexer(const string &line)";
			mes.description="unexpected character";
			mes.line=line_id;
			mes.other.push_back(string("the character is ")+line[i]+"("+to_string(int(line[i]))+")");
			error.push_back(mes);
			throw error;
			i++;
		}
	}
	if(int(SourceCode.size())<=line_id)
		SourceCode.resize(line_id+1);
	SourceCode[line_id]=result;
}

namespace { int TheInputIterator=0; }

inline vector<string> input_line()
{
	if(TheInputIterator>=(int)SourceCode.size()-1)
	{
		++TheInputIterator;
		//姑且用"@!@STOP"表示文件被读入完
		return {"@!@STOP"};
	}
	return SourceCode[++TheInputIterator];
}

bool GetCmpndStatement_if(SyntaxNode*);
void GetCmpndStatement_while(SyntaxNode*);
void GetCmpndStatement_do(SyntaxNode*);
void GetCmpndStatement_else(SyntaxNode*);
void GetCmpndStatement_fun(SyntaxNode*);
int GetOneStatement(SyntaxNode*);

inline int GetSentenceType(const string &x)
{
	if(x=="INPUT")return StateType_Input;
	if(x=="PRINT")return StateType_Print;
	if(x=="IF")return StateType_If;
	if(x=="DO")return StateType_Do;
	if(x=="WHILE")return StateType_While;
	if(x=="SET")return StateType_Set;
	if(x=="END")return StateType_End;
	if(x=="DEF")return StateType_Def;
	if(x=="VAL")return StateType_Var;
	if(x=="RETURN")return StateType_Return;
	return StateType_Move;
}

void Parser()
{
	SyntaxNode *node=new SyntaxNode;
	vector<string> sen;
	sen=input_line();
	while(sen.empty())sen=input_line();
	while(GetSentenceType(sen[0])==StateType_Def)
	{
		node->type=StateType_Fun;
		if(sen.size()!=3)
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="bool Parser(SyntaxNode *node)";
			mes.description="bad DEF-statement format";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
		node->name=sen[1];
		node->number=stoi(sen[2]);
		node->line_id=TheInputIterator;
		if(SyntaxForest.count(sen[1]))
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="bool Parser(SyntaxNode *node)";
			mes.description="the function has been defined";
			mes.line=TheInputIterator;
			mes.other.push_back("function name="+sen[1]);
			err.push_back(mes);
			throw err;
		}
		SyntaxForest[sen[1]]=node;
		node->body=new SyntaxNode;
		GetCmpndStatement_fun(node->body);
		node=new SyntaxNode;
		sen=input_line();
		while(sen.empty())sen=input_line();
	}
	TheInputIterator--;
	sen=input_line();
	while(sen.empty())sen=input_line();
	if(sen[0]!="@!@STOP")
	{
		node->type=StateType_Fun;//将整个程序视为一个函数
		node->name="";
		node->number=0;
		node->line_id=--TheInputIterator;
		node->body=new SyntaxNode;
		GetCmpndStatement_fun(node->body);
		SyntaxForest[""]=node;
	}
	else
	{
		delete node;
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool Parser(SyntaxNode *node)";
		mes.description="fail to find the main function";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
}

//获取一个语句
//简单语句：0
//EOF：-1
//END IF: 1
//WEND: 2
//ELSE: 3
//LOOP: 4
//FEND: 5
int GetOneStatement(SyntaxNode *node)
{
	oneMessage mes;
	ErrorMessage err;
	vector<string> sen;
	sen=input_line();
	if(sen.empty())
	{
		node->type=StateType_Empty;
		return 0;
	}
	if(sen[0]=="@!@STOP")return -1;
	//正常情况下，这些标识不可能是一个语句的开头
	//可能需要添加一些报错
	if(sen[0]=="END"&&sen.size()>1)
	{
		if(sen[1]=="IF"&&sen.size()==2)
			return 1;
		else
		{
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="expect IF after END";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
	}
	if(sen[0]=="ELSE")
	{
		if(sen.size()==1)
			return 3;
		else//报错
		{
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="unexpected words after ELSE";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
	}
	if(sen[0]=="WEND")
	{
		if(sen.size()==1)
			return 2;
		else//报错
		{
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="unexpected words after WEND";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
	}
	if(sen[0]=="FEND")
	{
		if(sen.size()==1)
			return 5;
		else//报错
		{
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="unexpected words after FEND";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
	}
	if(sen[0]=="LOOP")
	{
		if(sen.size()==1||sen[1]!="UNTIL")
		{
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="LOOP without UNTIL";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		}
		return 4;
	}
	int type=GetSentenceType(sen[0]);
	node->type=type;
	node->line_id=TheInputIterator;
	try
	{
		switch(type)
		{
		case StateType_Input:
		case StateType_Print:
		{
			if(sen[1][0]=='"')//存在输出输入提示
			{
				node->name=sen[1].substr(1);
				if(sen.size()>2)//提示内容后无分号，报错
				{
					if(sen[2]!=";")
					{
						mes.fun="bool GetOneStatement(SyntaxNode *node)";
						mes.description="expect \";\" in INPUT or PRINT statement";
						mes.line=TheInputIterator;
						err.push_back(mes);
						throw err;
					}
					vector<string>tmp;
					int bracks=0;
					for(size_t i=3;i<sen.size();i++)
					{
						if(sen[i][0]==','&&bracks==0)
						{
							node->var_list.push_back(MakeExpression(tmp));
							tmp.clear();
						}
						else
						{
							if(sen[i][0]=='(')bracks++;
							else if(sen[i][0]==')')bracks--;
							tmp.push_back(sen[i]);
						}
					}
					if(bracks!=0)
					{
						//括号不匹配
						mes.fun="bool GetOneStatement(SyntaxNode *node)";
						mes.description="unpaired bracks in INPUT or PRINT statement";
						mes.line=TheInputIterator;
						err.push_back(mes);
						throw err;
					}
					//如果tmp为空，说明语句结尾有多余的逗号
					node->var_list.push_back(MakeExpression(tmp));
				}
			}
			else
			{
				vector<string>tmp;
				int bracks=0;
				for(size_t i=1;i<sen.size();i++)
				{
					if(sen[i][0]==','&&bracks==0)
					{
						node->var_list.push_back(MakeExpression(tmp));
						tmp.clear();
					}
					else
					{
						if(sen[i][0]=='(')bracks++;
						else if(sen[i][0]==')')bracks--;
						tmp.push_back(sen[i]);
					}
				}
				if(bracks!=0)
				{
					//括号不匹配
					mes.fun="bool GetOneStatement(SyntaxNode *node)";
					mes.description="unpaired bracks in INPUT or PRINT statement";
					mes.line=TheInputIterator;
					err.push_back(mes);
					throw err;
				}
				//如果tmp为空，说明语句结尾有多余的逗号
				node->var_list.push_back(MakeExpression(tmp));
			}
		}
		break;
		case StateType_Move:
		{
			size_t i;
			vector<string>tmp;
			for(i=0;i<sen.size();i++)
			{
				if(sen[i]=="=")break;
				tmp.push_back(sen[i]);
			}
			node->left=MakeExpression(tmp);
			tmp.clear();
			if(sen[i]!="=")//报错
			{
				mes.fun="bool GetOneStatement(SyntaxNode *node)";
				mes.description="unknown statement type";
				mes.line=TheInputIterator;
				err.push_back(mes);
				throw err;
			}
			for(i++;i<sen.size();i++)
			{
				tmp.push_back(sen[i]);
			}
			node->expr=MakeExpression(tmp);
		}
		break;
		case StateType_If:
		{
			vector <string> tmp;
			if(sen[sen.size()-1]!="THEN")
			{
				mes.fun="bool GetOneStatement(SyntaxNode *node)";
				mes.description="expect THEN after IF or there are something else after THEN";
				mes.line=TheInputIterator;
				err.push_back(mes);
				throw err;
			}
			for(unsigned int i=1;i<sen.size()-1;i++)
			{
				tmp.push_back(sen[i]);
			}
			node->expr=MakeExpression(tmp);
			node->body=new SyntaxNode;
			if(GetCmpndStatement_if(node->body))//存在ELSE语句
			{
				node->body2=new SyntaxNode;
				GetCmpndStatement_else(node->body2);
			}
			else
			{
				node->body2=nullptr;
			}
		}
		break;
		case StateType_Do:
		{
			node->body=new SyntaxNode;
			GetCmpndStatement_do(node->body);
			sen=SourceCode[TheInputIterator];
			vector<string> tmp;
			for(unsigned int i=2;i<sen.size();i++)
			{
				tmp.push_back(sen[i]);
			}
			node->expr=MakeExpression(tmp);
		}
		break;
		case StateType_While:
		{
			vector<string> tmp;
			for(unsigned int i=1;i<sen.size();i++)
			{
				tmp.push_back(sen[i]);
			}
			node->expr=MakeExpression(tmp);
			node->body=new SyntaxNode;
			GetCmpndStatement_while(node->body);
		}
		break;
		case StateType_Set:
		{
			if(sen.size()!=3)
			{
				mes.fun="bool GetOneStatement(SyntaxNode *node)";
				mes.description="bad SET-statement format";
				mes.line=TheInputIterator;
				err.push_back(mes);
				throw err;
			}
			node->option=sen[1];
			node->name=sen[2];
		}
		break;
		case StateType_End:
			break;
		case StateType_Def:
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="a function cannot be defined inside another function";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
		case StateType_Return:
		{
			vector<string>tmp;
			for(unsigned int i=1;i<sen.size();i++)
			{
				tmp.push_back(sen[i]);
			}
			node->expr=MakeExpression(tmp);
			break;
		}
		case StateType_Var:
			if(sen.size()!=3||sen[1]!="array")
			{
				mes.fun="bool GetOneStatement(SyntaxNode *node)";
				mes.description="bad VAL-statement format";
				mes.line=TheInputIterator;
				err.push_back(mes);
				throw err;
			}
			node->name=sen[2];
			break;
		default://未知的语句类型，报错
			mes.fun="bool GetOneStatement(SyntaxNode *node)";
			mes.description="something strange happened, an unknown statement type was gotten";
			mes.line=TheInputIterator;
			err.push_back(mes);
			throw err;
			break;
		}
	}
	catch(ErrorMessage error)
	{
		for(auto &i:error)
		{
			if(!i.line)i.line=TheInputIterator;
		}
		throw error;
	}
	return 0;
}

//获取一个复合语句
bool GetCmpndStatement_if(SyntaxNode *node)
{
	node->type=StateType_Cmpnd;
	SyntaxNode *child=new SyntaxNode;
	int tmp;
	while((tmp=GetOneStatement(child))==0)
	{
		if(child->type!=StateType_Empty)
		{
			node->Cmpnd.push_back(child);
			child=new SyntaxNode;
		}
	}
	delete child;//最后一个分配的child一定不是一个合法的语句，占用的内存应释放
	if(tmp==1)return false;
	else if(tmp==3)return true;
	else if(tmp==-1)//未找到复合语句结束标志
	{

		oneMessage mes;
		ErrorMessage error;
		mes.fun="bool GetCmpndStatement_if(SyntaxNode *node)";
		mes.description="fail to find the end mark of an IF statement";
		mes.line=TheInputIterator;
		error.push_back(mes);
		throw error;
	}
	else if(tmp==2)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_if(SyntaxNode *node)";
		mes.description="IF statement ends with \"WEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==4)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_if(SyntaxNode *node)";
		mes.description="IF statement ends with \"LOOP\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==5)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_if(SyntaxNode *node)";
		mes.description="IF statement ends with \"FEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else
	{
		throw;
	}
}

void GetCmpndStatement_else(SyntaxNode *node)
{
	node->type=StateType_Cmpnd;
	SyntaxNode *child=new SyntaxNode;
	int tmp;
	while((tmp=GetOneStatement(child))==0)
	{
		if(child->type!=StateType_Empty)
		{
			node->Cmpnd.push_back(child);
			child=new SyntaxNode;
		}
	}
	delete child;//最后一个分配的child一定不是一个合法的语句，占用的内存应释放
	if(tmp==1)return;
	else if(tmp==-1)//未找到复合语句结束标志
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="bool GetCmpndStatement_else(SyntaxNode *node)";
		mes.description="fail to find the end mark of an IF statement";
		mes.line=TheInputIterator;
		error.push_back(mes);
		throw error;
	}
	else if(tmp==2)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_else(SyntaxNode *node)";
		mes.description="IF statement ends with \"WEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==3)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_else(SyntaxNode *node)";
		mes.description="too many \"ELSE\" in an IF statement";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==4)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_else(SyntaxNode *node)";
		mes.description="IF statement ends with \"LOOP\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==5)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_else(SyntaxNode *node)";
		mes.description="IF statement ends with \"FEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else
	{
		throw;
	}
}

void GetCmpndStatement_while(SyntaxNode *node)
{
	node->type=StateType_Cmpnd;
	SyntaxNode *child=new SyntaxNode;
	int tmp;
	while((tmp=GetOneStatement(child))==0)
	{
		if(child->type!=StateType_Empty)
		{
			node->Cmpnd.push_back(child);
			child=new SyntaxNode;
		}
	}
	delete child;//最后一个分配的child一定不是一个合法的语句，占用的内存应释放
	if(tmp==2)return;
	else if(tmp==-1)//未找到复合语句结束标志
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="bool GetCmpndStatement_while(SyntaxNode *node)";
		mes.description="fail to find the end mark of a WHILE statement";
		mes.line=TheInputIterator;
		error.push_back(mes);
		throw error;
	}
	else if(tmp==1)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_while(SyntaxNode *node)";
		mes.description="WHILE statement ends with \"END IF\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==3)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_while(SyntaxNode *node)";
		mes.description="WHILE statement ends with \"ELSE\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==4)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_while(SyntaxNode *node)";
		mes.description="WHILE statement ends with \"LOOP\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==5)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_while(SyntaxNode *node)";
		mes.description="WHILE statement ends with \"FEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else
	{
		throw;
	}
}

void GetCmpndStatement_do(SyntaxNode *node)
{
	node->type=StateType_Cmpnd;
	SyntaxNode *child=new SyntaxNode;
	int tmp;
	while((tmp=GetOneStatement(child))==0)
	{
		if(child->type!=StateType_Empty)
		{
			node->Cmpnd.push_back(child);
			child=new SyntaxNode;
		}
	}
	delete child;//最后一个分配的child一定不是一个合法的语句，占用的内存应释放
	if(tmp==4)return;
	else if(tmp==-1)//未找到复合语句结束标志
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="bool GetCmpndStatement_do(SyntaxNode *node)";
		mes.description="fail to find the end mark of a DO-statement";
		mes.line=TheInputIterator;
		error.push_back(mes);
		throw error;
	}
	else if(tmp==1)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_do(SyntaxNode *node)";
		mes.description="DO-statement ends with \"END IF\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==3)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_do(SyntaxNode *node)";
		mes.description="DO-statement ends with \"ELSE\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==2)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_do(SyntaxNode *node)";
		mes.description="DO-statement ends with \"WEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==5)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_do(SyntaxNode *node)";
		mes.description="DO-statement ends with \"FEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else
	{
		throw;
	}
}

void GetCmpndStatement_fun(SyntaxNode *node)
{
	node->type=StateType_Cmpnd;
	SyntaxNode *child=new SyntaxNode;
	int tmp;
	while((tmp=GetOneStatement(child))==0)
	{
		if(child->type!=StateType_Empty)
		{
			node->Cmpnd.push_back(child);
			child=new SyntaxNode;
		}
	}
	delete child;//最后一个分配的child一定不是一个合法的语句，占用的内存应释放
	if(tmp==5||tmp==-1)return;
	else if(tmp==1)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_fun(SyntaxNode *node)";
		mes.description="unpaired \"END IF\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==2)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_fun(SyntaxNode *node)";
		mes.description="unpaired \"WEND\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else if(tmp==3)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_fun(SyntaxNode *node)";
		mes.description="unpaired \"ELSE\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}

	else if(tmp==4)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="bool GetCmpndStatement_if(SyntaxNode *node)";
		mes.description="unpaired \"LOOP\"";
		mes.line=TheInputIterator;
		err.push_back(mes);
		throw err;
	}
	else
	{
		throw;
	}
}

SyntaxNode::SyntaxNode()
{
	type=StateType_Empty;
	line_id=-1;
}

//执行程序
BasicVar ProcessStaments(SyntaxNode *node)
{
	if(!node)return make_pair(VarType_void,pointer());
	if(node->type==StateType_Cmpnd)
	{
		for(auto i:node->Cmpnd)
		{
			ProcessStaments(i);
		}
		return make_pair(VarType_void,pointer());
	}
#ifdef _DEBUG
	//printf("[Line %d]",node->line_id);
#endif // _DEBUG
	try
	{
		switch(node->type)
		{
		case StateType_Input:
			INPUT_(node->name,node->var_list);
			break;
		case StateType_Print:
			PRINT_(node->name,node->var_list);
			break;
		case StateType_Move:
			*CalcExprLeft(node->left)=*CalcExpr(node->expr);
			break;
		case StateType_If:
			if(VarToBool(*CalcExpr(node->expr)))
			{
				ProcessStaments(node->body);
			}
			else
			{
				ProcessStaments(node->body2);
			}
			break;
		case StateType_While:
			while(VarToBool(*CalcExpr(node->expr)))
			{
				ProcessStaments(node->body);
			}
			break;
		case StateType_Do:
			do
			{
				ProcessStaments(node->body);
			} while(!VarToBool(*CalcExpr(node->expr)));
			break;
		case StateType_Set:
			SET_(node->option,node->name);
			break;
		case StateType_End:
			Exit(0);
			break;
		case StateType_Return:
			//暂时不允许函数返回引用
			return *CalcExpr(node->expr);
		case StateType_Var:
			DeclareLocalVar(node->name,VarType_vector);
		default:
			break;
		}
	}
	catch(ErrorMessage error)
	{
		for(auto &i:error)
		{
			if(!i.line)i.line=node->line_id;
		}
		throw error;
	}
	return make_pair(VarType_void,pointer());;
}

BasicVar ProcessFunction(SyntaxNode * node)
{
	if(node->type!=StateType_Fun)
	{
		throw;
	}
	InitFuctionCall();
	BasicVar tmp=ProcessStaments(node->body);
	InitFunctionReturn();
	return tmp;
}
