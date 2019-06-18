smart_ptr<BasicVar> opr_not(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	if(y->first!=VarType_bool)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> opr_and(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)";
		mes.description="expect a Boolean variable for particular operator";
		mes.other.push_back("operator NOT[O"+to_string(Opr_Not)+"]");
		mes.other.push_back("right=[T"+to_string(y->first)+"]");
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
	smart_ptr<BasicVar> z;
	z.malloc();
	z->first=VarType_bool;
	z->second=New[VarType_bool]();
	z->second.get<bool>()=!y->second.get<bool>();
	return z;
}