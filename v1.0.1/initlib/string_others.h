#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_string_TLargeFloat(const pointer &val)
{
	pointer p=New[VarType_TLargeFloat]();
	p.get<TLargeFloat>() = TLargeFloat();
	p.get<TLargeFloat>()=TLargeFloat(val.get<string>());
	return p;
}

pointer Trans_string_ldouble(const pointer &val)
{
	pointer p=New[VarType_ldouble]();
	p.get<long double>()=stold(val.get<string>());
	return p;
}

pointer Trans_string_double(const pointer &val)
{
	pointer p=New[VarType_double]();
	p.get<double>()=stod(val.get<string>());
	return p;
}

pointer Trans_string_float(const pointer &val)
{
	pointer p=New[VarType_float]();
	p.get<double>()=stof(val.get<string>());
	return p;
}

pointer Trans_string_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=stoi(val.get<string>());
	return p;
}

pointer Trans_string_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=stoul(val.get<string>());
	return p;
}

pointer Trans_string_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=stoll(val.get<string>());
	return p;
}

pointer Trans_string_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=stoull(val.get<string>());
	return p;
}

pointer Trans_string_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=stoi(val.get<string>());
	return p;
}

pointer Trans_string_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)stoul(val.get<string>());
	return p;
}