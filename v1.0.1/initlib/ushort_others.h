#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_ushort_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat((long double)val.get<unsigned short>());
	return p;
}

pointer Trans_ushort_ldouble(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<long double>()=(long double)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<unsigned short>();
	return p;
}


pointer Trans_ushort_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<unsigned short>();
	return p;
}

pointer Trans_ushort_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<unsigned short>());
	return p;
}

pointer Trans_ushort_char(const pointer &val)
{
    pointer p=New[VarType_char]();
    p.get<char>()=(char)val.get<unsigned short>();
    return p;
}