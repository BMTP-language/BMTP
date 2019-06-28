vector<signle_opr_t2> opr_neg_t;

pointer opr_neg_tlf(const pointer &y)
{
	pointer z=New[VarType_TLargeFloat]();
	z.get<TLargeFloat>()=-y.get<TLargeFloat>();
	return z;
}

pointer opr_neg_int(const pointer &y)
{
	pointer z=New[VarType_int]();
	z.get<int>()=-y.get<int>();
	return z;
}

pointer opr_neg_ll(const pointer &y)
{
	pointer z=New[VarType_ll]();
	z.get<long long>()=-y.get<long long>();
	return z;
}

pointer opr_neg_short(const pointer &y)
{
	pointer z=New[VarType_short]();
	z.get<short>()=-y.get<short>();
	return z;
}

pointer opr_neg_float(const pointer &y)
{
	pointer z=New[VarType_float]();
	z.get<float>()=-y.get<float>();
	return z;
}

pointer opr_neg_double(const pointer &y)
{
	pointer z=New[VarType_double]();
	z.get<double>()=-y.get<double>();
	return z;
}

pointer opr_neg_ldouble(const pointer &y)
{
	pointer z=New[VarType_ldouble]();
	z.get<long double>()=-y.get<long double>();
	return z;
}

smart_ptr<BasicVar> opr_neg(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)
{
	smart_ptr<BasicVar> z;
	z.malloc();
	if(TypeList[y->first].typetype<=4)
	{
		z->first=y->first;
		z->second=opr_neg_t[y->first](y->second);
		return z;
	}
	oneMessage mes;
	ErrorMessage err;
	mes.fun="smart_ptr<BasicVar> opr_neg(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)";
	mes.description="particular operator is only parctical for int/float/complex/quaternion";
	mes.other.push_back("operator-[O"+to_string(Opr_Neg)+"]");
	mes.other.push_back("right=[T"+to_string(y->first)+"]");
	mes.line=0;
	err.push_back(mes);
	throw err;
}