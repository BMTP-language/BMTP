BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y);

vector<signle_opr_t> opr_pow_t;

template<typename tp1,typename tp2>
tp1 fastpow(tp1 x,tp2 y)
{
	if(x==1)return 1;
	if(y<0)return tp1(1.0)/fastpow(x,tp2(0.0)-y);
	tp1 z=1;
	while(y)
	{
		if(y&1)z*=x;
		x=x*x;
		y=y>>1;
	}
	return z;
}

pointer opr_pow_tlf(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_TLargeFloat]();
	z.get<TLargeFloat>()=pow(x.get<TLargeFloat>(),y.get<TLargeFloat>());
	return z;
}

pointer opr_pow_int(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_int]();
	z.get<int>()=fastpow(x.get<int>(),y.get<int>());
	return z;
}

pointer opr_pow_uint(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_uint]();
	z.get<unsigned int>()=fastpow(x.get<unsigned int>(),y.get<unsigned int>());
	return z;
}

pointer opr_pow_ll(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ll]();
	z.get<long long>()=fastpow(x.get<long long>(),y.get<long long>());
	return z;
}

pointer opr_pow_ull(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ull]();
	z.get<unsigned long long>()=fastpow(x.get<unsigned long long>(),y.get<unsigned long long>());
	return z;
}

pointer opr_pow_short(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_short]();
	z.get<short>()=fastpow(x.get<short>(),y.get<short>());
	return z;
}

pointer opr_pow_ushort(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ushort]();
	z.get<unsigned short>()=fastpow(x.get<unsigned short>(),y.get<unsigned short>());
	return z;
}

pointer opr_pow_float(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_float]();
	z.get<float>()=powf(x.get<float>(),y.get<float>());
	return z;
}

pointer opr_pow_double(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_double]();
	z.get<double>()=pow(x.get<double>(),y.get<double>());
	return z;
}

pointer opr_pow_ldouble(const pointer& x,const pointer &y)
{
	pointer z=New[VarType_ldouble]();
	z.get<long double>()=powl(x.get<long double>(),y.get<long double>());
	return z;
}

BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y)
{
	BasicVar z;
	if(x.first==y.first)
	{
		z.first=x.first;
		if(opr_pow_t[x.first]==nullptr)
		{
			oneMessage mes;
			ErrorMessage err;
			mes.fun="BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y)";
			mes.description="particular operator is not found";
			mes.other.push_back("operator^[O"+to_string(Opr_Pow)+"]");
			mes.other.push_back("left=[T"+to_string(x.first)+"]");
			mes.other.push_back("right=[T"+to_string(y.first)+"]");
			mes.line=0;
			err.push_back(mes);
			throw err;
		}
		z.second=opr_pow_t[x.first](x.second,y.second);
		return z;
	}
	if(TypeList[x.first].typetype==TypeList[y.first].typetype)
	{
		if(TypeList[x.first].subtypetype>TypeList[y.first].subtypetype)
		{
			z.first=x.first;
			if(opr_pow_t[x.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator^[O"+to_string(Opr_Pow)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_pow_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=y.first;
			if(opr_pow_t[y.first]==nullptr)
			{
				oneMessage mes;
				ErrorMessage err;
				mes.fun="BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y)";
				mes.description="particular operator is not found";
				mes.other.push_back("operator^[O"+to_string(Opr_Pow)+"]");
				mes.other.push_back("left=[T"+to_string(x.first)+"]");
				mes.other.push_back("right=[T"+to_string(y.first)+"]");
				mes.line=0;
				err.push_back(mes);
				throw err;
			}
			z.second=opr_pow_t[y.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	//xy均为数字
	if(TypeList[x.first].typetype<=4&&TypeList[y.first].typetype<=4)
	{
		if(TypeList[y.first].typetype==TypeType_int&&TypeList[x.first].typetype!=TypeType_int)
		{
			z.first=x.first;
			z.second=New[x.first]();
			switch(x.first<<7|y.first)
			{
			case VarType_double<<7|VarType_int:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<int>());
				break;
			case VarType_ldouble<<7|VarType_int:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<int>());
				break;
			case VarType_float<<7|VarType_int:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<int>());
				break;
			case VarType_TLargeFloat<<7|VarType_int:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<int>());
				break;
			///////////////////////////////////////////////////
			case VarType_double<<7|VarType_uint:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<unsigned int>());
				break;
			case VarType_ldouble<<7|VarType_uint:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<unsigned int>());
				break;
			case VarType_float<<7|VarType_uint:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<unsigned int>());
				break;
			case VarType_TLargeFloat<<7|VarType_uint:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<unsigned int>());
				break;
			//////////////////////////////////////////////////
			case VarType_double<<7|VarType_ll:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<long long>());
				break;
			case VarType_ldouble<<7|VarType_ll:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<long long>());
				break;
			case VarType_float<<7|VarType_ll:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<long long>());
				break;
			case VarType_TLargeFloat<<7|VarType_ll:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<long long>());
				break;
			/////////////////////////////////////////////////
			case VarType_double<<7|VarType_ull:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<unsigned long long>());
				break;
			case VarType_ldouble<<7|VarType_ull:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<unsigned long long>());
				break;
			case VarType_float<<7|VarType_ull:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<unsigned long long>());
				break;
			case VarType_TLargeFloat<<7|VarType_ull:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<unsigned long long>());
				break;
			///////////////////////////////////////////////
			case VarType_double<<7|VarType_short:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<short>());
				break;
			case VarType_ldouble<<7|VarType_short:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<short>());
				break;
			case VarType_float<<7|VarType_short:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<short>());
				break;
			case VarType_TLargeFloat<<7|VarType_short:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<short>());
				break;
			///////////////////////////////////////////////
			case VarType_double<<7|VarType_ushort:
				z.second.get<double>()=fastpow(x.second.get<double>(),y.second.get<unsigned short>());
				break;
			case VarType_ldouble<<7|VarType_ushort:
				z.second.get<long double>()=fastpow(x.second.get<long double>(),y.second.get<unsigned short>());
				break;
			case VarType_float<<7|VarType_ushort:
				z.second.get<float>()=fastpow(x.second.get<float>(),y.second.get<unsigned short>());
				break;
			case VarType_TLargeFloat<<7|VarType_ushort:
				z.second.get<TLargeFloat>()=fastpow(x.second.get<TLargeFloat>(),y.second.get<unsigned short>());
				break;
			default:
				//不可能
				throw;
				break;
			}
			return z;
		}
		if(TypeList[x.first].typetype>TypeList[y.first].typetype)
		{
			z.first=x.first;
			z.second=opr_pow_t[x.first](x.second,ImplicitTypeConversion(y,x.first).second);
			return z;
		}
		else
		{
			z.first=y.first;
			z.second=opr_pow_t[x.first](ImplicitTypeConversion(x,y.first).second,y.second);
			return z;
		}
	}
	oneMessage mes;
	ErrorMessage err;
	mes.fun="BasicVar opr_pow_const(const BasicVar &x,const BasicVar &y)";
	mes.description="particular operator is not found";
	mes.other.push_back("operator^[O"+to_string(Opr_Pow)+"]");
	mes.other.push_back("left=[T"+to_string(x.first)+"]");
	mes.other.push_back("right=[T"+to_string(y.first)+"]");
	mes.line=0;
	err.push_back(mes);
	throw err;
}

smart_ptr<BasicVar> opr_pow(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	smart_ptr<BasicVar> v;
	v.malloc(opr_pow_const(*x,*y));
	return v;
}