#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_TLargeFloat_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_ldouble(const pointer &val)
{
	pointer p=New[VarType_ldouble]();
	p.get<long double>()=val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<float>()=(float)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<TLargeFloat>().AsFloat();
	return p;
}

pointer Trans_TLargeFloat_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=val.get<TLargeFloat>().AsString();
	return p;
}