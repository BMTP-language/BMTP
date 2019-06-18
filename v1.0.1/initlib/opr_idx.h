smart_ptr<BasicVar> opr_idx(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	if(TypeList[x->first].typetype!=TypeType_array)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> opr_idx(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)";
		mes.description="particular operator is only parctical for array";
		mes.other.push_back("operator_[O"+to_string(Opr_Idx)+"]");
		mes.other.push_back("left=[T"+to_string(x->first)+"]");
		mes.other.push_back("right=[T"+to_string(y->first)+"]");
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
	try
	{
		return &(x->second.get<vector<BasicVar>>().at(ImplicitTypeConversion(*y,VarType_ll).second.get<long long>()));
	}
	catch(out_of_range err)
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="smart_ptr<BasicVar> opr_idx(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)";
		mes.description="an STL-error is caught";
		mes.other.push_back("operator_[O"+to_string(Opr_Idx)+"]");
		mes.other.push_back(err.what());
		mes.line=0;
		error.push_back(mes);
		throw error;
	}
}

smart_ptr<BasicVar> opr_idx_left(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	if(TypeList[x->first].typetype!=TypeType_array)
	{
		oneMessage mes;
		ErrorMessage err;
		mes.fun="smart_ptr<BasicVar> opr_idx_left(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)";
		mes.description="particular operator is only parctical for array";
		mes.other.push_back("operator_[O"+to_string(Opr_Idx)+"]");
		mes.other.push_back("left=[T"+to_string(x->first)+"]");
		mes.other.push_back("right=[T"+to_string(y->first)+"]");
		mes.line=0;
		err.push_back(mes);
		throw err;
	}
	long long tmp=ImplicitTypeConversion(*y,VarType_ll).second.get<long long>();
	if(tmp>=x->second.get<vector<BasicVar>>().size())x->second.get<vector<BasicVar>>().resize(tmp+1,make_pair(VarType_void,pointer()));
	try
	{
		return &(x->second.get<vector<BasicVar>>().at(tmp));
	}
	catch(out_of_range err)
	{
		oneMessage mes;
		ErrorMessage error;
		mes.fun="smart_ptr<BasicVar> opr_idx_left(smart_ptr<BasicVar> x,smart_ptr<BasicVar>y)";
		mes.description="an STL-error is caught";
		mes.other.push_back("operator_[O"+to_string(Opr_Idx)+"]");
		mes.other.push_back(err.what());
		mes.line=0;
		error.push_back(mes);
		throw error;
	}
}