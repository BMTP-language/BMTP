BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y);

vector<signle_opr_t> opr_eql_t;

pointer opr_eql_tlf(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(abs(x.get<TLargeFloat>()-y.get<TLargeFloat>())<=DEFAULT_Epsilon_F)
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_int(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<int>()==y.get<int>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_uint(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<unsigned int>()==y.get<unsigned int>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_ll(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<long long>()==y.get<long long>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_ull(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<unsigned long long>()==y.get<unsigned long long>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_short(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<short>()==y.get<short>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_ushort(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<unsigned short>()==y.get<unsigned short>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_float(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(std::abs(x.get<float>()-y.get<float>())<DEFAULT_Epsilon_f)
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_double(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(std::abs(x.get<double>()-y.get<double>())<DEFAULT_Epsilon_f)
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_ldouble(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(std::abs(x.get<long double>()-y.get<long double>())<DEFAULT_Epsilon_f)
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_char(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<char>()==y.get<char>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_string(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<string>()==y.get<string>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

pointer opr_eql_vector(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<vector<BasicVar>>().size()!=y.get<vector<BasicVar>>().size())
	{
		z.get<bool>()=false;
		return z;
	}
	for(size_t i=0;i<x.get<vector<BasicVar>>().size();i++)
	{
		if(!opr_eql_const(x.get<vector<BasicVar>>()[i],y.get<vector<BasicVar>>()[i]).second.get<bool>())
		{
			z.get<bool>()=false;
			return z;
		}
	}
	z.get<bool>()=true;
	return z;
}

pointer opr_eql_bool(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_bool]();
	if(x.get<bool>()==y.get<bool>())
		z.get<bool>()=true;
	else
		z.get<bool>()=false;
	return z;
}

BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y)
{
	BasicVar z;
	if(x.first==y.first)
	{
		z.first=VarType_bool;
		if(opr_eql_t[x.first]==nullptr)
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y)";
			mes.description="particular operator is not found";
			mes.other.push_back("operator=[O"+to_string(Opr_Eql)+"]");
			mes.other.push_back("left=[T"+to_string(x.first)+"]");
			mes.other.push_back("right=[T"+to_string(y.first)+"]");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		z.second=opr_eql_t[x.first](x.second,y.second);
		return z;
	}
	if(TypeList[x.first].typetype==TypeList[y.first].typetype)
	{
		if(TypeList[x.first].subtypetype>TypeList[y.first].subtypetype)
		{
			z.first=VarType_bool;
			if(opr_eql_t[x.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator=[O"+to_string(Opr_Eql)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_eql_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=VarType_bool;
			if(opr_eql_t[y.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator=[O"+to_string(Opr_Eql)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_eql_t[y.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	//xy均为数字
	if(TypeList[x.first].typetype<=4&&TypeList[y.first].typetype<=4)
	{
		if(TypeList[x.first].typetype>TypeList[y.first].typetype)
		{
			z.first=VarType_bool;
			z.second=opr_eql_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=VarType_bool;
			z.second=opr_eql_t[x.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	oneMessage mes;
	ErrorMessage err;
	mes.fun="BasicVar opr_eql_const(const BasicVar &x,const BasicVar &y)";
	mes.description="particular operator is not found";
	mes.other.push_back("operator=[O"+to_string(Opr_Eql)+"]");
	mes.other.push_back("left=[T"+to_string(x.first)+"]");
	mes.other.push_back("right=[T"+to_string(y.first)+"]");
	mes.line=0;
	err.push_back(mes);
	throw err;
}

smart_ptr<BasicVar> opr_eql(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	smart_ptr<BasicVar> v;
	v.malloc(opr_eql_const(*x,*y));
	return v;
}