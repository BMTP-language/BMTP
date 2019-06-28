// TLargeFloat.cpp: implementation of the TLargeFloat class.
//   超高精度浮点数类TLargeFloat
//////////////////////////////////////////////////////////////////////

#include "TLargeFloat.h"
#include <algorithm>
#include <iostream>
//#include "assert.h"
#include <math.h>
#include "option.h"

//#define MyDebugAssert(b) { if (!(b)) __asm{ int 3 } }

//用以控制运算精度的工具
class TDigitsLengthCtrl
{
private:
    std::vector<TLargeFloat*> m_list;
public:
    inline void add_to_ctrl(TLargeFloat* var) { m_list.push_back(var); }
    void SetDigitsLength(const long uiDigitsLength) 
    { 
        long size=m_list.size();
        for (long i=0;i<size;++i)
            m_list[i]->SetDigitsLength(uiDigitsLength);
    }
};

//自动精度控制
class TDigitsLengthAutoCtrl:public TDigitsLengthCtrl
{
private:
    std::vector<long>    m_DigitsLengthList;
    long   step;
public:
    TDigitsLengthAutoCtrl(long DigitsMul,long DigitsLength0,long MaxDigitsLength)//(收敛系数,初始精度,最终需要精度)
    {
        //MyDebugAssert(DigitsLength0>0);
        //得到最佳的精度梯度
        long curDigitsLength=MaxDigitsLength;
        while (curDigitsLength>DigitsLength0)
        {
            m_DigitsLengthList.push_back(curDigitsLength);

            if ((DigitsLength0<=1)&&(curDigitsLength==1)) break;
            curDigitsLength=(curDigitsLength+DigitsMul-1)/DigitsMul;
        }
        step=get_runcount()-1;
    }
    inline long get_runcount() { return m_DigitsLengthList.size(); }
    inline void SetNextDigits()
    {
        //MyDebugAssert(step>=0);
        long DigitsLength=m_DigitsLengthList[step];
        if (step>0)
            DigitsLength+=4;
        SetDigitsLength(DigitsLength);
        --step;
        if (step<0) step=0;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////

void TLargeFloat_CtrlExponent(TLargeFloat::TArray& v,TMaxInt iMoveRightCount)
{
    //iMoveCountBig 有可能存不下 但现在的体系下因右移超过值域比较难
    long iMoveCountBig=(long)(iMoveRightCount/TLargeFloat::em10Power);
    long iMoveCountSmall=(long)(iMoveRightCount%TLargeFloat::em10Power);

    //大的右移
    if (iMoveCountBig>0)
    {
        for (long i=v.size()-1;i>=iMoveCountBig;--i)
            v[i]=v[i-iMoveCountBig];
        for (long j=0;j<iMoveCountBig;++j)
            v[j]=0;
    }
    //小的右移
    if (iMoveCountSmall>0)
    {
        TInt32bit iDiv=1;
        for (long j=0;j<iMoveCountSmall;++j) iDiv*=10;

        long v_size=v.size();
        for (long i=iMoveCountBig;i<v_size-1;++i)
        {
            v[i+1]+=(v[i]%iDiv)*TLargeFloat::emBase;
            v[i]/=iDiv;
        }
        if (v_size>=1)
            v[v_size-1]/=iDiv;
    }
    
}

    void TLargeFloat_ArrayAddTestLast(TInt32bit* x,long xsize)
    {
        for (long i=xsize-1;i>0;--i)
        {
            if (x[i]>=TLargeFloat::emBase)//进位
            {
                ++x[i-1];
                x[i]-=TLargeFloat::emBase;
            }
            else
                break;
        }   
    }

void TLargeFloat_ArrayAdd(TInt32bit* result,long rsize,const TInt32bit* y,long ysize)
{
    //MyDebugAssert(rsize>=ysize);
    //MyDebugAssert(ysize>0);
    TInt32bit* ri=&result[rsize-ysize];
    long i;
    for (i=ysize-1;i>0;--i)
    {
        TInt32bit r=ri[i]+y[i];
        if (r>=TLargeFloat::emBase)//进位
        {
            ++ri[i-1];
            r-=TLargeFloat::emBase;
        }
        ri[i]=r;
    }    
    ri[0]+=y[0];
  
    TLargeFloat_ArrayAddTestLast(result,rsize-ysize+1);
}

void TLargeFloat_ArraySub(TInt32bit* result,long rsize,const TInt32bit* y,long ysize)
{
    //MyDebugAssert(rsize>=ysize);
    //MyDebugAssert(ysize>0);
    TInt32bit* ri=&result[rsize-ysize];
    long i;
    for (i=ysize-1;i>0;--i)
    {
        TInt32bit r=ri[i]-y[i];
        if (r<0)//借位
        {
            --ri[i-1];
            r+=TLargeFloat::emBase;
        }
        ri[i]=r;
    }    
    ri[0]-=y[0];
  
    for (i=rsize-ysize;i>0;--i)
    {
        if (result[i]<0)//借位
        {
            --result[i-1];
            result[i]+=TLargeFloat::emBase;
        }
        else
            break;
    }   
}


        TInt32bit _inti_csMaxMuliValue()
        {
            double rb=1; double mb=1;
            for (long i=0;i<100/TLargeFloat::em10Power;++i)
            {
                mb*=(1.0/TLargeFloat::emBase);
                rb+=mb;
            }
            return (TInt32bit)( TInt32bit_MAX_VALUE*(1.0/(TLargeFloat::emBase-1))/rb ) -1;
        }
    //不超界所允许的乘数值M满足: (emBase-1)*M + (emBase-1)*M/B + (emBase-1)*M/B^2 + (emBase-1)*M/B^3 ... <=TInt32bit_MAX_VALUE
    static const TInt32bit csMaxMuliValue=_inti_csMaxMuliValue();
    static const TInt32bit csMaxMulAddValue = csMaxMuliValue - (TLargeFloat::emBase-1);

void TLargeFloat_ArrayMUL_ExE(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize)
{
    //N*N复杂度的简单乘法实现
    for (long k=0;k<rminsize;++k) result[k]=0;
    while ((ysize>0)&&(y[0]==0)) { --ysize; ++y; ++result; --rminsize;}
    //while ((xsize>0)&&(x[0]==0)) { --xsize; ++x; ++result; --rminsize;}
    while ((ysize>0)&&(y[ysize-1]==0)) { --ysize; }
    //while ((xsize>0)&&(x[xsize-1]==0)) { --xsize; }

    long add_sum=0;
    for (long i=0;i<xsize;++i)
    {
        long xi=x[i];
        if (xi>0)
        {
            TInt32bit* resulti=&result[i+1];
            long y_limit=std::min(rminsize-1-i,ysize);
            for (long j=0;j<y_limit;++j)
            {
                resulti[j]+=(xi*y[j]);
            }
            add_sum+=xi;
            if (add_sum>csMaxMulAddValue)
            {
                for (long k=i+y_limit;k>0;--k)
                {
                    TInt32bit v=result[k];
                    if (v>=TLargeFloat::emBase)
                    {
                        result[k-1]+=v/TLargeFloat::emBase;
                        result[k]= v%TLargeFloat::emBase;
                    }
                }
                add_sum=0;
            }
        }
    }
    if (add_sum>0)
    {
        for (long k=rminsize-1;k>0;--k)
        {
            TInt32bit v=result[k];
            if (v>=TLargeFloat::emBase)
            {
                result[k-1]+=v/TLargeFloat::emBase;
                result[k]= v%TLargeFloat::emBase;
            }
        }
    }
}

//数组乘法  核心
void ArrayMUL(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize);


void ArrayMUL_Dichotomy(TInt32bit* result,long rminsize,const TInt32bit* x,const TInt32bit* y,long MulSize)
{

    //二分法的乘法实现
    //   x*y=(a*E+b)*(c*E+d)=(E*E-E)*ac + E*(a+b)*(c+d) -(E-1)*bd

    //   temp_a_add_b=a+b;
    //   temp_c_add_d=c+d;
    //   result=E*temp_a_add_b*temp_c_add_d;

    //   temp_ac=a*c;
    //   result+=E*E*temp_ac;
    //   result-=E*temp_ac;

    //   temp_bd=b*d;
    //   result+=temp_bd;
    //   result-=E*temp_bd;
    // 
    long i;

    const long E=((MulSize+1)>>1);
    //if (tmpData+2*(E+1)>end_tmpData)  __asm int 3
    const long acSize=MulSize-E;
    const long acErrorSize=E-acSize; 

    const TInt32bit* a=x;
    const TInt32bit* b=&x[acSize];
    const TInt32bit* c=y;
    const TInt32bit* d=&y[acSize];

    TLargeFloat::TArray _tmpData((E+1)*2);
    TInt32bit* tmpData=&_tmpData[0];

    TInt32bit* temp_a_add_b=&tmpData[0];
    TInt32bit* temp_c_add_d=&tmpData[(E+1)];

    temp_a_add_b[0]=0;     
    for (i=0;i<E;++i) temp_a_add_b[i+1]=b[i];
    TLargeFloat_ArrayAdd(temp_a_add_b,1+E,a,E-acErrorSize);
    temp_c_add_d[0]=0;
    for (i=0;i<E;++i) temp_c_add_d[i+1]=d[i];
    TLargeFloat_ArrayAdd(temp_c_add_d,1+E,c,E-acErrorSize);

    const long abcdPos=(2*MulSize-E-2*(E+1));
    for (i=0;i<abcdPos;++i) result[i]=0;
    ArrayMUL(&result[abcdPos],std::min(rminsize-abcdPos,2*(E+1)),temp_a_add_b,E+1,temp_c_add_d,E+1);
    for (i=abcdPos+2*(E+1);i<rminsize;++i) result[i]=0;

    TInt32bit* temp_ac=&tmpData[0];
    const long acminsize=std::min(rminsize,acSize*2);
    ArrayMUL(temp_ac,acminsize,a,acSize,c,acSize);
    TLargeFloat_ArrayAdd(result,acminsize,temp_ac,acminsize);
    long sub_ac_size=acSize*2;
    if (E+sub_ac_size>rminsize) sub_ac_size=rminsize-E;
    TLargeFloat_ArraySub(result,E+sub_ac_size,temp_ac,sub_ac_size);

    const long add_bdPos=2*MulSize-2*E;
    long sub_bd_size=2*E;
    const long sub_bdPos=add_bdPos-E;
    if (sub_bdPos+sub_bd_size>rminsize) sub_bd_size=rminsize-sub_bdPos;
    if (sub_bd_size>0)
    {
        TInt32bit* temp_bd=&tmpData[0];
        ArrayMUL(temp_bd,sub_bd_size,b,E,d,E);
        long add_bd_size=2*E;
        if (add_bdPos+add_bd_size>rminsize) add_bd_size=rminsize-add_bdPos;
        if (add_bd_size>0)
          TLargeFloat_ArrayAdd(result,add_bdPos+add_bd_size,temp_bd,add_bd_size);
        TLargeFloat_ArraySub(result,sub_bdPos+sub_bd_size,temp_bd,sub_bd_size); 
    }
}


void ArrayMUL_DichotomyPart(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize)
{
    //二分法的辅助函数
    ///////////////////////////////////////////
    //x*y=(a*E+b)*y=a*y*E+b*y;

    if (xsize==ysize)
    {
        ArrayMUL_Dichotomy(result,rminsize,x,y,ysize);
        return ;
    }

    if (xsize<ysize)
    {
        std::swap(xsize,ysize);
        std::swap(x,y);
    }

    const long E=ysize;
    const long asize=xsize-E;
    const TInt32bit* a=&x[0];
    const TInt32bit* b=&x[asize];

 
    ArrayMUL(result,rminsize,a,asize,y,ysize);
    for (long i=asize+ysize;i<rminsize;++i) result[i]=0;
    const long byPos=asize;
    if (byPos<rminsize)
    {
        const long byminsize=rminsize-byPos;
        TLargeFloat::TArray tmpData(byminsize);
        TInt32bit* tmp_by=&tmpData[0];
        ArrayMUL_Dichotomy(tmp_by,byminsize,b,y,E);
        TLargeFloat_ArrayAdd(result,rminsize,tmp_by,byminsize);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////

//VS2017 bug：
/*
在option.cpp中调用了TLargeFloat的构造函数，构造函数间接使用了csTMaxInt_Digits，由于csTMaxInt_Digits的定于本身包含运算，
option.cpp的代码在TLargeFloat.cpp之间执行，使得在使用csTMaxInt_Digits时，程序还未执行const long csTMaxInt_Digits=(long)(log10((long double)TMaxInt_MAX_VALUE)+1);
导致本应该是9的csTMaxInt_Digits等于0
*/
//const long csTMaxInt_Digits=(long)(log10((long double)TMaxInt_MAX_VALUE)+1);
const long csTMaxInt_Digits = 9;

TLargeFloat::TLargeFloat(const SelfType& Value)
:m_Digits(Value.m_Digits)
{
    m_Exponent=Value.m_Exponent;
    m_Sign=Value.m_Sign;
}
    
TLargeFloat::TLargeFloat(const long double DefultValue)
:m_Digits(TDigits(emLongDoubleDigits+1).GetDigitsArraySize(),0)
{
    fToLargeFloat(DefultValue);
}

TLargeFloat::TLargeFloat(const long double DefultValue,const TDigits& DigitsLength)
:m_Digits(DigitsLength.GetDigitsArraySize(),0)
{
    fToLargeFloat(DefultValue);
}

TLargeFloat::TLargeFloat(const char* strValue)
:m_Digits(TDigits(csTMaxInt_Digits).GetDigitsArraySize(),0)
{
    sToLargeFloat(std::string(strValue));
}
TLargeFloat::TLargeFloat(const char* strValue,const TDigits& DigitsLength)
:m_Digits(DigitsLength.GetDigitsArraySize(),0)
{
    sToLargeFloat(std::string(strValue));
}
TLargeFloat::TLargeFloat(const std::string& strValue)
:m_Digits(TDigits(csTMaxInt_Digits).GetDigitsArraySize(),0)
{
    sToLargeFloat(strValue);
}
TLargeFloat::TLargeFloat(const std::string& strValue,const TDigits& DigitsLength)
:m_Digits(DigitsLength.GetDigitsArraySize(),0)
{
    sToLargeFloat(strValue);
}
TLargeFloat::TLargeFloat()
:m_Digits(1,0)
{
	fToLargeFloat(0);
}


void TLargeFloat::Zero()
{
    if (m_Sign==0) return;
    m_Sign=0;
    m_Exponent=0;
    long old_size=m_Digits.size();
    for (long i=0;i<old_size;++i)
        m_Digits[i]=0;
}

void TLargeFloat::Canonicity()
{
    //规格化 小数部分
    //规格化前允许:m_Digits[0]>=emBase 也就是小数部分的值大于等于1.0;
    //规格化前允许:m_Digits[]前面有多个0值 也就是小数部分的值小于0.1;
    //规格化后的数满足的条件：小数部分的值 小于1.0，大于等于0.1;

    if (m_Sign==0)
    {
        return;
    }
    long old_size=m_Digits.size();
    if (old_size<=0) return; //不可能发生:)

    if ( (m_Digits[0]<emBase)&&(m_Digits[0]>=(emBase/10)) ) return;

    //处理右移
	while(m_Digits[0]>=emBase)//是否需要右移1
	{
		m_Exponent+=em10Power;
		//（由张子辰添加）
		//防止右移产生精度损失
		if(m_Digits[old_size-1])
		{
			m_Digits.push_back(0);
			old_size++;
		}
		for(long i=old_size-1;i>=2;--i)
			m_Digits[i]=m_Digits[i-1];
		if(old_size>=2) m_Digits[1]=m_Digits[0]%emBase;
		m_Digits[0]/=emBase;
	}

    ////////////////////////////////////
    //考虑左移

    //大的左移
    long iMoveCountBig=old_size;
    for (long i=0;i<old_size;++i)
    {
        if (m_Digits[i]!=0)
        {
            iMoveCountBig=i;
            break;
        }
    }
    if (iMoveCountBig==old_size)
    {
        //as Zero();
        m_Sign=0;
        m_Exponent=0;
        return;
    }

    if (iMoveCountBig>0)
    {
        m_Exponent-=(iMoveCountBig*em10Power);
        for (long j=0;j<old_size-iMoveCountBig;++j)
            m_Digits[j]=m_Digits[j+iMoveCountBig];
        for (long k=old_size-iMoveCountBig;k<old_size;++k)
            m_Digits[k]=0;
    }

    //小的左移
    TInt32bit iMoveMul=1;
    long iMoveCountSmall=0;
    while (iMoveMul*m_Digits[0]<(emBase/10))
    {
        iMoveMul*=10;
        ++iMoveCountSmall;
    }
    if (iMoveCountSmall>0) 
    {
        m_Exponent-=iMoveCountSmall;
        for (long i=old_size-1;i>=0;--i)
            m_Digits[i]*=iMoveMul;
        for (long j=old_size-1;j>0;--j)
        {
            TInt32bit value=m_Digits[j];
            m_Digits[j]=value%emBase;
            m_Digits[j-1]+=value/emBase;
        }
    }
}

void TLargeFloat::iToLargeFloat(TMaxInt iValue)
{
    Zero();
    long new_size=TDigits(csTMaxInt_Digits).GetDigitsArraySize();
    if ((long)m_Digits.size()<new_size)
        m_Digits.resize(new_size,0);
    if (iValue==0)
        return;
    else if (iValue>0)
        m_Sign=1;
    else
    {
        m_Sign=-1;
        iValue=-iValue;
    }

    //无误差转换
    TMaxInt tmp_iValue=iValue;
    long n=0;
    for (;tmp_iValue!=0;++n)
        tmp_iValue/=emBase;
    m_Exponent=n*em10Power;
    for (long i=0;i<n;++i)
    {
        m_Digits[n-1-i]=(TInt32bit)(iValue%emBase);
        iValue/=emBase;
    }
    Canonicity();
}

void TLargeFloat::fToLargeFloat(long double fValue)
{
    TMaxInt iValue=(TMaxInt)fValue;
    if (iValue==fValue)
    {
        iToLargeFloat(iValue);
        return;
    }

    Zero();
    long new_size=TDigits(emLongDoubleDigits+1).GetDigitsArraySize();
    if ((long)m_Digits.size()<new_size)
        m_Digits.resize(new_size,0);
    if (fValue==0)
        return;
    else if (fValue>0)
        m_Sign=1;
    else
    {
        m_Sign=-1;
        fValue=-fValue;
    }

    m_Exponent=(long)floor(log10(fValue));
    fValue/=pow(10.0,(long)m_Exponent.AsInt());
    long size=m_Digits.size();
    for (long i=0;i<size;++i)//得到小数位
    {
        if (fValue<=0) break;
        fValue*=emBase;
        TInt32bit iValue=(TInt32bit)floor(fValue);
        fValue-=iValue;
        m_Digits[i]=iValue;
    }      
    
    Canonicity();
}

void TLargeFloat::Swap(SelfType& Value)
{
    if (this==&Value) return;
    m_Digits.swap(Value.m_Digits);
    std::swap(m_Sign,Value.m_Sign);
    std::swap(m_Exponent,Value.m_Exponent);
}

void TLargeFloat::Chs()
{
    m_Sign*=(-1);
}

void TLargeFloat::Abs()
{
    if (m_Sign!=0) m_Sign=1;
}

unsigned long TLargeFloat::GetDigitsLength() const
{
    return m_Digits.size()*em10Power;
}

void TLargeFloat::SetDigitsLength(const TDigits& DigitsLength)
{
    m_Digits.resize(DigitsLength.GetDigitsArraySize(),0);
}

TLargeFloat::SelfType& TLargeFloat::operator =  (const SelfType& Value)
{
    if (this==&Value) return *this;
    m_Digits=Value.m_Digits;
    m_Sign=Value.m_Sign;
    m_Exponent=Value.m_Exponent;
    return *this;
}

TLargeFloat::SelfType& TLargeFloat::operator =  (long double  fValue)
{
    fToLargeFloat(fValue);
    return *this;
}

long double TLargeFloat::AsFloat() const
{
    if ( ((m_Exponent.AsInt())>=emLongDoubleMaxExponent)
        ||((m_Exponent.AsInt())<=emLongDoubleMinExponent) )
    {
        throw TException("ERROR:TLargeFloat::AsFloat()");
    }

    if (m_Sign==0) return 0;
    long double result=m_Sign*pow((long double)10.0,(long double)m_Exponent.AsInt());

    long double r=1;
    long double Sum=0;
    long old_size=m_Digits.size();
    for (long i=0;i<old_size;++i)//得到小数位
    {
        r/=emBase;//r*=(1.0/emBase);
        if (r==0) break; 
        Sum+=(m_Digits[i]*r);
    }       
    return result*Sum;
}
std::string  TLargeFloat::AsString2() const
{
    if (m_Sign==0)
        return "0";

    //计算需要的字符串空间大小
    long str_length=2+m_Digits.size()*em10Power;
    if  (m_Sign<0) ++str_length; //负号

    TMaxInt EpValue=m_Exponent.AsInt();
    long UEP_StrLength=0;
    if (EpValue!=0) 
    {
        ++str_length;
        if (EpValue<0)
        {
            EpValue=-EpValue;
            ++str_length;
        }
        UEP_StrLength=0;
        while (EpValue>0)
        {
            EpValue/=10;
            ++UEP_StrLength;
        }
        str_length+=UEP_StrLength;
    }

    std::string result(str_length,' ');
    std::string::value_type* pStr=&result[0];

    //输出字符串
    if  (m_Sign<0) { *pStr='-'; ++pStr; }
    *pStr='0'; ++pStr;   *pStr='.'; ++pStr;

    long dgsize=m_Digits.size();
    for (long i=0;i<dgsize;++i)
    {
        TInt32bit Value=m_Digits[i];
        for (long d=0;d<em10Power;++d)
        {
            pStr[em10Power-1-d]=(char)('0'+(Value%10));
            Value/=10;
        }
        pStr+=em10Power;
    }

    EpValue=m_Exponent.AsInt();
    if (EpValue!=0) 
    {
        *pStr='e'; ++pStr;
        if (EpValue<0)
        {
            EpValue=-EpValue;
            *pStr='-'; ++pStr;
        }
        for (long i=0;i<UEP_StrLength;++i)
        {
            pStr[UEP_StrLength-1-i]=(char)('0'+(EpValue%10));
            EpValue/=10;
        }
        pStr+=UEP_StrLength;
    }

    return result;
}

    template<class PChar>
    PChar sToLargeFloat_GetCharIntEnd(PChar int_begin,PChar str_end)
    {
        long int_count=0;
        for (PChar i=int_begin;i<str_end;++i)
        {
            if ( ('0'<=(*i)) && ((*i)<='9') )
                ++int_count;
            else
                break;
        }
        return &int_begin[int_count];
    }

    inline void sToLargeFloat_setAChar(TLargeFloat::TArray& Digits,long set_index,char aChar)
    {
        unsigned long Value=aChar-'0';
        long e10Power_count=TLargeFloat::em10Power-1-(set_index%TLargeFloat::em10Power);
        for (long i=0;i<e10Power_count;++i)
          Value*=10;
        Digits[set_index/TLargeFloat::em10Power]+=Value;
    }

void TLargeFloat::sToLargeFloat(const std::string& strValue)
{
    if (strValue.size()<=0)
    {
        this->Zero();
        return;
    }
    typedef const std::string::value_type* PChar;
    PChar pstr_begin=strValue.c_str();
    PChar pstr_end=&pstr_begin[strValue.size()];

    //str as:  [+|-][0..9][.][0..9][E|e][+|-][0..9] 
    bool is_have_sign=false;
    long  sign=1;
    bool is_have_int=false;
    PChar int_begin=0; PChar int_end=0; 
    bool is_have_dot=false;
    bool is_have_digits=false;
    PChar digits_begin=0; PChar digits_end=0; 
    bool is_have_ep=false;
    bool is_have_ep_sign=false;
    long  ep_sign=1;
    bool is_have_ep_int=false;
    PChar ep_int_begin=0; PChar ep_int_end=0; 

    //处理正负号
    if ((*pstr_begin)=='-') 
    {
        sign = -1;
        is_have_sign=true;
        ++pstr_begin; 
    }
    else if ((*pstr_begin)=='+') 
    {
        is_have_sign=true;
        ++pstr_begin; 
    }

    //处理前面的整数部分
    int_begin=pstr_begin;
    int_end=sToLargeFloat_GetCharIntEnd(int_begin,pstr_end);
    is_have_int=(int_begin!=int_end);
    if ((int_end-int_begin>=2)&&((*int_begin)=='0')) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 0"); //'0?'数字表示错误
    pstr_begin=int_end;
    if (((*pstr_begin)!='.')&&(!is_have_int)) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 1"); //没有任何数字
    
    //处理小数部分
    if ((*pstr_begin)=='.')
    {
        is_have_dot=true;
        ++pstr_begin;

        digits_begin=pstr_begin;
        digits_end=sToLargeFloat_GetCharIntEnd(digits_begin,pstr_end);
        is_have_digits=(digits_begin!=digits_end);
        if ((!is_have_digits)&&(!is_have_int)) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 2"); //没有任何数字
        pstr_begin=digits_end;
    }

    //处理指数部分
    if ( ((*pstr_begin)=='e')||((*pstr_begin)=='E') )
    {
        is_have_ep=true;
        ++pstr_begin;
        //处理指数的正负号
        if ((*pstr_begin)=='-') 
        {
            ep_sign = -1;
            is_have_ep_sign=true;
            ++pstr_begin; 
        }
        else if((*pstr_begin)=='+') 
        {
            is_have_ep_sign=true;
            ++pstr_begin; 
        }
        //处理指数中的整数
        ep_int_begin=pstr_begin;
        ep_int_end=sToLargeFloat_GetCharIntEnd(ep_int_begin,pstr_end);
        is_have_ep_int=(ep_int_begin!=ep_int_end);
        if ((ep_int_end-ep_int_begin>=2)&&((*ep_int_begin)=='0')) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 3"); //'0?'数字表示错误
        pstr_begin=ep_int_end;
        if (!is_have_ep_int) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 4"); //指数没有数字
    }
    if (pstr_begin!=pstr_end) throw TException("ERROR:TLargeFloat::sToLargeFloat(); 5"); //未预料的字符


    //////////////////
    //实际的类型转换

    this->Zero();
    this->m_Sign=sign;
    this->m_Exponent=(int_end-int_begin);
    long need_digits_count=(int_end-int_begin)+(digits_end-digits_begin);
    if ((long)this->GetDigitsLength()<need_digits_count)
      this->SetDigitsLength(need_digits_count);
    long set_index=0;
    PChar set_begin=int_begin;
    for (;set_begin<int_end;++set_begin,++set_index)
        sToLargeFloat_setAChar(this->m_Digits,set_index,*set_begin);
    set_begin=digits_begin;
    for (;set_begin<digits_end;++set_begin,++set_index)
        sToLargeFloat_setAChar(this->m_Digits,set_index,*set_begin);

    if (is_have_ep)
    {
        TExpInt ep=0;
        for (PChar set_begin=ep_int_begin;set_begin<ep_int_end;++set_begin)
        {
            ep*=10;
            ep+=((*set_begin)-'0');
        }
        ep*=ep_sign;
        this->m_Exponent+=ep;
    }
    Canonicity();
}


long  TLargeFloat::Compare(const SelfType& Value) const
{
    if (this==&Value) return 0;
    //(*this)>Value 返回1，小于返回-1，相等返回0

    //比较符号
    if (m_Sign>Value.m_Sign)
        return 1;
    else if(m_Sign<Value.m_Sign)
        return -1;
    else //m_Sign==Value.m_Sign
    {
        if(m_Sign==0)
            return 0;
    }
    //m_Sign==Value.m_Sign

    //比较指数
    if (m_Exponent.AsInt()>Value.m_Exponent.AsInt())
        return m_Sign;
    else if (m_Exponent.AsInt()<Value.m_Exponent.AsInt())
        return -m_Sign;
    else//(m_Exponent==Value.m_Exponent)
    {
        long sizeS=m_Digits.size();
        long sizeV=Value.m_Digits.size();
        long min_size=std::min(sizeS,sizeV);
        for (long i=0;i<min_size;++i)
        {
            if (m_Digits[i]!=Value.m_Digits[i])
            {
                if (m_Digits[i]>Value.m_Digits[i])
                    return m_Sign;
                else //if (m_Digits[i]<Value.m_Digits[i])
                    return -m_Sign;
            }
        }

        //继续比较 处理尾部
        if (sizeS>sizeV)
        {
            for (long i=sizeV;i<sizeS;++i) 
            {
                if (m_Digits[i]>0)
                    return m_Sign;;
            }
        }
        else if (sizeS<sizeV)
        {
            for (long i=sizeS;i<sizeV;++i) 
            {
                if (Value.m_Digits[i]>0)
                    return -m_Sign;
            }
        }
        //sizeS==sizeV
        
        return 0;
    }
}

//////////////////////////////
//+ -

TLargeFloat::SelfType& TLargeFloat::operator += (const SelfType& Value)
{
    if (Value.m_Sign==0)
        return (*this);
    else if (m_Sign==0)
        return (*this)=Value;
    else if (m_Sign==Value.m_Sign)
    {
        TInt32bit oldSign=m_Sign;
        Abs_Add(Value);
        m_Sign=oldSign;
        return *this;
    }
    else
        return *this-=(-Value);
}

TLargeFloat::SelfType& TLargeFloat::operator -= (const SelfType& Value)
{
    if (Value.m_Sign==0)
        return (*this);
    else if (m_Sign==0)
    {
        (*this)=-Value;
        return (*this);
    }
    else if (m_Sign==Value.m_Sign)
    {
        long comResult=this->Compare(Value);    
        if (comResult==0)
        {
            this->Zero();
        }
        else
        {
            Abs_Sub_Abs(Value);
            m_Sign=comResult;
        } 
        return *this;
    }
    else
        return *this+=(-Value);
}

void TLargeFloat::Abs_Add(const SelfType& Value)//绝对值加法
{
    this->Abs();
    SelfType Right(Value);
    Right.Abs();

    //精度一致
    if (m_Digits.size()<Right.m_Digits.size())
        m_Digits.resize(Right.m_Digits.size(),0);
    else if (m_Digits.size()>Right.m_Digits.size())
        Right.m_Digits.resize(m_Digits.size(),0);
    //被加数指数较大就可以了
    if (m_Exponent.AsInt()<Right.m_Exponent.AsInt())
        this->Swap(Right);
	//（由张子辰添加）
	//防止大数字“吃掉”小数字
	if((int)m_Digits.size()*4<=m_Exponent.AsInt()-Right.m_Exponent.AsInt())
	{
		const long t=std::min<long>((m_Exponent.AsInt()-Right.m_Exponent.AsInt())/4+1,(DEFAULT_DigitsArraySize2+3)/4);
		m_Digits.resize(t,0);
		Right.m_Digits.resize(t,0);
	}
    long size=m_Digits.size();

    TMaxInt iMoveRightCount=m_Exponent.AsInt()-Right.m_Exponent.AsInt();
    if (iMoveRightCount>=size*(TMaxInt)TLargeFloat::em10Power) return;
    TLargeFloat_CtrlExponent(Right.m_Digits,iMoveRightCount);//对齐小数点

    TLargeFloat_ArrayAdd(&m_Digits[0],size,&Right.m_Digits[0],size);
    if (m_Digits[0]>=emBase)
        Canonicity();
}

void TLargeFloat::Abs_Sub_Abs(const SelfType& Value)//绝对值减法
{
    this->Abs();
    SelfType Right(Value);
    Right.Abs();

    long comResult=this->Compare(Right);
    if (comResult==0)
    {
        this->Zero();
        return;
    }
    else if (comResult<0)
        this->Swap(Right);

    //精度一致
    if (m_Digits.size()<Right.m_Digits.size())
        m_Digits.resize(Right.m_Digits.size(),0);
    else if (m_Digits.size()>Right.m_Digits.size())
        Right.m_Digits.resize(m_Digits.size(),0);
	//（由张子辰添加）
	//防止大数字“吃掉”小数字
	if((int)m_Digits.size()*4<=m_Exponent.AsInt()-Right.m_Exponent.AsInt())
	{
		const long t=std::min<long>((m_Exponent.AsInt()-Right.m_Exponent.AsInt())/4+1,(DEFAULT_DigitsArraySize2+3)/4);
		m_Digits.resize(t,0);
		Right.m_Digits.resize(t,0);
	}
    long size=m_Digits.size();

    TMaxInt iMoveRightCount=m_Exponent.AsInt()-Right.m_Exponent.AsInt();
    if (iMoveRightCount>=size*(TMaxInt)TLargeFloat::em10Power) return;
    TLargeFloat_CtrlExponent(Right.m_Digits,iMoveRightCount); //对齐小数点

    TLargeFloat_ArraySub(&m_Digits[0],size,&Right.m_Digits[0],size);
    if (m_Digits[0]*10<emBase)
        Canonicity();
}



/////////////////////////////////
// /

    //最大的32bit整数除数M满足: (emBase-1)+(M-1)*emBase<=TInt32bit_MAX_VALUE
    const TInt32bit csMaxDiviValue=TInt32bit_MAX_VALUE/TLargeFloat::emBase -1;
    //最大的整数除数M满足: (emBase-1)+(M-1)*emBase<=TMaxInt_MAX_VALUE
    const TMaxInt csMaxDiviValueBig=TMaxInt_MAX_VALUE/TLargeFloat::emBase -1;

void TLargeFloat::DivInt(TMaxInt iValue)//除以一个整数
{
    if (iValue==0) 
        throw TException("ERROR:TLargeFloat::DivInt()");
    else if (iValue<0)
    {
        iValue=-iValue;
        this->Chs();
        //continue
    }
    if (iValue==1)  return;
    if (this->m_Sign==0) return;

    if (iValue>csMaxDiviValueBig) 
    {
        TLargeFloat y(0.0);
        y.iToLargeFloat(iValue);
        (*this)/=y;
        return;
    }
    
    if (iValue<=csMaxDiviValue)
    {
        TInt32bit iDiv=(TInt32bit)iValue;
        long size=m_Digits.size();
        for (long i=0;i<size-1;++i)
        {
            m_Digits[i+1]+=(m_Digits[i]%iDiv)*emBase;
            m_Digits[i]/=iDiv;
        }
        if (size>=1)
            m_Digits[size-1]/=iDiv;
    }
    else 
    {
        TMaxInt iDiv=iValue;
        long size=m_Digits.size();
        TMaxInt cur_value=m_Digits[0];
        for (long i=0;i<size-1;++i)
        {
            m_Digits[i]=(TInt32bit)(cur_value/iDiv);
            cur_value=m_Digits[i+1]+(cur_value%iDiv)*emBase;
        }
        if (size>=1)
            m_Digits[size-1]=(TInt32bit)(cur_value/iDiv);
    }

    Canonicity();
}

TLargeFloat::SelfType& TLargeFloat::operator /= (long double  fValue)
{
    TMaxInt iValue=(TMaxInt)fValue;
    if (iValue==fValue)
    {
        DivInt(iValue);
        return *this;
    }
    else
        return (*this)/=TLargeFloat(fValue);
}

TLargeFloat::SelfType& TLargeFloat::operator /= (const SelfType& Value)
{
    SelfType x(Value);
	x.SetDigitsLength(DEFAULT_DigitsArraySize2);	
    x.Rev();//x=1/x;
	(*this)*=x;
	//（由张子辰添加）
	//删去无用的0，节约内存
	for(long i=m_Digits.size()-1;i>=0;i--)
	{
		if(m_Digits[i])
		{
			m_Digits.resize(i+1);
			break;
		}
	}
    return (*this);
}

void TLargeFloat::Rev()//求倒数
{
    //求1/a,
    // 则有a=1/x;  y=a-1/x;
    // 求导得y'=1/x^2;
    // 代入牛顿方程 x(n+1)=x(n)-y(x(n))/y'(x(n));
    // 有迭代式 x_next=(2-a*x)*x; //可证明：该公式为2阶收敛公式
    // 证明:令x=1/a+dx;
    // 则有x_next-1/a=(2-a*(1/a+dx))*(1/a+dx)-1/a
    //               =(-a)*dx^2
    // 证毕.

    if (this->m_Sign==0) throw TException("ERROR:TLargeFloat::Rev()");
    TExpInt oldExponent=m_Exponent;

    SelfType a(*this);
    a.m_Exponent=0;
    SelfType x(0.0,TLargeFloat::TDigits(a.GetDigitsLength()));
    x=1.0/a.AsFloat();//初始迭代值

    TDigitsLengthAutoCtrl dlCtrl(2,emLongDoubleDigits-1,(a.m_Digits.size()*TLargeFloat::em10Power));
    SelfType ta(0.0,TLargeFloat::TDigits(a.GetDigitsLength()));
    SelfType temp(0.0,TLargeFloat::TDigits(a.GetDigitsLength()));
    dlCtrl.add_to_ctrl(&ta);
    dlCtrl.add_to_ctrl(&x);
    dlCtrl.add_to_ctrl(&temp);
    for (long i=0;i<dlCtrl.get_runcount();++i)
    {
        ta=a;
        dlCtrl.SetNextDigits();
        //x=x*2-x*x*ta;
        temp=x;
        temp*=temp;
        temp*=ta;
        x*=2;
        x-=temp;
    }

    x.m_Exponent-=oldExponent;
    this->Swap(x);
}


/////////////////////////////////////////////////////
//

void TLargeFloat::RevSqrt()//
{
    //求1/a^0.5,
    // 则有a=1/x^2;  y=a-1/x^2;
    // 求导得y'=2/x^3;
    // 代入牛顿方程 x(n+1)=x(n)-y(x(n))/y'(x(n));
    // 有迭代式 x_next=(3-a*x*x)*x/2; //可证明：该公式为2阶收敛公式
    // 证明:令x=1/a^0.5+dx;
    // 则有x_next-1/a^0.5=(3-a*(1/a^0.5+dx)*(1/a^0.5+dx))*(1/a^0.5+dx)/2 - 1/a^0.5
    //                   =-(1.5/a^0.5)*dx^2-0.5*a*dx^3;
    // 证毕.
    if (this->m_Sign<0) throw TException("ERROR:TLargeFloat::RevSqrt()");
    if (this->m_Sign==0) return;

    SelfType a(*this);
    TMaxInt sqrExponent=a.m_Exponent.AsInt()/2;
    a.m_Exponent-=(sqrExponent*2);


    SelfType x(0.0,TDigits(a.GetDigitsLength()));
    x=1.0/sqrt(a.AsFloat());//初始迭代值

    TDigitsLengthAutoCtrl dlCtrl(2,emLongDoubleDigits-1,(a.m_Digits.size()*TLargeFloat::em10Power));
    SelfType ta(0.0,TLargeFloat::TDigits(a.GetDigitsLength()));
    SelfType temp(0.0,TLargeFloat::TDigits(a.GetDigitsLength()));
    dlCtrl.add_to_ctrl(&ta);
    dlCtrl.add_to_ctrl(&x);
    dlCtrl.add_to_ctrl(&temp);
    for (long i=0;i<dlCtrl.get_runcount();++i)
    {
        ta=a;
        dlCtrl.SetNextDigits();
        //x=(3-x*x*ta)*x/2;
        temp=x;
        temp*=temp;
        temp*=ta;
        temp.Chs();
        temp+=3;
        x*=temp;
        x/=2;
    }

    x.m_Exponent-=sqrExponent;
    this->Swap(x);
}

////////////////////////////
//*

void TLargeFloat::MulInt(TMaxInt iValue)
{
    if (this->m_Sign==0) return;
    if (iValue==0) 
    {
        this->Zero();
        return;
    }
    else if (iValue<0)
    {
        iValue=-iValue;
        this->Chs();
        //continue
    }

    if (iValue>csMaxMuliValue)
    {
        TLargeFloat y(0.0);
        y.iToLargeFloat(iValue);
        (*this)*=y;
        return;
    }

    //乘以一个整数
    TInt32bit Value=(TInt32bit)iValue;
    long size=m_Digits.size();
    for (long i=size-1;i>=0;--i)
    {
        m_Digits[i]*=Value;
    }
    for (long j=size-1;j>=1;--j)
    {
        if (m_Digits[j]>=emBase)
        {
            m_Digits[j-1]+=m_Digits[j]/emBase;
            m_Digits[j]%=emBase;
        }
    }
    Canonicity();
}
TLargeFloat::SelfType& TLargeFloat::operator *= (long double  fValue)
{
    TMaxInt iValue=(TMaxInt)fValue;
    if (iValue==fValue)
    {
        MulInt(iValue);
        return *this;
    }
    else
    {
        TLargeFloat temp(fValue);
        return (*this)*=temp;
    }
}

inline void getBestMulSize(const TInt32bit*& x,long& xsize,const TInt32bit*& y,long& ysize)
{
    long bxsize=xsize;
    long bysize=ysize;

    long i;
    for (i=xsize-1;i>=0;--i)
    {
        if (x[i]==0) 
            --bxsize;
        else
            break;
    }
    for (i=ysize-1;i>=0;--i)
    {
        if (y[i]==0) 
            --bysize;
        else
            break;
    }

    if (bxsize<bysize)
    {
        std::swap(bxsize,bysize);
        std::swap(xsize,ysize);
        std::swap(x,y);
    }
    xsize=bxsize;

    const long csABigMulSize=1000;
    if ( (bysize<csABigMulSize)||(bxsize*2>=bysize*3)||(bxsize>ysize) )
        ysize=bysize;
    else 
        ysize=bxsize;
}

TLargeFloat::SelfType& TLargeFloat::operator *= (const SelfType& Value)
{
    long xsize=m_Digits.size();
    long ysize=Value.m_Digits.size();
    if ((m_Sign==0)||(Value.m_Sign==0)) 
    {
        if (m_Sign!=0)
            this->Zero();
        if (xsize<ysize)
            m_Digits.resize(ysize,0);
        return *this;
    }
    if (xsize<ysize)
        m_Digits.resize(ysize,0);
    
    long rminsize=
		long((DEFAULT_DigitsArraySize2+TLargeFloat::em10Power-1)
			/TLargeFloat::em10Power);//乘法结果需要的位数

    const TInt32bit* x=&m_Digits[0];
    const TInt32bit* y=&Value.m_Digits[0];
    getBestMulSize(x,xsize,y,ysize);
    if (rminsize>xsize+ysize)
        rminsize=xsize+ysize;

    TArray tempArray(rminsize);
    ArrayMUL(&tempArray[0],rminsize,x,xsize,y,ysize);

    xsize=m_Digits.size();
    //long resultsize=std::min(xsize,rminsize);
	long resultsize=rminsize;
    long i;
	if(xsize<resultsize)m_Digits.resize(resultsize);
    for (i=0;i<resultsize;++i)  this->m_Digits[i]=tempArray[i];
    for (i=resultsize;i<xsize;++i) this->m_Digits[i]=0;
    m_Exponent+=Value.m_Exponent;
    m_Sign*=Value.m_Sign;
    Canonicity();
    return *this;
}


////////////////////////////////////////////////////////////
//快速傅立叶变换实现大整数乘法 
//  FFT Mul

struct TComplex //复数类型    
{
    double  i;
    double  r;
};

//初始化

void FFTInitial(TComplex* w,long* lst,const long n)
{
    //计算W^i , i in [0--n)
    const double PI=3.1415926535897932385;
    const double seta=2.0*PI/n;
    long i,j;

    long n2=n>>2;
    for (i=0;i<=n2;++i)
    {
        w[i].i=sin(seta*i);
        w[i].r=cos(seta*i);
    }
    for (j=(n2+1),i=(n2-1); i>=0 ;--i,++j)
    {
        w[j].i= w[i].i;
        w[j].r=-w[i].r;
    }

    //计算排序用数组
    lst[0]=0; 
    i=1;
    long m;
    for (n2=n>>1,m=2; n2>0 ;n2=n2>>1,m =m<<1) 
    {
        for (long j=0;i<m;++i,++j)
        {
            lst[i]=lst[j]+n2;
        }
    }
}

//快速傅立叶变换
void FFT(TComplex* a,const TComplex* w,const long n)
{
    long n2=1;  
    long n1=n >> 1;
    for (long per=1;per<n;)
    {
        long m=n2;  long j=0;  long k=n2;
        per<<=1;
        while (k<n) 
        {
            for (long i2=0; j<m ;++j,++k,i2+=n1)
            {
                TComplex tmpb=a[j];
                double   tmp1=a[k].r*w[i2].r-a[k].i*w[i2].i;
                double   tmp2=a[k].r*w[i2].i+a[k].i*w[i2].r;
                a[j].r=tmpb.r+tmp1;
                a[j].i=tmpb.i+tmp2;
                a[k].r=tmpb.r-tmp1;
                a[k].i=tmpb.i-tmp2;
            }
            m+=per;  
            j+=n2;  
            k+=n2;
        }
        n2=per;
        n1>>=1;
    }
}


//反向快速傅立叶变换
void InvFFT(TComplex* a,const TComplex* w,const long n)
{
    for (long i=0;i<n;++i)
        a[i].i=-a[i].i;
    FFT(a,w,n);
}

void FFT_Convolution(const TComplex* a,TComplex* b,const long n)
{
    for (long i=0;i<n;++i)
    {
        double br=a[i].r*b[i].r - a[i].i*b[i].i;
        b[i].i=a[i].r*b[i].i + a[i].i*b[i].r;
        b[i].r=br;
    }
}

    void FFT_IntArrayCopyToComplexArray(const TInt32bit* ACoef,const long ASize,TComplex* result,const long n,const long* lst)
    {
        for (long i=0;i<n;++i)
        {
            long ai=lst[i];
            if (ai<ASize)
                result[i].r=ACoef[ai];  
            else
                result[i].r=0.0;  
            result[i].i=0.0;
        }
    }

    void FFT_ComplexArrayToIntArray(const TComplex* cmx,const long n,TInt32bit* result,const long rMinSize)
    {
        const double tmpRevN=1.0/n;
        double FFT_int_base;
        TLargeFloat::TArray tempResult;
        TInt32bit* dst=0;
        long dstsize=0;
        {   
            FFT_int_base=(long)TLargeFloat::emBase;
            dstsize=rMinSize;
            dst=result;
        }
        const double tmpRevFFT_int_base=1.0/FFT_int_base;

        double MaxWorstEr=0;
        double tmp=0.0;
        for (long i=std::min(n,dstsize)-1;i>=1;--i)
        {
            double num =cmx[i-1].r*tmpRevN+tmp;
            tmp=floor((num+0.499)*tmpRevFFT_int_base);
            dst[i]=(TInt32bit)(num+0.499-tmp*FFT_int_base);

            double newWorstEr=abs(num-tmp*FFT_int_base-dst[i]);//记录最大误差
           if (newWorstEr>MaxWorstEr)
                MaxWorstEr=newWorstEr;  
        }
        if (dstsize>0)
            dst[0]=(TInt32bit)(tmp+0.1);
        if (MaxWorstEr>=0.499) 
            throw TLargeFloat::TException("ERROR:FFT's FFT_ComplexArrayToIntArray(); "); //误差过大
    }


//FFT平方
void SquareFFT(const TInt32bit* ACoef,const long ASize,TInt32bit* CCoef,const long CMinSize)
{
    long ansize=ASize+ASize;
    long n=1;
    while (n<ansize) n<<=1;

    std::vector<TComplex>   _w((n>>1)+1);   
    std::vector<long>       _lst(n);        
    TComplex* w=&_w[0];
    long*     lst=&_lst[0];
    FFTInitial(w,lst,n);

    std::vector<TComplex>   _a(n);  
    TComplex* a=&_a[0];
    FFT_IntArrayCopyToComplexArray(ACoef,ASize,a,n,lst);

    FFT(a,w, n);
    FFT_Convolution(a,a,n);

    std::vector<TComplex>   _tmpa(n);
    _tmpa.swap(_a);
    TComplex* tmpa=&_tmpa[0]; a=&_a[0];
    for (long i=0;i<n;++i)  a[i]=tmpa[lst[i]];  
    _tmpa.clear();
    _lst.clear();
    InvFFT(a,w,n);
    _w.clear();
    FFT_ComplexArrayToIntArray(a,n,CCoef,CMinSize);
 }


//FFT乘法
void MulFFT(const TInt32bit* ACoef,const long ASize,const TInt32bit* BCoef,const long BSize,TInt32bit* CCoef,const long CMinSize)
{
    long absize=(ASize+BSize);
    long n=1;
    while (n<absize) n<<=1;

    std::vector<TComplex>   _w((n>>1)+1);
    std::vector<long>       _lst(n);
    TComplex* w=&_w[0];
    long*     lst=&_lst[0];
    FFTInitial(w,lst,n);

    std::vector<TComplex>   _a(n);
    TComplex* a=&_a[0];
    FFT_IntArrayCopyToComplexArray(ACoef,ASize,a,n,lst);
    std::vector<TComplex>   _b(n);
    TComplex* b=&_b[0];
    FFT_IntArrayCopyToComplexArray(BCoef,BSize,b,n,lst);

    FFT(b,w, n);
    FFT(a,w, n);
    FFT_Convolution(a,b,n);
    for (long i=0;i<n;++i)  a[i]=b[lst[i]];  
    _b.clear();
    _lst.clear();
    InvFFT(a,w,n);
    _w.clear();
    FFT_ComplexArrayToIntArray(a,n,CCoef,CMinSize);
}


/////////////////////////////////////////////////////////////////////////////////////////////

const long csBestMUL_ExE_size           =1024       /TLargeFloat::em10Power;
const long csBestMUL_Dichotomy_size     =4096       /TLargeFloat::em10Power;
const long csBestMul_FFT4Max_size       =16777216   /TLargeFloat::em10Power;
void ArrayMUL(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize)
{
    if (xsize<ysize)
    {
        std::swap(xsize,ysize);
        std::swap(x,y);
    }
    if (rminsize<ysize)
    {
        xsize=rminsize;
        ysize=rminsize;
    }
    if (ysize<=csBestMUL_ExE_size)  //
        TLargeFloat_ArrayMUL_ExE(result,rminsize,x,xsize,y,ysize);
    else  if (ysize<=csBestMUL_Dichotomy_size)
        ArrayMUL_DichotomyPart(result,rminsize,x,xsize,y,ysize);
    else if ((xsize+ysize)<=(csBestMul_FFT4Max_size*2))
    {
        if (x==y)
            SquareFFT(x,xsize,result,rminsize);
        else
            MulFFT(x,xsize,y,ysize,result,rminsize);
    }
    else
        ArrayMUL_DichotomyPart(result,rminsize,x,xsize,y,ysize);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下代码由张子辰添加
void CutZero(std::string &result)
{
	int len=result.length();
	bool isInt=true;
	for(int i=0;i<len;i++)
	{
		if(result[i]=='.')
		{
			isInt=false;
			break;
		}
	}
	int i=0;
	if(result[0]=='-')i=1;
	while(result[i]=='0')i++;
	int j=result.length()-1;
	if(!isInt)
	{
		while(result[j]=='0')j--;
		if(result[j]=='.')j--;//删除整数后的小数点 
	}
	result=result.substr(i,j-i+1);
	if(result.length()==0)result="0";
	else if(result[0]=='.')result="0"+result;
}

//去除多余的精度
void CutDigits(std::string &result)
{
	int len=result.length();
	bool nodot=true;
	for(auto i:result)
	{
		if(i=='.')
		{
			nodot=false;
			break;
		}
	}
	int len_0=len;//开始时数字的长度
	bool begin_zero=true;
	unsigned int cnt=0;
	for(int i=0;i<len;i++)
	{
		if(result[i]=='0'&&begin_zero)continue;
		if(result[i]>='0'&&result[i]<='9')cnt++,begin_zero=false;
		if(cnt>DEFAULT_DigitsArraySize&&result[i]>='0'&&result[i]<='9')
		{
			//四舍
			if(result[i]<'5')result=result.substr(0,i);
			//五入
			else
			{
				result=result.substr(0,i);
				//进位
				for(int i=result.length()-1;i>=0;i--)
				{
					if(result[i]>='0'&&result[i]<='9')
					{
						if(result[i]!='9')
						{
							result[i]++;
							goto end;
						}
						else
						{
							result[i]='0';
						}
					}
				}
				//进位一直到最高位
				result="1"+result;
				len_0++;
			end:;
			}
			break;
		}
	}
	if(nodot)result.resize(len_0,'0');
}

std::string TLargeFloat::AsString() const
{
	std::string result=AsString2();
	//	return result;
	bool isnegtive=false;
	if(result[0]=='-')
	{
		isnegtive=true;
		result=result.substr(1,result.length()-1);
	}
	unsigned int len=result.length();
	int pE=0;
	for(int i=len-1;i>=0;i--)
	{
		if(result[i]=='e')
		{
			pE=i;
			break;
		}
	}
	if(!pE)
	{
		CutDigits(result);
		CutZero(result);
		if(isnegtive)return '-'+result;
		else return result;
	}
	unsigned long EpValue=0;
	//	std::cout<<"{"<<result<<"}"<<std::endl;
	if(result[pE+1]=='-')
	{
		for(unsigned int i=pE+2;i<len;i++)
		{
			EpValue=EpValue*10+result[i]-'0';
		}
		result=result.substr(0,pE);
		std::string tmp;
		tmp.resize(EpValue,'0');
		result[1]='0';
		result=tmp+result;
		result[1]='.';
	}
	else
	{
		for(unsigned int i=pE+1;i<len;i++)
		{
			EpValue=EpValue*10+result[i]-'0';
		}
		result=result.substr(0,pE);
		len=result.length();
		int i=0;
		while(result[i]!='.')i++;
		if(len-i<=EpValue)result.resize(EpValue+i+1,'0');
		for(unsigned int j=0;j<EpValue;j++)
		{
			std::swap(result[i],result[i+1]);
			i++;
		}
	}
	//	std::cout<<"{"<<result<<"}"<<std::endl;
	CutZero(result);
	CutDigits(result);
	CutZero(result);
	if(isnegtive&&result!="0")return '-'+result;
	else return result;
}

//向下取整
TLargeFloat floor(const TLargeFloat &x)
{
	TLargeFloat Val(x);
	if (Val.m_Sign==-1)
	{
		Val.m_Sign=1;
		return -floor(Val);
	}
	TMaxInt Ep=Val.m_Exponent.AsInt();
	if(Ep<=0)return TLargeFloat("0");
	if((int)Val.m_Digits.size()>(Ep+3)/4)
	{
		Val.m_Digits.resize((Ep+3)/4);
	}
	switch(Ep%4)
	{
	case 1:((*(--Val.m_Digits.end()))/=1000)*=1000;break;
	case 2:((*(--Val.m_Digits.end()))/=100)*=100;break;
	case 3:((*(--Val.m_Digits.end()))/=10)*=10;break;
	}
	return Val;
}

bool TLargeFloat::isInt()const
{
	if(m_Sign==0)return true;
	TMaxInt Ep=m_Exponent.AsInt();
	if(Ep<=0)return false;
	if((int)m_Digits.size()>(Ep+3)/4)
	{
		for(unsigned int i=(Ep+3)/4;i<m_Digits.size();i++)
		{
			if(m_Digits[i])return false;
		}
	}
	else if((int)m_Digits.size()<(Ep+3)/4)
		return true;
	switch(Ep%4)
	{
	case 1:return (m_Digits[(Ep+3)/4-1])%1000==0;
	case 2:return (m_Digits[(Ep+3)/4-1])%100==0;
	case 3:return (m_Digits[(Ep+3)/4-1])%10==0;
	default:return true;
	}
}

TLargeFloat::operator int()
{
	long double tmp=AsFloat();
	if(tmp>0x7fffffff)throw TException("TLargeFloat::operator int()");
	return int(tmp);
}

TLargeFloat fastpow(const TLargeFloat &x,long long y);//基于快速幂的乘方
long long fastpowi(const long long &x,long long y);

TLargeFloat exp(const TLargeFloat & x)
{
	if(x.m_Sign==-1)
		return TLargeFloat(1)/exp(-x);
	if(x.m_Sign==0)
	{
		return 1;
	}
	TMaxInt Exp=x.m_Exponent.AsInt();
	if(Exp>2)
	{
//		system("pause");
		TLargeFloat y(x);
		y.m_Exponent-=Exp-2;
		return fastpow(exp(y),fastpowi(10,Exp-2));
	}
	TLargeFloat result("1",TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat tmp("1",TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat epsilon("1e-"+std::to_string(DEFAULT_Epsilon+2),
		TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat result2;
	for(TLargeFloat i=1;tmp>epsilon;i+=1)
	{
		tmp*=x;
		tmp/=i;
		result2=result+tmp;
		if(result2==result)break;
		result=result2;
	}
	return result;
}

//x=-ln(a)
//f(x)=e^(-x)-a
//f'(x)=-e(-x)
//x_next=x-f(x)/f'(x)
//x_next=x+1-a*e^x
TLargeFloat ln(const TLargeFloat &a)
{
	//using namespace std;
	//cout<<"ln"<<a<<endl;
	if(a.m_Sign!=1)throw TLargeFloatException("TLargeFloat ln(const TLargeFloat &x)");
	if(a>TLargeFloat("1"))
	{
		return -ln(TLargeFloat(1)/a);
	}
	TLargeFloat x(-log(a.AsFloat()),TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat d(1-a*exp(x));
	d.SetDigitsLength(DEFAULT_DigitsArraySize2);
	TLargeFloat epsilon("1e-"+std::to_string(DEFAULT_Epsilon+2),
		TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat nepsilon(epsilon);
	nepsilon.m_Sign=-1;
	while(d<nepsilon||d>epsilon)
	{
		//std::cout<<x<<std::endl;
		x+=d;
		d=1-a*exp(x);
	}
	x.m_Sign*=-1;
	return x;
}

TLargeFloat pow(const TLargeFloat & x,const TLargeFloat & y)
{
	if(abs(x)<=DEFAULT_Epsilon_F)
		return x;
	if(y.isInt())
		return fastpow(x,(long long)y.AsFloat());
	else
		return exp(y*ln(x));
}

long long fastpowi(const long long & x,long long y)
{
	long long ans=1;
	long long m=x;
	while(y)
	{
		if(y&1)ans*=m;
		m*=m;
		y>>=1;
	}
	return ans;
}

TLargeFloat fastpow(const TLargeFloat & x,long long y)
{
	if(y<0)
		return TLargeFloat(1)/fastpow(x,-y);
	TLargeFloat ans(1);
	TLargeFloat m(x);
	while(y)
	{
		if(y&1)ans*=m;
		m*=m;
		y>>=1;
	}
	return ans;
}

TLargeFloat & TLargeFloat::operator%=(long double fValue)
{
	(*this)-=floor((*this)/fValue)*fValue;
	return (*this);
}

TLargeFloat & TLargeFloat::operator%=(const SelfType & Value)
{
	(*this)-=floor((*this)/Value)*Value;
	return (*this);
}

TLargeFloat sin(const TLargeFloat & x)
{
	TLargeFloat result(x);
	TLargeFloat tmp(x);
	tmp.SetDigitsLength(DEFAULT_DigitsArraySize2);
	result.SetDigitsLength(DEFAULT_DigitsArraySize2);
	TLargeFloat epsilon("1e-"+std::to_string(DEFAULT_Epsilon+2),
		TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat x2(x*x);
	x2.SetDigitsLength(DEFAULT_DigitsArraySize2);
	bool flag=true;
	for(TLargeFloat i=3,j=2;tmp>epsilon;i+=2,j+=2)
	{
		tmp*=x2;
		tmp/=i*j;
		if(flag)result-=tmp;
		else result+=tmp;
		flag=!flag;
	}
	return result;
}

TLargeFloat cos(const TLargeFloat & x)
{
	TLargeFloat result(1);
	TLargeFloat tmp(1);
	tmp.SetDigitsLength(DEFAULT_DigitsArraySize2);
	result.SetDigitsLength(DEFAULT_DigitsArraySize2);
	TLargeFloat epsilon("1e-"+std::to_string(DEFAULT_Epsilon+2),
		TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
	TLargeFloat x2(x*x);
	x2.SetDigitsLength(DEFAULT_DigitsArraySize2);
	bool flag=true;
	for(TLargeFloat i=2;tmp>epsilon;i+=2)
	{
		tmp*=x2;
		tmp/=TLargeFloat(i);
		if(flag)result-=tmp;
		else result+=tmp;
		flag=!flag;
	}
	return result;
}
