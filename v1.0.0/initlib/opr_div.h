BasicVar opr_div_const(const BasicVar &x,const BasicVar &y);

vector<signle_opr_t> opr_div_t;

pointer opr_div_tlf(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_TLargeFloat]();
	z.get<TLargeFloat>()=x.get<TLargeFloat>()/y.get<TLargeFloat>();
	return z;
}

pointer opr_div_int(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_int]();
	z.get<int>()=x.get<int>()/y.get<int>();
	return z;
}

pointer opr_div_uint(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_uint]();
	z.get<unsigned int>()=x.get<unsigned int>()/y.get<unsigned int>();
	return z;
}

pointer opr_div_ll(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ll]();
	z.get<long long>()=x.get<long long>()/y.get<long long>();
	return z;
}

pointer opr_div_ull(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ull]();
	z.get<unsigned long long>()=x.get<unsigned long long>()/y.get<unsigned long long>();
	return z;
}

pointer opr_div_short(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_short]();
	z.get<short>()=x.get<short>()/y.get<short>();
	return z;
}

pointer opr_div_ushort(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ushort]();
	z.get<unsigned short>()=x.get<unsigned short>()/y.get<unsigned short>();
	return z;
}

pointer opr_div_float(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_float]();
	z.get<float>()=x.get<float>()/y.get<float>();
	return z;
}

pointer opr_div_double(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_double]();
	z.get<double>()=x.get<double>()/y.get<double>();
	return z;
}

pointer opr_div_ldouble(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ldouble]();
	z.get<long double>()=x.get<long double>()/y.get<long double>();
	return z;
}

BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)
{
	BasicVar z;
	if(x.first==y.first)
	{
		z.first=x.first;
		if(opr_div_t[x.first]==nullptr)
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)";
			mes.description="particular operator is not found";
			mes.other.push_back("operator/[O"+to_string(Opr_Div)+"]");
			mes.other.push_back("left=[T"+to_string(x.first)+"]");
			mes.other.push_back("right=[T"+to_string(y.first)+"]");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		z.second=opr_div_t[x.first](x.second,y.second);
		return z;
	}
	if(TypeList[x.first].typetype==TypeList[y.first].typetype)
	{
		if(TypeList[x.first].subtypetype>TypeList[y.first].subtypetype)
		{
			z.first=x.first;
			if(opr_div_t[x.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator/[O"+to_string(Opr_Div)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_div_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=y.first;
			if(opr_div_t[y.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator/[O"+to_string(Opr_Div)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_div_t[y.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	//xy均为数字
	if(TypeList[x.first].typetype<=4&&TypeList[y.first].typetype<=4)
	{
		if(TypeList[x.first].typetype>TypeList[y.first].typetype)
		{
			z.first=x.first;
			z.second=opr_div_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=y.first;
			z.second=opr_div_t[x.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	if(TypeList[x.first].typetype==TypeType_array&&TypeList[y.first].typetype<=4)
	{
		z.first=x.first;
		z.second=New[VarType_vector]();
		size_t s=x.second.get<vector<BasicVar>>().size();
		z.second.get<vector<BasicVar>>().resize(s);
		for(size_t i=0;i<s;i++)
		{
			try
			{
				z.second.get<vector<BasicVar>>()[i]=opr_div_const(x.second.get<vector<BasicVar>>()[i],y);
			}
			catch(ErrorMessage err)
			{
				oneMessage mes;
				mes.fun="BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)";
				mes.description="operator for an item in an array is not found";
				mes.line=0;
				mes.other.push_back("operator/[O"+to_string(Opr_Div)+"]");
				mes.other.push_back("at:"+to_string(i));
				err.push_back(mes);
				throw err;
			}
		}
		return z;
	}
	oneMessage mes;
	ErrorMessage err;
	mes.fun="BasicVar opr_div_const(const BasicVar &x,const BasicVar &y)";
	mes.description="particular operator is not found";
	mes.other.push_back("operator/[O"+to_string(Opr_Div)+"]");
	mes.other.push_back("left=[T"+to_string(x.first)+"]");
	mes.other.push_back("right=[T"+to_string(y.first)+"]");
	mes.line=0;
	err.push_back(mes);
	throw err;
}

smart_ptr<BasicVar> opr_div(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	smart_ptr<BasicVar> v;
	v.malloc(opr_div_const(*x,*y));
	return v;
}