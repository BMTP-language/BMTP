﻿//init.cpp
//解释器内部函数的预处理
#include "declaration.h"
using namespace std;
namespace
{
#define _FILE_INIT_CPP_
#include "initlib/TLF_others.h"
#include "initlib/ldouble_others.h"
#include "initlib/double_others.h"
#include "initlib/float_others.h"
#include "initlib/int_others.h"
#include "initlib/uint_others.h"
#include "initlib/ll_others.h"
#include "initlib/ull_others.h"
#include "initlib/short_others.h"
#include "initlib/ushort_others.h"
#include "initlib/char_others.h"
#include "initlib/string_others.h"
#include "initlib/calctwoval.h"
#include "initlib/sysfun.h"
}
//初始化基本变量
void InitTypeList()
{
	TypeList.resize(15);
	TypeList[VarType_TLargeFloat]={sizeof(TLargeFloat),TypeType_float,40};
	TypeList[VarType_ldouble]={sizeof(long double),TypeType_float,30};
	TypeList[VarType_double]={sizeof(double),TypeType_float,20};
	TypeList[VarType_float]={sizeof(float),TypeType_float,10};
	TypeList[VarType_int]={sizeof(int),TypeType_int,20};
	TypeList[VarType_uint]={sizeof(unsigned int),TypeType_int,19};
	TypeList[VarType_short]={sizeof(short),TypeType_int,10};
	TypeList[VarType_ushort]={sizeof(unsigned short),TypeType_int,9};
	TypeList[VarType_ll]={sizeof(long long),TypeType_int,30};
	TypeList[VarType_ull]={sizeof(unsigned long long),TypeType_int,29};
	TypeList[VarType_char]={sizeof(char),TypeType_char,10};
	TypeList[VarType_string]={sizeof(string),TypeType_string,40};
	TypeList[VarType_vector]={sizeof(vector<BasicVar>),TypeType_array,40};
	TypeList[VarType_bool]={sizeof(bool),TypeType_other,10};
	TypeList[VarType_void]={1,TypeType_other,1};
	////////////////////////////////////////////////
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_ldouble)]=Trans_TLargeFloat_ldouble;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_double)]=Trans_TLargeFloat_double;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_float)]=Trans_TLargeFloat_float;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_int)]=Trans_TLargeFloat_int;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_uint)]=Trans_TLargeFloat_uint;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_ll)]=Trans_TLargeFloat_ll;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_ull)]=Trans_TLargeFloat_ull;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_short)]=Trans_TLargeFloat_short;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_ushort)]=Trans_TLargeFloat_ushort;
	TypeConversion[make_pair(VarType_TLargeFloat,VarType_string)]=Trans_TLargeFloat_string;
	////////////////
	TypeConversion[make_pair(VarType_ldouble,VarType_TLargeFloat)]=Trans_ldouble_TLargeFloat;
	TypeConversion[make_pair(VarType_ldouble,VarType_double)]=Trans_ldouble_double;
	TypeConversion[make_pair(VarType_ldouble,VarType_float)]=Trans_ldouble_float;
	TypeConversion[make_pair(VarType_ldouble,VarType_int)]=Trans_ldouble_int;
	TypeConversion[make_pair(VarType_ldouble,VarType_uint)]=Trans_ldouble_uint;
	TypeConversion[make_pair(VarType_ldouble,VarType_ll)]=Trans_ldouble_ll;
	TypeConversion[make_pair(VarType_ldouble,VarType_ull)]=Trans_ldouble_ull;
	TypeConversion[make_pair(VarType_ldouble,VarType_short)]=Trans_ldouble_short;
	TypeConversion[make_pair(VarType_ldouble,VarType_ushort)]=Trans_ldouble_ushort;
	TypeConversion[make_pair(VarType_ldouble,VarType_string)]=Trans_ldouble_string;
	///////////////
	TypeConversion[make_pair(VarType_double,VarType_TLargeFloat)]=Trans_double_TLargeFloat;
	TypeConversion[make_pair(VarType_double,VarType_ldouble)]=Trans_double_ldouble;
	TypeConversion[make_pair(VarType_double,VarType_float)]=Trans_double_float;
	TypeConversion[make_pair(VarType_double,VarType_int)]=Trans_double_int;
	TypeConversion[make_pair(VarType_double,VarType_uint)]=Trans_double_uint;
	TypeConversion[make_pair(VarType_double,VarType_ll)]=Trans_double_ll;
	TypeConversion[make_pair(VarType_double,VarType_ull)]=Trans_double_ull;
	TypeConversion[make_pair(VarType_double,VarType_short)]=Trans_double_short;
	TypeConversion[make_pair(VarType_double,VarType_ushort)]=Trans_double_ushort;
	TypeConversion[make_pair(VarType_double,VarType_string)]=Trans_double_string;
	//////////////
	TypeConversion[make_pair(VarType_float,VarType_TLargeFloat)]=Trans_float_TLargeFloat;
	TypeConversion[make_pair(VarType_float,VarType_ldouble)]=Trans_float_ldouble;
	TypeConversion[make_pair(VarType_float,VarType_double)]=Trans_float_double;
	TypeConversion[make_pair(VarType_float,VarType_int)]=Trans_float_int;
	TypeConversion[make_pair(VarType_float,VarType_uint)]=Trans_float_uint;
	TypeConversion[make_pair(VarType_float,VarType_ll)]=Trans_float_ll;
	TypeConversion[make_pair(VarType_float,VarType_ull)]=Trans_float_ull;
	TypeConversion[make_pair(VarType_float,VarType_short)]=Trans_float_short;
	TypeConversion[make_pair(VarType_float,VarType_ushort)]=Trans_float_ushort;
	TypeConversion[make_pair(VarType_float,VarType_string)]=Trans_float_string;
	//////////////
	TypeConversion[make_pair(VarType_int,VarType_TLargeFloat)]=Trans_int_TLargeFloat;
	TypeConversion[make_pair(VarType_int,VarType_ldouble)]=Trans_int_ldouble;
	TypeConversion[make_pair(VarType_int,VarType_double)]=Trans_int_double;
	TypeConversion[make_pair(VarType_int,VarType_float)]=Trans_int_float;
	TypeConversion[make_pair(VarType_int,VarType_uint)]=Trans_int_uint;
	TypeConversion[make_pair(VarType_int,VarType_ll)]=Trans_int_ll;
	TypeConversion[make_pair(VarType_int,VarType_ull)]=Trans_int_ull;
	TypeConversion[make_pair(VarType_int,VarType_short)]=Trans_int_short;
	TypeConversion[make_pair(VarType_int,VarType_ushort)]=Trans_int_ushort;
	TypeConversion[make_pair(VarType_int,VarType_string)]=Trans_int_string;
	TypeConversion[make_pair(VarType_int,VarType_char)]=Trans_int_char;
	//////////////
	TypeConversion[make_pair(VarType_uint,VarType_TLargeFloat)]=Trans_uint_TLargeFloat;
	TypeConversion[make_pair(VarType_uint,VarType_ldouble)]=Trans_uint_ldouble;
	TypeConversion[make_pair(VarType_uint,VarType_double)]=Trans_uint_double;
	TypeConversion[make_pair(VarType_uint,VarType_float)]=Trans_uint_float;
	TypeConversion[make_pair(VarType_uint,VarType_int)]=Trans_uint_int;
	TypeConversion[make_pair(VarType_uint,VarType_ll)]=Trans_uint_ll;
	TypeConversion[make_pair(VarType_uint,VarType_ull)]=Trans_uint_ull;
	TypeConversion[make_pair(VarType_uint,VarType_short)]=Trans_uint_short;
	TypeConversion[make_pair(VarType_uint,VarType_ushort)]=Trans_uint_ushort;
	TypeConversion[make_pair(VarType_uint,VarType_string)]=Trans_uint_string;
	TypeConversion[make_pair(VarType_uint,VarType_char)]=Trans_uint_char;
	//////////////
	TypeConversion[make_pair(VarType_ll,VarType_TLargeFloat)]=Trans_ll_TLargeFloat;
	TypeConversion[make_pair(VarType_ll,VarType_ldouble)]=Trans_ll_ldouble;
	TypeConversion[make_pair(VarType_ll,VarType_double)]=Trans_ll_double;
	TypeConversion[make_pair(VarType_ll,VarType_float)]=Trans_ll_float;
	TypeConversion[make_pair(VarType_ll,VarType_uint)]=Trans_ll_uint;
	TypeConversion[make_pair(VarType_ll,VarType_int)]=Trans_ll_int;
	TypeConversion[make_pair(VarType_ll,VarType_ull)]=Trans_ll_ull;
	TypeConversion[make_pair(VarType_ll,VarType_short)]=Trans_ll_short;
	TypeConversion[make_pair(VarType_ll,VarType_ushort)]=Trans_ll_ushort;
	TypeConversion[make_pair(VarType_ll,VarType_string)]=Trans_ll_string;
	TypeConversion[make_pair(VarType_ll,VarType_char)]=Trans_ll_char;
	/////////////
	TypeConversion[make_pair(VarType_ull,VarType_TLargeFloat)]=Trans_ull_TLargeFloat;
	TypeConversion[make_pair(VarType_ull,VarType_ldouble)]=Trans_ull_ldouble;
	TypeConversion[make_pair(VarType_ull,VarType_double)]=Trans_ull_double;
	TypeConversion[make_pair(VarType_ull,VarType_float)]=Trans_ull_float;
	TypeConversion[make_pair(VarType_ull,VarType_uint)]=Trans_ull_uint;
	TypeConversion[make_pair(VarType_ull,VarType_int)]=Trans_ull_int;
	TypeConversion[make_pair(VarType_ull,VarType_ll)]=Trans_ull_ll;
	TypeConversion[make_pair(VarType_ull,VarType_short)]=Trans_ull_short;
	TypeConversion[make_pair(VarType_ull,VarType_ushort)]=Trans_ull_ushort;
	TypeConversion[make_pair(VarType_ull,VarType_string)]=Trans_ull_string;
	TypeConversion[make_pair(VarType_ull,VarType_char)]=Trans_ull_char;
	//////////
	TypeConversion[make_pair(VarType_short,VarType_TLargeFloat)]=Trans_short_TLargeFloat;
	TypeConversion[make_pair(VarType_short,VarType_ldouble)]=Trans_short_ldouble;
	TypeConversion[make_pair(VarType_short,VarType_double)]=Trans_short_double;
	TypeConversion[make_pair(VarType_short,VarType_float)]=Trans_short_float;
	TypeConversion[make_pair(VarType_short,VarType_uint)]=Trans_short_uint;
	TypeConversion[make_pair(VarType_short,VarType_int)]=Trans_short_int;
	TypeConversion[make_pair(VarType_short,VarType_ll)]=Trans_short_ll;
	TypeConversion[make_pair(VarType_short,VarType_ull)]=Trans_short_ull;
	TypeConversion[make_pair(VarType_short,VarType_ushort)]=Trans_short_ushort;
	TypeConversion[make_pair(VarType_short,VarType_string)]=Trans_short_string;
	TypeConversion[make_pair(VarType_short,VarType_char)]=Trans_short_char;
	/////////
	TypeConversion[make_pair(VarType_ushort,VarType_TLargeFloat)]=Trans_ushort_TLargeFloat;
	TypeConversion[make_pair(VarType_ushort,VarType_ldouble)]=Trans_ushort_ldouble;
	TypeConversion[make_pair(VarType_ushort,VarType_double)]=Trans_ushort_double;
	TypeConversion[make_pair(VarType_ushort,VarType_float)]=Trans_ushort_float;
	TypeConversion[make_pair(VarType_ushort,VarType_uint)]=Trans_ushort_uint;
	TypeConversion[make_pair(VarType_ushort,VarType_int)]=Trans_ushort_int;
	TypeConversion[make_pair(VarType_ushort,VarType_ll)]=Trans_ushort_ll;
	TypeConversion[make_pair(VarType_ushort,VarType_ull)]=Trans_ushort_ull;
	TypeConversion[make_pair(VarType_ushort,VarType_short)]=Trans_ushort_short;
	TypeConversion[make_pair(VarType_ushort,VarType_string)]=Trans_ushort_string;
	TypeConversion[make_pair(VarType_ushort,VarType_char)]=Trans_ushort_char;
	/////////
	TypeConversion[make_pair(VarType_char,VarType_uint)]=Trans_char_uint;
	TypeConversion[make_pair(VarType_char,VarType_int)]=Trans_char_int;
	TypeConversion[make_pair(VarType_char,VarType_ll)]=Trans_char_ll;
	TypeConversion[make_pair(VarType_char,VarType_ull)]=Trans_char_ull;
	TypeConversion[make_pair(VarType_char,VarType_short)]=Trans_char_short;
	TypeConversion[make_pair(VarType_char,VarType_ushort)]=Trans_char_ushort;
	TypeConversion[make_pair(VarType_char,VarType_string)]=Trans_char_string;
	/////////
	TypeConversion[make_pair(VarType_string,VarType_TLargeFloat)]=Trans_string_TLargeFloat;
	TypeConversion[make_pair(VarType_string,VarType_ldouble)]=Trans_string_ldouble;
	TypeConversion[make_pair(VarType_string,VarType_double)]=Trans_string_double;
	TypeConversion[make_pair(VarType_string,VarType_float)]=Trans_string_float;
	TypeConversion[make_pair(VarType_string,VarType_uint)]=Trans_string_uint;
	TypeConversion[make_pair(VarType_string,VarType_int)]=Trans_string_int;
	TypeConversion[make_pair(VarType_string,VarType_ll)]=Trans_string_ll;
	TypeConversion[make_pair(VarType_string,VarType_ull)]=Trans_string_ull;
	TypeConversion[make_pair(VarType_string,VarType_short)]=Trans_string_short;
	TypeConversion[make_pair(VarType_string,VarType_ushort)]=Trans_string_ushort;
}

void InitNew()
{
	New.resize(15);
	New[VarType_TLargeFloat]=New_tlargefloat;
	New[VarType_bool]=New_size1;
	New[VarType_char]=New_size1;
	New[VarType_double]=New_size8;
	New[VarType_float]=New_size4;
	New[VarType_int]=New_size4;
	New[VarType_ldouble]=New_size16;
	New[VarType_ll]=New_size8;
	New[VarType_short]=New_size2;
	New[VarType_string]=New_string;
	New[VarType_uint]=New_size4;
	New[VarType_ull]=New_size8;
	New[VarType_ushort]=New_size2;
	New[VarType_vector]=New_vector;
	New[VarType_void]=New_size1;
}

namespace
{
	void InitCalcAdd()
	{
		opr_add_t.resize(15);
		opr_add_t[VarType_TLargeFloat]=opr_add_tlf;
		opr_add_t[VarType_double]=opr_add_double;
		opr_add_t[VarType_float]=opr_add_float;
		opr_add_t[VarType_int]=opr_add_int;
		opr_add_t[VarType_ldouble]=opr_add_ldouble;
		opr_add_t[VarType_ll]=opr_add_ll;
		opr_add_t[VarType_short]=opr_add_short;
		opr_add_t[VarType_string]=opr_add_string;
		opr_add_t[VarType_uint]=opr_add_uint;
		opr_add_t[VarType_ull]=opr_add_ull;
		opr_add_t[VarType_ushort]=opr_add_ushort;
		opr_add_t[VarType_vector]=opr_add_vector;
	}
	void InitCalcSub()
	{
		opr_sub_t.resize(15);
		opr_sub_t[VarType_TLargeFloat]=opr_sub_tlf;
		opr_sub_t[VarType_double]=opr_sub_double;
		opr_sub_t[VarType_float]=opr_sub_float;
		opr_sub_t[VarType_int]=opr_sub_int;
		opr_sub_t[VarType_ldouble]=opr_sub_ldouble;
		opr_sub_t[VarType_ll]=opr_sub_ll;
		opr_sub_t[VarType_short]=opr_sub_short;
		opr_sub_t[VarType_uint]=opr_sub_uint;
		opr_sub_t[VarType_ull]=opr_sub_ull;
		opr_sub_t[VarType_ushort]=opr_sub_ushort;
		opr_sub_t[VarType_vector]=opr_sub_vector;
	}
	void InitCalcMul()
	{
		opr_mul_t.resize(15);
		opr_mul_t[VarType_TLargeFloat]=opr_mul_tlf;
		opr_mul_t[VarType_double]=opr_mul_double;
		opr_mul_t[VarType_float]=opr_mul_float;
		opr_mul_t[VarType_int]=opr_mul_int;
		opr_mul_t[VarType_ldouble]=opr_mul_ldouble;
		opr_mul_t[VarType_ll]=opr_mul_ll;
		opr_mul_t[VarType_short]=opr_mul_short;
		opr_mul_t[VarType_uint]=opr_mul_uint;
		opr_mul_t[VarType_ull]=opr_mul_ull;
		opr_mul_t[VarType_ushort]=opr_mul_ushort;
	}
	void InitCalcDiv()
	{
		opr_div_t.resize(15);
		opr_div_t[VarType_TLargeFloat]=opr_div_tlf;
		opr_div_t[VarType_double]=opr_div_double;
		opr_div_t[VarType_float]=opr_div_float;
		opr_div_t[VarType_int]=opr_div_int;
		opr_div_t[VarType_ldouble]=opr_div_ldouble;
		opr_div_t[VarType_ll]=opr_div_ll;
		opr_div_t[VarType_short]=opr_div_short;
		opr_div_t[VarType_uint]=opr_div_uint;
		opr_div_t[VarType_ull]=opr_div_ull;
		opr_div_t[VarType_ushort]=opr_div_ushort;
	}
	void InitCalcMod()
	{
		opr_mod_t.resize(15);
		opr_mod_t[VarType_TLargeFloat]=opr_mod_tlf;
		opr_mod_t[VarType_double]=opr_mod_double;
		opr_mod_t[VarType_float]=opr_mod_float;
		opr_mod_t[VarType_int]=opr_mod_int;
		opr_mod_t[VarType_ldouble]=opr_mod_ldouble;
		opr_mod_t[VarType_ll]=opr_mod_ll;
		opr_mod_t[VarType_short]=opr_mod_short;
		opr_mod_t[VarType_uint]=opr_mod_uint;
		opr_mod_t[VarType_ull]=opr_mod_ull;
		opr_mod_t[VarType_ushort]=opr_mod_ushort;
	}
	void InitCalcIdv()
	{
		opr_idv_t.resize(15);
		opr_idv_t[VarType_TLargeFloat]=opr_idv_tlf;
		opr_idv_t[VarType_double]=opr_idv_double;
		opr_idv_t[VarType_float]=opr_idv_float;
		opr_idv_t[VarType_int]=opr_idv_int;
		opr_idv_t[VarType_ldouble]=opr_idv_ldouble;
		opr_idv_t[VarType_ll]=opr_idv_ll;
		opr_idv_t[VarType_short]=opr_idv_short;
		opr_idv_t[VarType_uint]=opr_idv_uint;
		opr_idv_t[VarType_ull]=opr_idv_ull;
		opr_idv_t[VarType_ushort]=opr_idv_ushort;
	}
	void InitCalcPow()
	{
		opr_pow_t.resize(15);
		opr_pow_t[VarType_TLargeFloat]=opr_pow_tlf;
		opr_pow_t[VarType_double]=opr_pow_double;
		opr_pow_t[VarType_float]=opr_pow_float;
		opr_pow_t[VarType_int]=opr_pow_int;
		opr_pow_t[VarType_ldouble]=opr_pow_ldouble;
		opr_pow_t[VarType_ll]=opr_pow_ll;
		opr_pow_t[VarType_short]=opr_pow_short;
		opr_pow_t[VarType_uint]=opr_pow_uint;
		opr_pow_t[VarType_ull]=opr_pow_ull;
		opr_pow_t[VarType_ushort]=opr_pow_ushort;
	}
	void InitCalcEql()
	{
		opr_eql_t.resize(15);
		opr_eql_t[VarType_TLargeFloat]=opr_eql_tlf;
		opr_eql_t[VarType_double]=opr_eql_double;
		opr_eql_t[VarType_float]=opr_eql_float;
		opr_eql_t[VarType_int]=opr_eql_int;
		opr_eql_t[VarType_ldouble]=opr_eql_ldouble;
		opr_eql_t[VarType_ll]=opr_eql_ll;
		opr_eql_t[VarType_short]=opr_eql_short;
		opr_eql_t[VarType_uint]=opr_eql_uint;
		opr_eql_t[VarType_ull]=opr_eql_ull;
		opr_eql_t[VarType_ushort]=opr_eql_ushort;
		opr_eql_t[VarType_char]=opr_eql_char;
		opr_eql_t[VarType_string]=opr_eql_string;
		opr_eql_t[VarType_vector]=opr_eql_vector;
		opr_eql_t[VarType_bool]=opr_eql_bool;
	}
	void InitCalcLes()
	{
		opr_les_t.resize(15);
		opr_les_t[VarType_TLargeFloat]=opr_les_tlf;
		opr_les_t[VarType_double]=opr_les_double;
		opr_les_t[VarType_float]=opr_les_float;
		opr_les_t[VarType_int]=opr_les_int;
		opr_les_t[VarType_ldouble]=opr_les_ldouble;
		opr_les_t[VarType_ll]=opr_les_ll;
		opr_les_t[VarType_short]=opr_les_short;
		opr_les_t[VarType_uint]=opr_les_uint;
		opr_les_t[VarType_ull]=opr_les_ull;
		opr_les_t[VarType_ushort]=opr_les_ushort;
		opr_les_t[VarType_char]=opr_les_char;
		opr_les_t[VarType_string]=opr_les_string;
	}
	void InitCalcGrt()
	{
		opr_grt_t.resize(15);
		opr_grt_t[VarType_TLargeFloat]=opr_grt_tlf;
		opr_grt_t[VarType_double]=opr_grt_double;
		opr_grt_t[VarType_float]=opr_grt_float;
		opr_grt_t[VarType_int]=opr_grt_int;
		opr_grt_t[VarType_ldouble]=opr_grt_ldouble;
		opr_grt_t[VarType_ll]=opr_grt_ll;
		opr_grt_t[VarType_short]=opr_grt_short;
		opr_grt_t[VarType_uint]=opr_grt_uint;
		opr_grt_t[VarType_ull]=opr_grt_ull;
		opr_grt_t[VarType_ushort]=opr_grt_ushort;
		opr_grt_t[VarType_char]=opr_grt_char;
		opr_grt_t[VarType_string]=opr_grt_string;
	}
	void InitCalcPos()
	{
		opr_pos_t.resize(15);
		opr_pos_t[VarType_TLargeFloat]=opr_pos_tlf;
		opr_pos_t[VarType_double]=opr_pos_double;
		opr_pos_t[VarType_float]=opr_pos_float;
		opr_pos_t[VarType_int]=opr_pos_int;
		opr_pos_t[VarType_ldouble]=opr_pos_ldouble;
		opr_pos_t[VarType_ll]=opr_pos_ll;
		opr_pos_t[VarType_short]=opr_pos_short;
		opr_pos_t[VarType_uint]=opr_pos_uint;
		opr_pos_t[VarType_ull]=opr_pos_ull;
		opr_pos_t[VarType_ushort]=opr_pos_ushort;
	}
	void InitCalcNeg()
	{
		opr_neg_t.resize(15);
		opr_neg_t[VarType_TLargeFloat]=opr_neg_tlf;
		opr_neg_t[VarType_double]=opr_neg_double;
		opr_neg_t[VarType_float]=opr_neg_float;
		opr_neg_t[VarType_int]=opr_neg_int;
		opr_neg_t[VarType_ldouble]=opr_neg_ldouble;
		opr_neg_t[VarType_ll]=opr_neg_ll;
		opr_neg_t[VarType_short]=opr_neg_short;
	}
}

void InitCalcTwoVar()
{
	InitCalcAdd();
	InitCalcSub();
	InitCalcMul();
	InitCalcDiv();
	InitCalcMod();
	InitCalcIdv();
	InitCalcPow();
	InitCalcEql();
	InitCalcLes();
	InitCalcGrt();
	InitCalcPos();
	InitCalcNeg();
	CalcTwoVar.resize(25);
	CalcTwoVar[Opr_Add]=opr_add;
	CalcTwoVar[Opr_Sub]=opr_sub;
	CalcTwoVar[Opr_Mul]=opr_mul;
	CalcTwoVar[Opr_Div]=opr_div;
	CalcTwoVar[Opr_Mod]=opr_mod;
	CalcTwoVar[Opr_Pow]=opr_pow;
	CalcTwoVar[Opr_Eql]=opr_eql;
	CalcTwoVar[Opr_Neq]=opr_neq;
	CalcTwoVar[Opr_Les]=opr_les;
	CalcTwoVar[Opr_Grt]=opr_grt;
	CalcTwoVar[Opr_Leq]=opr_leq;
	CalcTwoVar[Opr_Geq]=opr_geq;
	CalcTwoVar[Opr_And]=opr_and;
	CalcTwoVar[Opr_Orr]=opr_orr;
	CalcTwoVar[Opr_Not]=opr_not;
	CalcTwoVar[Opr_Xor]=opr_xor;
	CalcTwoVar[Opr_Pos]=opr_pos;
	CalcTwoVar[Opr_Neg]=opr_neg;
	CalcTwoVar[Opr_Idx]=opr_idx;
	CalcTwoVar[Opr_Idx_left]=opr_idx_left;
}

void InitSystemFunction()
{
	SystemFunctionParameterCount.resize(20);
	SystemFunctionParameterCount[Fun_SQR]=1;
	SystemFunctionParameterCount[Fun_ABS]=1;
	SystemFunctionParameterCount[Fun_SIN]=1;
	SystemFunctionParameterCount[Fun_COS]=1;
	SystemFunctionParameterCount[Fun_TAN]=1;
	SystemFunctionParameterCount[Fun_ARCSIN]=1;
	SystemFunctionParameterCount[Fun_ARCCOS]=1;
	SystemFunctionParameterCount[Fun_ARCTAN]=1;
	SystemFunctionParameterCount[Fun_LN]=1;
	SystemFunctionParameterCount[Fun_LG]=1;
	SystemFunctionParameterCount[Fun_EXP]=1;
	SystemFunctionParameterCount[Fun_FLOOR]=1;
	SystemFunctionParameterCount[Fun_CEIL]=1;
	SystemFunctionParameterCount[Fun_ROUND]=1;
	SystemFunctionParameterCount[Fun_FACT]=1;
	SystemFunctionParameterCount[Fun_PI]=0;
	SystemFunctionParameterCount[Fun_MAX]=-1;
	SystemFunctionParameterCount[Fun_MIN]=-1;
	/////////////////////////////////////////////
	SystemFunction.resize(20);
	SystemFunction[Fun_SQR]=fun_sqr;
	SystemFunction[Fun_ABS]=fun_abs;
	SystemFunction[Fun_SIN]=fun_sin;
	SystemFunction[Fun_COS]=fun_cos;
	SystemFunction[Fun_TAN]=fun_tan;
	SystemFunction[Fun_ARCSIN]=fun_arcsin;
	SystemFunction[Fun_ARCCOS]=fun_arccos;
	SystemFunction[Fun_ARCTAN]=fun_arctan;
	SystemFunction[Fun_LN]=fun_ln;
	SystemFunction[Fun_LG]=fun_lg;
	SystemFunction[Fun_EXP]=fun_exp;
	SystemFunction[Fun_FLOOR]=fun_floor;
	SystemFunction[Fun_CEIL]=fun_ceil;
	SystemFunction[Fun_ROUND]=fun_round;
	SystemFunction[Fun_FACT]=fun_fact;
	SystemFunction[Fun_PI]=fun_pi;
	SystemFunction[Fun_MAX]=fun_max;
	SystemFunction[Fun_MIN]=fun_min;
}