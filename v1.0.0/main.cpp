//main.cpp
//包含主函数
//作者：张子辰
#include <iostream>
#include <windows.h>
#include "option.h"
#include "declaration.h"

using namespace std;

void Exit(int id)
{
#ifdef _DEBUG
	system("pause");
#endif // _DEBUG
	exit(id);
}

void OutputErrorInfo(ErrorMessage error)
{
	for(auto i:error)
	{
		if(i.line)printf("Error(line %d)\n",i.line);
		else printf("Error(line \?\?)\n");
		printf("Fuction:\t%s\n",i.fun.c_str());
		printf("Description:\t%s\n",i.description.c_str());
		for(auto j:i.other)
		{
			printf("%s\n",j.c_str());
		}
		puts("");
	}
}

void OutputLexerResult()
{
	for(auto i:SourceCode)
	{
		puts("/////////////////////////////");
		for(auto j:i)
		{
			printf("%s\n",j.c_str());
		}
	}
	puts("/////////////////////////////");
}

void OutputExpression(const Expression &e)
{
	printf("$");
	for(auto i:e)
	{
		if(i.type==ExprType_Num)
		{
			putchar('[');
			cout<<i.number;
			printf("] ");
		}
		else if(i.type==ExprType_Opr)
		{
			printf("{%d}",i.opr);
		}
		else if(i.type==ExprType_Var)
		{
			printf("%s ",i.variable.c_str());
		}
		else if(i.type==ExprType_Fun)
		{
			printf("@%s@%d@ ",i.variable.c_str(),i.opr);
		}
	}
	putchar('$');
}

void TreeOut(SyntaxNode *now,int deepth=0)
{	
	if(!now)
	{
		for(int i=0;i<deepth;i++)printf("\t");
		printf("|--");
		puts("(empty)");
		return;
	}
	if(now->type==StateType_Cmpnd)
	{
		for(auto i:now->Cmpnd)
		{
			TreeOut(i,deepth);
		}
		return;
	}
	for(int i=0;i<deepth;i++)printf("\t");
	printf("|--");
	switch(now->type)
	{
	case StateType_Print:
		printf("PRINT ");
		WriteStringToConsole(now->name);
		printf(" ");
		for(auto i:now->var_list)
		{
			OutputExpression(i);
		}
		putchar('\n');
		break;
	case StateType_Input:
		printf("INPUT ");
		WriteStringToConsole(now->name);
		printf(" ");
		for(auto i:now->var_list)
		{
			OutputExpression(i);
		}
		putchar('\n');
		break;
	case StateType_If:
		printf("IF ");
		OutputExpression(now->expr);
		putchar('\n');
		TreeOut(now->body,deepth+1);
		printf("ELSE\n");
		TreeOut(now->body2,deepth+1);
		break;
	case StateType_Do:
		printf("DO ");
		OutputExpression(now->expr);
		putchar('\n');
		TreeOut(now->body,deepth+1);
		break;
	case StateType_While:
		printf("WHILE ");
		OutputExpression(now->expr);
		putchar('\n');
		TreeOut(now->body,deepth+1);
		break;
	case StateType_Move:
		printf("MOVE ");
		OutputExpression(now->left);
		OutputExpression(now->expr);
		putchar('\n');
		break;
	case StateType_End:
		puts("END");
		break;
	case StateType_Set:
		printf("SET %s %s\n",now->option.c_str(),now->name.c_str());
		break;
	default:
		break;
	}
}

void OutputSyntaxTree()
{
	for(auto i:SyntaxForest)
	{
		puts("******************************");
		TreeOut(i.second->body);
		puts("******************************");
	}
}

int main(int argc,char **argv)
{
	InitTypeList();
	InitNew();
	InitCalcTwoVar();
	InitSystemFunction();
#ifdef _DEBUG
	argc=2;
	argv[1]=new char[128];
	cin>>argv[1];
#endif // _DEBUG
	if(argc<2)
	{
		printf("No such file!");
		Exit(2);
	}
	try
	{
		LoadFile(string(argv[1]));
	}
	catch(const ErrorMessage &error)
	{
		//printf("An error has happend when loading program file.\n");
		OutputErrorInfo(error);
		Exit(-1);
	}
	for(int i=1;i<int(Lines.size());i++)
	{
		try
		{
			Lexer(i);
		}
		catch(const ErrorMessage &error)
		{
			//printf("An error has happend when lexical analysing Line %d.\n",i);
			OutputErrorInfo(error);
		}
	}
#ifdef _DEBUG
	OutputLexerResult();
	system("pause");
#endif // _DEBUG
	try
	{
		Parser();
	}
	catch(const ErrorMessage &error)
	{
		OutputErrorInfo(error);
		Exit(-1);
	}
	//Exit(0);
#ifdef _DEBUG
	OutputSyntaxTree();
	system("pause");
#endif // _DEBUG
	try
	{
		ProcessFunction(SyntaxForest[""]);
	}
	catch(const ErrorMessage &error)
	{
		OutputErrorInfo(error);
	}
	//其实应该报错（未找到END语句）
	Exit(0);
	return 0;
}