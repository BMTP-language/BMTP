#include "TLargeFloat.h"
//精度 
unsigned long DEFAULT_DigitsArraySize=100;
unsigned long DEFAULT_DigitsArraySize2=110;
unsigned long DEFAULT_Epsilon=100;
TLargeFloat DEFAULT_Epsilon_F = TLargeFloat("1.0e-100");
long double DEFAULT_Epsilon_f=1e-100;
double DEFAULT_Extradigit=0.1;
//默认的多个变量之间的分隔
char DEFAULT_NumberBreak='\n';
//完成一条输出语句后是否自动输出换行
bool DEFAULT_Newline=true;
unsigned int DEFAULT_Vartype=0;
int DEFAULT_chcp=65001;