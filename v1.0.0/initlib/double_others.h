#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_double_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat(val.get<double>());
	return p;
}

pointer Trans_double_ldouble(const pointer &val)
{
	pointer p=New[VarType_ldouble]();
	p.get<long double>()=(long double)val.get<double>();
	return p;
}

pointer Trans_double_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<double>();
	return p;
}

pointer Trans_double_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<double>();
	return p;
}

pointer Trans_double_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<double>();
	return p;
}

pointer Trans_double_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<double>();
	return p;
}


pointer Trans_double_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<double>();
	return p;
}

pointer Trans_double_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<double>();
	return p;
}

pointer Trans_double_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<double>();
	return p;
}

pointer Trans_double_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<double>());
	return p;
}