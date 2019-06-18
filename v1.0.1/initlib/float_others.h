#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_float_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>()=TLargeFloat(val.get<float>());
	return p;
}

pointer Trans_float_ldouble(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<long double>()=(long double)val.get<float>();
	return p;
}

pointer Trans_float_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=(double)val.get<float>();
	return p;
}

pointer Trans_float_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<float>();
	return p;
}

pointer Trans_float_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<float>();
	return p;
}

pointer Trans_float_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<float>();
	return p;
}


pointer Trans_float_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<float>();
	return p;
}

pointer Trans_float_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<float>();
	return p;
}

pointer Trans_float_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<float>();
	return p;
}

pointer Trans_float_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=to_string(val.get<float>());
	return p;
}