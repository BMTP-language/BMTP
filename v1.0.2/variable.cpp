//variable.cpp
//包含所有变量的定义
#include "declaration.h"
using namespace std;

vector<string> Lines;
vector<vector<string>> SourceCode;
vector<TypeList_t> TypeList;
map<string,SyntaxNode*> SyntaxForest;
std::map<std::pair<unsigned int,unsigned int>,TypeConversion_t> TypeConversion;
vector<New_t>New;
vector<CalcTwoVar_t> CalcTwoVar;
vector<SystemFunction_t> SystemFunction;
vector<int> SystemFunctionParameterCount;
set<string> Keywordlist={"INPUT","PRINT","IF","THEN","ELSE","END","WHILE",
"WEND","DO","LOOP","UNTIL","AND","OR","XOR","MOD","SET","VAR","TRUE","FALSE",
"DEF","RETURN","FEND","MAIN","SQR","ABS","SIN","COS","TAN","ARCSIN","ARCCOS",
"ARCTAN","LN","LG","EXP","FLOOR","CEIL","ROUND","FACT","PI","MAX","MIN","ROOT"};//暂时把系统函数视为关键字
