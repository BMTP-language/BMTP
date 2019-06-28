#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_short_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat((long double)val.get<short>());
	return p;
}

pointer Trans_short_ldouble(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<long double>()=(long double)val.get<short>();
	return p;
}

pointer Trans_short_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<short>();
	return p;
}

pointer Trans_short_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<short>();
	return p;
}

pointer Trans_short_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<short>();
	return p;
}

pointer Trans_short_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<short>();
	return p;
}


pointer Trans_short_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<short>();
	return p;
}

pointer Trans_short_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<short>();
	return p;
}

pointer Trans_short_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<short>();
	return p;
}

pointer Trans_short_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<short>());
	return p;
}

pointer Trans_short_char(const pointer &val)
{
    pointer p=New[VarType_char]();
    p.get<char>()=(char)val.get<short>();
    return p;
}