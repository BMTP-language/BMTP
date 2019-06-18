// TLargeFloat.h: interface for the TLargeFloat class.
//   超高精度浮点数类TLargeFloat
//   2004.03.28 by HouSisong@GMail.com
//////////////////////////////////////////////////////////////////////
// Update 2004.03.31 by HouSisong 
// ......
// Update 2007.11.29-12.20 by HouSisong  重构一遍,速度加快很多，我用它计算了上亿位的PI值:)


#ifndef _TLARGE_FLOAT_H__INCLUDED_
#define _TLARGE_FLOAT_H__INCLUDED_

extern unsigned long DEFAULT_DigitsArraySize2;
#include <vector>
#include <sstream>
#include <string>
#include <exception>
#include <limits>
#include <stdexcept>

class TLargeFloat;//超高精度浮点数类TLargeFloat
class TLargeFloatException;//超高精度浮点数异常类
class TLargeInt;//高精度整数类
class TLargeIntExpection;//高精度整数异常类

//改进方向：
//  1.强力优化ArrayMUL数组乘运算(当前实现了二分法和FFT算法)： 
//     a.将实数按齐偶作为复数进行傅立叶变换的算法实现，加快乘法速度
//     b.实现混合基的傅立叶变换，加快乘法速度
//     c.考虑用x87的10byte浮点数实现FFT以减小误差从而增大FFT能够计算的最大位数限制
//     d.用SSE2等优化快速复利叶变换，加快乘法速度  
//     e.或者将傅立叶变换替换为数论变换的实现(使用整数)
//  2.内部使用8位(或9位)十进制来实现，节约内存；或者2进制的底数(这样的话，输出函数就会麻烦一些了)
//  3.添加新的基本运算函数，如：指数运算power、对数运算log、三角函数sin,cos,tan等

//注意：如果浮点数与TLargeFloat进行混合运算, 可能会产生误差(有效位数会受到浮点数影响)；
//  整数 或 为可表示整数的浮点数 参与运算不会产生误差；
//  对于很大的整数或者有小数位的浮点数，建议用字符串的形式转换为超高精度浮点数(不会引入误差)

//void ArrayMUL(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize);//数组乘, 需要优化的首要目标

//超高精度浮点数异常类  //TLargeFloat运算中出现错误的时候会抛出该类型的异常
class TLargeFloatException :public std::runtime_error
{
public:
    TLargeFloatException(const char * Error_Msg) :std::runtime_error(Error_Msg){}
    virtual ~TLargeFloatException() throw() {}
};


//TCatchIntError只是对整数类型TInt进行的一层包装
//超高精度浮点数类的指数运算时使用
//设计TCatchIntError是为了当整数运算超出值域的时候，抛出异常
//template<要包装的整数类型，超界时抛出的异常类型，TInt最小值，TInt最大值>
template <typename TInt,typename TException,TInt MinValue,TInt MaxValue>
class TCatchIntError
{
private:
    typedef TCatchIntError<TInt,TException,MinValue,MaxValue> SelfType;
    TInt  m_Int;

    inline SelfType& inc(const TInt& uValue)
    {
        if ( (uValue<0)||(uValue>MaxValue)||(MaxValue-uValue<m_Int) )
            throw TException("ERROR:TCatchIntError::inc(); ");
        m_Int+=uValue;
        return (*this);
    }
    inline SelfType& dec(const TInt& uValue)
    {
        if ( (uValue<0)||(uValue>MaxValue)||(MinValue+uValue>m_Int) )
            throw TException("ERROR:TCatchIntError::dec()");
        m_Int-=uValue;
        return (*this);
    }
    inline SelfType& mul(const TInt& iValue)
    {
        if (iValue==0)
            m_Int=0;
        else
        {
            TInt tmp=m_Int*iValue;
            if ( (iValue<MinValue)||(iValue>MaxValue)||(tmp<MinValue)||(tmp>MaxValue)||((tmp/iValue)!=m_Int) )
                throw TException("ERROR:TCatchIntError::mul(); ");
            m_Int=tmp;
        }
        return (*this);
    }
public:
    inline TCatchIntError() :m_Int(0){ }
    inline TCatchIntError(const TInt& Value)       :m_Int(0) { (*this)+=Value;}
    inline TCatchIntError(const SelfType& Value)   :m_Int(0) { (*this)+=(Value.m_Int); }
    inline const TInt& AsInt() const    { return m_Int; }
    inline SelfType& operator +=(const TInt& Value) //throw(TLargeFloatException)
        { if (Value<0) return dec(-Value);
            else return inc(Value);   }
    inline SelfType& operator -=(const TInt& Value) //throw(TLargeFloatException)
        { if (Value<0)  return inc(-Value);
            else return dec(Value); }
    inline SelfType& operator *=(const TInt& Value) //throw(TLargeFloatException)
       {  return mul(Value); }

    inline SelfType& operator +=(const SelfType& Value) { return (*this)+=(Value.m_Int); }//throw(TLargeFloatException)
    inline SelfType& operator -=(const SelfType& Value) { return (*this)-=(Value.m_Int); }//throw(TLargeFloatException)
    inline SelfType& operator *=(const SelfType& Value) { return (*this)*=(Value.m_Int); }//throw(TLargeFloatException)
};

    //指数部分使用的整数类型  填写编译器支持的较大的整数类型 
    //typedef long long TMaxInt;   
    //const TMaxInt   TMaxInt_MAX_VALUE =   TMaxInt(9223372036854775807ll);
    //const TMaxInt   TMaxInt_MIN_VALUE = - TMaxInt_MAX_VALUE;
    typedef long  TMaxInt; 
    const TMaxInt  TMaxInt_MAX_VALUE =  2147483647;
    const TMaxInt  TMaxInt_MIN_VALUE = -TMaxInt_MAX_VALUE;

    //小数部分使用的整数类型  32bit位的整数类型 
    typedef long TInt32bit;
    const TInt32bit  TInt32bit_MAX_VALUE =  2147483647;
    const TInt32bit  TInt32bit_MIN_VALUE = -TInt32bit_MAX_VALUE;


//超高精度浮点数类
class TLargeFloat  
{
public:
    //一些常量和类型定义
    typedef std::vector<TInt32bit> TArray;//小数位使用的数组类型
    enum { em10Power =4,   //数组为10000进制，
           emBase =10000,  //数组的一个元素对应4个十进制位
           emLongDoubleDigits =std::numeric_limits<long double>::digits10,//long double的10进制有效精度
           emLongDoubleMaxExponent =std::numeric_limits<long double>::max_exponent10,//long double的最大10进制指数
           emLongDoubleMinExponent =std::numeric_limits<long double>::min_exponent10 };//long double的最小10进制指数
    typedef   TLargeFloatException   TException;
    typedef   TCatchIntError<TMaxInt,TException,TMaxInt_MIN_VALUE,TMaxInt_MAX_VALUE>  TExpInt;//指数类型
    typedef  TLargeFloat  SelfType;
public:
    class TDigits//TDigits用来设置TLargeFloat的精度;//增加这个类是为了避免TLargeFloat的构造函数的可能误用
    {
    private:
        unsigned long  m_DigitsArraySize;
    public:
        explicit TDigits(const long uiDigitsLength) 
        {   if (uiDigitsLength<=0) throw TException("ERROR:TLargeFloat::TDigits()");
            m_DigitsArraySize=(uiDigitsLength+em10Power-1)/em10Power; }
        inline const unsigned long  GetDigitsArraySize() const   { return m_DigitsArraySize; }
    };
    TLargeFloat(const SelfType& Value);
    TLargeFloat(const long double DefultValue);//默认浮点精度  //注意：转换可能存在小的误差
    explicit TLargeFloat(const long double DefultValue,const TDigits& DigitsLength);//TDigits 十进制有效位数 //注意：转换可能存在小的误差
    explicit TLargeFloat(const char* strValue);//使用字符串本身的精度
    explicit TLargeFloat(const char* strValue,const TDigits& DigitsLength);
    explicit TLargeFloat(const std::string& strValue);
    explicit TLargeFloat(const std::string& strValue,const TDigits& DigitsLength);
	inline TLargeFloat(const long long& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const double& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const float& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const long& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const int& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const short& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const unsigned long& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const unsigned int& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const unsigned short& DefultValue){ fToLargeFloat((long double)DefultValue); }
	inline TLargeFloat(const unsigned long long& DefultValue){ fToLargeFloat((long double)DefultValue); }
    explicit TLargeFloat();
    long double AsFloat() const;//转化为浮点数
    std::string  AsString() const;//转换为字符串 
    std::string AsString2() const;//原来的AsString函数，其返回为纯小数+指数的形式（eg. 0.1234300e2)，需转换 
    void SetDigitsLength(const TDigits& DigitsLength);  //重新设置10进制有效位数 
    inline void SetDigitsLength(const long uiDigitsLength) { SetDigitsLength(TDigits(uiDigitsLength)); }
    unsigned long GetDigitsLength() const;//返回当前的10进制有效位数

    void Swap(SelfType& Value);//交换值
    void Zero(); //设置为0

    inline void StrToLargeFloat(const std::string& strValue) { sToLargeFloat(strValue); }
    inline void StrToLargeFloat(const char* strValue) { sToLargeFloat(std::string(strValue)); }
    SelfType& operator =  (const SelfType& Value);
    SelfType& operator =  (long double  fValue); //注意：转换可能存在小的误差  
    inline const SelfType  operator -  () const  {  SelfType temp(*this); temp.Chs(); return temp;  }//求负
    inline const SelfType& operator +  () const  { return (*this); }//求正

    SelfType& operator += (const SelfType& Value);
    SelfType& operator -= (const SelfType& Value);
    inline SelfType& operator += (long double  fValue)  { return (*this)+=TLargeFloat(fValue); }
    inline SelfType& operator -= (long double  fValue)  { return (*this)-=TLargeFloat(fValue); }
    friend inline const TLargeFloat operator + (const TLargeFloat& x,const TLargeFloat& y)  { TLargeFloat temp(x);  return temp+=y; }
    friend inline const TLargeFloat operator - (const TLargeFloat& x,const TLargeFloat& y)  { TLargeFloat temp(x);  return temp-=y; }
    friend inline const TLargeFloat operator + (const TLargeFloat& x,long double y)  { TLargeFloat temp(x);  return temp+=y; }
    friend inline const TLargeFloat operator - (const TLargeFloat& x,long double y)  { TLargeFloat temp(x);  return temp-=y; }
    friend inline const TLargeFloat operator + (long double x,const TLargeFloat& y)  { return y+x; }
    friend inline const TLargeFloat operator - (long double x,const TLargeFloat& y)  { return -(y-x); }

    SelfType& operator *= (long double  fValue);
    SelfType& operator *= (const SelfType& Value); 
    friend inline const TLargeFloat operator * (const TLargeFloat& x,const TLargeFloat& y)  { TLargeFloat temp(x); if (&x!=&y)  return temp*=y; else { temp.Sqr(); return temp;  } }
    friend inline const TLargeFloat operator * (const TLargeFloat& x,long double y)  {TLargeFloat temp(x); return temp*=y;}
    friend inline const TLargeFloat operator * (long double x,const TLargeFloat& y)  { return y*x; }
    SelfType& operator /= (long double  fValue);
    SelfType& operator /= (const SelfType& Value);
    friend inline const TLargeFloat operator / (const TLargeFloat& x,const TLargeFloat& y)  { TLargeFloat temp(x); return temp/=y; }
    friend inline const TLargeFloat operator / (const TLargeFloat& x,long double y)  { TLargeFloat temp(x); return temp/=y; }
    friend inline const TLargeFloat operator / (long double x,const TLargeFloat& y)  { TLargeFloat temp(y); temp.Rev(); return temp*=x; }

	SelfType& operator %= (long double  fValue);
	SelfType& operator %= (const SelfType& Value);
	friend inline const TLargeFloat operator % (const TLargeFloat& x,const TLargeFloat& y)  { TLargeFloat temp(x); return temp%=y; }
	friend inline const TLargeFloat operator % (const TLargeFloat& x,long double y)  { TLargeFloat temp(x); return temp%=y; }
	friend inline const TLargeFloat operator % (long double x,const TLargeFloat& y)  { TLargeFloat temp(x); return temp%y; }

    friend inline bool operator ==(const TLargeFloat& x,const TLargeFloat& y) { return (x.Compare(y)==0); }
    friend inline bool operator < (const TLargeFloat& x,const TLargeFloat& y) { return (x.Compare(y)<0); }
    friend inline bool operator ==(const TLargeFloat& x,long double y) { return (x==TLargeFloat(y)); }
    friend inline bool operator < (const TLargeFloat& x,long double y) { return (x<TLargeFloat(y)); }
    friend inline bool operator ==(long double x,const TLargeFloat& y) { return (y==x); }
    friend inline bool operator < (long double x,const TLargeFloat& y) { return (y>x); }
    friend inline bool operator !=(const TLargeFloat& x,const TLargeFloat& y) { return !(x==y); }
    friend inline bool operator > (const TLargeFloat& x,const TLargeFloat& y) { return (y<x); }
    friend inline bool operator >=(const TLargeFloat& x,const TLargeFloat& y) { return !(x<y); }
    friend inline bool operator <=(const TLargeFloat& x,const TLargeFloat& y) { return !(x>y); }
    friend inline bool operator !=(const TLargeFloat& x,long double y) { return !(x==y); }
    friend inline bool operator > (const TLargeFloat& x,long double y) { return (y<x); }
    friend inline bool operator >=(const TLargeFloat& x,long double y) { return !(x<y); }
    friend inline bool operator <=(const TLargeFloat& x,long double y) { return !(x>y); }
    friend inline bool operator !=(long double x,const TLargeFloat& y) { return !(x==y); }
    friend inline bool operator > (long double x,const TLargeFloat& y) { return (y<x); }
    friend inline bool operator >=(long double x,const TLargeFloat& y) { return !(x<y); }
    friend inline bool operator <=(long double x,const TLargeFloat& y) { return !(x>y); }

    friend inline const TLargeFloat abs(const TLargeFloat& x)  { TLargeFloat result(x); result.Abs(); return result; }//绝对值,|x|
	friend inline const TLargeFloat sqrt(const TLargeFloat& x) { TLargeFloat result(x); result.SetDigitsLength(DEFAULT_DigitsArraySize2);result.Sqrt(); return result; } //开方,x^0.5 
    friend inline const TLargeFloat revsqrt(const TLargeFloat& x) { TLargeFloat result(x); result.SetDigitsLength(DEFAULT_DigitsArraySize2);result.RevSqrt(); return result; }//求1/x^0.5;
    friend inline const TLargeFloat sqr(const TLargeFloat& x) { TLargeFloat result(x); result.Sqr(); return result; };//平方,x^2
    friend inline void swap(TLargeFloat& a,TLargeFloat& b) { a.Swap(b); }//交换值
    friend inline std::ostream& operator << (std::ostream& cout, const TLargeFloat& Value) { return cout<<Value.AsString(); }
    friend inline std::istream& operator >> (std::istream& cin, TLargeFloat& Value){std::string tmp;cin>>tmp;Value.sToLargeFloat(tmp);return cin;}

///////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    TInt32bit   m_Sign;     //符号位  正:1,  负:-1, 零: 0
    TExpInt     m_Exponent; //保存10为底的指数
    TArray      m_Digits;   //小数部分 排列顺序是TArray[0]为第一个小数位(em10Power个10进制位),依此类推；取值范围[0--(emBase-1)]
private:
    void Canonicity();//规格化 转化值到合法格式
    void fToLargeFloat(long double fValue);//内部使用 浮点数转化为 TLargeFloat,并采用默认精度
    void iToLargeFloat(TMaxInt iValue);//内部使用 整数转化为 TLargeFloat,并采用默认精度
    void sToLargeFloat(const std::string& strValue);//内部使用 字符串转化为 TLargeFloat
    long Compare(const SelfType& Value) const;//比较两个数；(*this)>Value 返回1，小于返回-1，相等返回0
    void Abs_Add(const SelfType& Value);//绝对值加  x:=|x|+|y|;
    void Abs_Sub_Abs(const SelfType& Value);//绝对值减的绝对值x:=| |x|-|y| |;
    
public:
    void Chs();//求负
    void Abs();//绝对值
    void MulInt(TMaxInt iValue);//乘以一个整数；
    void DivInt(TMaxInt iValue);//除以一个整数；
    void Rev();//求倒数
    void RevSqrt();//求1/x^0.5;
    inline void Sqrt()  { SelfType x(*this);  x.RevSqrt(); (*this)*=x; } //求x^0.5;
    inline void Sqr()   { (*this)*=(*this); };//平方,x^2
////////////////////////////////////////////////////////////////////////////////////////
//以下内容由张子辰（E-mail: zichen350@gmail.com; blog: https://wcioqmara.blogspot.com ）添加，
//日期：20190402 
public:
	friend TLargeFloat floor(const TLargeFloat &x);
	friend TLargeFloat exp(const TLargeFloat &x);//指数e^x
	friend TLargeFloat ln(const TLargeFloat &x);//自然对数 lnx
	friend TLargeFloat sin(const TLargeFloat &x);//正弦 \sin x
	friend TLargeFloat cos(const TLargeFloat &x);//余弦 \cos x
	bool isInt()const;//是否为整数
	explicit operator int();
};
TLargeFloat pow(const TLargeFloat &x,const TLargeFloat &y);//乘方 x^y
inline TLargeFloat root(const TLargeFloat &x,const TLargeFloat &y)//开方 \sqrt[y]{x}
{
	return pow(x,TLargeFloat(1)/y);
}

inline TLargeFloat ceil(const TLargeFloat &x)
{
	if (x.isInt())return x;
	else return floor(x)+TLargeFloat(1);
}

inline TLargeFloat log(const TLargeFloat &x,const TLargeFloat &y)//对数 \log_{y}x
{
	return ln(x)/ln(y);
}

inline TLargeFloat tan(const TLargeFloat &x)//正切 \tan x
{
	return sin(x)/cos(x);
}

inline TLargeFloat cot(const TLargeFloat &x)//余切 \cot x
{
	return cos(x)/sin(x);
}

inline TLargeFloat sec(const TLargeFloat &x)//正割 \sec x
{
	return TLargeFloat(1)/cos(x);
}

inline TLargeFloat ces(const TLargeFloat &x)//余割 \ces x
{
	return TLargeFloat(1)/sin(x);
}

/*
TLargeFloat arcsin(const TLargeFloat &x);//反正弦 \arcsin x
TLargeFloat arccos(const TLargeFloat &x);//反余弦 \arccos x
TLargeFloat arctan(const TLargeFloat &x);//反正切 \arctan x
TLargeFloat arccot(const TLargeFloat &x);//反余切 \arccot x
TLargeFloat arcsec(const TLargeFloat &x);//反正割 \arcsec x
TLargeFloat arcces(const TLargeFloat &x);//反余割 \arcces x
TLargeFloat fact(const TLargeFloat &x);//阶乘 x!*/

#endif // _TLARGE_FLOAT_H__INCLUDED_
// TLargeFloat.h
