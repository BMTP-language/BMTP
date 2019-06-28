#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif
pointer Trans_char_uint(const pointer &val)
{
	pointer p=New[VarType_uint]();
	p.get<unsigned int>()=(unsigned int)val.get<char>();
	return p;
}

pointer Trans_char_int(const pointer &val)
{
	pointer p=New[VarType_int]();
	p.get<int>()=(int)val.get<char>();
	return p;
}


pointer Trans_char_ll(const pointer &val)
{
	pointer p=New[VarType_ll]();
	p.get<long long>()=(long long)val.get<char>();
	return p;
}

pointer Trans_char_ull(const pointer &val)
{
	pointer p=New[VarType_ull]();
	p.get<unsigned long long>()=(unsigned long long)val.get<char>();
	return p;
}

pointer Trans_char_short(const pointer &val)
{
	pointer p=New[VarType_short]();
	p.get<short>()=(short)val.get<char>();
	return p;
}

pointer Trans_char_ushort(const pointer &val)
{
	pointer p=New[VarType_ushort]();
	p.get<unsigned short>()=(unsigned short)val.get<char>();
	return p;
}

pointer Trans_char_string(const pointer &val)
{
	pointer p=New[VarType_string]();
	p.get<string>()=(string)""+val.get<char>();
	return p;
}
