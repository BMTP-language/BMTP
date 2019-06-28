#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_ll_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat((long double)val.get<long long>());
	return p;
}

pointer Trans_ll_ldouble(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<long double>()=(long double)val.get<long long>();
	return p;
}

pointer Trans_ll_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<long long>();
	return p;
}

pointer Trans_ll_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<long long>();
	return p;
}

pointer Trans_ll_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<long long>();
	return p;
}

pointer Trans_ll_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<long long>();
	return p;
}


pointer Trans_ll_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<long long>();
	return p;
}

pointer Trans_ll_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<long long>();
	return p;
}

pointer Trans_ll_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<long long>();
	return p;
}

pointer Trans_ll_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<long long>());
	return p;
}

pointer Trans_ll_char(const pointer &val)
{
    pointer p=New[VarType_char]();
    p.get<char>()=(char)val.get<long long>();
    return p;
}