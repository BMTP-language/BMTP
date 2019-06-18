#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_ldouble_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat(val.get<long double>());
	return p;
}

pointer Trans_ldouble_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<long double>();
	return p;
}

pointer Trans_ldouble_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<long double>();
	return p;
}

pointer Trans_ldouble_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<long double>();
	return p;
}

pointer Trans_ldouble_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<long double>();
	return p;
}

pointer Trans_ldouble_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<long double>();
	return p;
}


pointer Trans_ldouble_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<long double>();
	return p;
}

pointer Trans_ldouble_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<long double>();
	return p;
}

pointer Trans_ldouble_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<long double>();
	return p;
}

pointer Trans_ldouble_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<long double>());
	return p;
}