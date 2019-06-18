smart_ptr<BasicVar> fun_sqr(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=sqrt(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=sqrt(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_abs(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	switch (val[0]->first)
	{
	case VarType_TLargeFloat:
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=abs(val[0]->second.get<TLargeFloat>());
		return a;
	case VarType_double:
		a->first=VarType_double;
		a->second=New[VarType_double]();
		a->second.get<double>()=std::abs(val[0]->second.get<double>());
		return a;
	case VarType_ldouble:
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=std::abs(val[0]->second.get<long double>());
		return a;
	case VarType_float:
		a->first=VarType_float;
		a->second=New[VarType_float]();
		a->second.get<float>()=std::abs(val[0]->second.get<float>());
		return a;
	case VarType_int:
		a->first=VarType_int;
		a->second=New[VarType_int]();
		a->second.get<int>()=std::abs(val[0]->second.get<int>());
		return a;
	case VarType_ll:
		a->first=VarType_ll;
		a->second=New[VarType_ll]();
		a->second.get<long long>()=std::abs(val[0]->second.get<long long>());
		return a;
	case VarType_short:
		a->first=VarType_short;
		a->second=New[VarType_short]();
		a->second.get<short>()=std::abs(val[0]->second.get<short>());
		return a;
	case VarType_uint:
		a->first=VarType_uint;
		a->second=New[VarType_uint]();
		a->second.get<unsigned int>()=(val[0]->second.get<unsigned int>());
		return a;
	case VarType_ull:
		a->first=VarType_ull;
		a->second=New[VarType_ull]();
		a->second.get<unsigned long long>()=(val[0]->second.get<unsigned long long>());
		return a;
	case VarType_ushort:
		a->first=VarType_ushort;
		a->second=New[VarType_ushort]();
		a->second.get<unsigned short>()=(val[0]->second.get<unsigned short>());
		return a;
	default:
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> fun_abs(const vector<smart_ptr<BasicVar>> &val)";
		mes.description="particular function is not found";
		mes.other.push_back("fuction=ABS");
		mes.other.push_back("[T"+to_string(val[0]->first)+"]");
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
	}
}

smart_ptr<BasicVar> fun_sin(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=sin(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=sin(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_cos(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=cos(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=cos(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_tan(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=tan(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=tan(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_arcsin(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=asin(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
	return a;
}

smart_ptr<BasicVar> fun_arccos(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=acos(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
	return a;
}

smart_ptr<BasicVar> fun_arctan(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=atan(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
	return a;
}

smart_ptr<BasicVar> fun_ln(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=ln(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=log(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_lg(const vector<smart_ptr<BasicVar>> &val)
{
	
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=log10(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
	return a;
}

smart_ptr<BasicVar> fun_exp(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=exp(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=exp(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_ceil(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=ceil(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=ceil(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_floor(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if (val[0]->first==VarType_TLargeFloat)
	{
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=floor(val[0]->second.get<TLargeFloat>());
		return a;
	}
	else
	{
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=floor(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
		return a;
	}
}

smart_ptr<BasicVar> fun_round(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=round(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>());
	return a;
}

smart_ptr<BasicVar> fun_fact(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	a->first=VarType_ldouble;
	a->second=New[VarType_ldouble]();
	a->second.get<long double>()=tgamma(ImplicitTypeConversion(*val[0],VarType_ldouble).second.get<long double>()+1);
	return a;
}

smart_ptr<BasicVar> fun_max(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if(val.size()==1)
	{
		a->first=val[0]->first;
		a->second=MemCopy(val[0]->second);
		return a;
	}
	if(val.size()==0)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> fun_max(const vector<smart_ptr<BasicVar>> &val)";
		mes.description="parameter count does not match the function";
		mes.other.push_back("fun=MAX");
		err.push_back(mes);
		throw err;
	}
	if(opr_grt(val[1],val[0])->second.get<bool>())
	{
		a->first=val[1]->first;
		a->second=MemCopy(val[1]->second);
	}
	else
	{
		a->first=val[0]->first;
		a->second=MemCopy(val[0]->second);
	}
	for(size_t i=2;i<val.size();i++)
	{
		if(opr_grt(val[i],a)->second.get<bool>())
		{
			a->first=val[i]->first;
			a->second=MemCopy(val[i]->second);
		}
	}
	return a;
}

smart_ptr<BasicVar> fun_min(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	if(val.size()==1)
	{
		a->first=val[0]->first;
		a->second=MemCopy(val[0]->second);
		return a;
	}
	if(val.size()==0)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> fun_min(const vector<smart_ptr<BasicVar>> &val)";
		mes.description="parameter count does not match the function";
		mes.other.push_back("fun=MIN");
		err.push_back(mes);
		throw err;
	}
	if(opr_les(val[1],val[0])->second.get<bool>())
	{
		a->first=val[1]->first;
		a->second=MemCopy(val[1]->second);
	}
	else
	{
		a->first=val[0]->first;
		a->second=MemCopy(val[0]->second);
	}
	for(size_t i=2;i<val.size();i++)
	{
		if(opr_les(val[i],a)->second.get<bool>())
		{
			a->first=val[i]->first;
			a->second=MemCopy(val[i]->second);
		}
	}
	return a;
}

smart_ptr<BasicVar> fun_pi(const vector<smart_ptr<BasicVar>> &val)
{
	smart_ptr<BasicVar> a;
	a.malloc();
	switch(DEFAULT_Vartype)
	{
	default:
	case VarType_TLargeFloat:
		a->first=VarType_TLargeFloat;
		a->second=New[VarType_TLargeFloat]();
		a->second.get<TLargeFloat>()=TLargeFloat("3.1415926535897932384626433832795028841971693993751058209749445923078164062\
862089986280348253421170679821480865132823066470938446095505822317253594081\
284811174502841027019385211055596446229489549303819",TLargeFloat::TDigits(DEFAULT_DigitsArraySize2));
		break;
	case VarType_double:
		a->first=VarType_double;
		a->second=New[VarType_double]();
		a->second.get<double>()=3.141592653589793238462643;
		break;
	case VarType_float:
		a->first=VarType_float;
		a->second=New[VarType_float]();
		a->second.get<float>()=3.141592653589793238462643f;
		break;
	case VarType_ldouble:
		a->first=VarType_ldouble;
		a->second=New[VarType_ldouble]();
		a->second.get<long double>()=3.141592653589793238462643L;
		break;
	case VarType_int:
		a->first=VarType_int;
		a->second=New[VarType_int]();
		a->second.get<int>()=3;
		break;
	case VarType_ll:
		a->first=VarType_ll;
		a->second=New[VarType_ll]();
		a->second.get<long long>()=3;
		break;
	case VarType_uint:
		a->first=VarType_uint;
		a->second=New[VarType_uint]();
		a->second.get<unsigned int>()=3;
		break;
	case VarType_ull:
		a->first=VarType_ull;
		a->second=New[VarType_ull]();
		a->second.get<unsigned long long>()=3;
		break;
	}
	return a;
}