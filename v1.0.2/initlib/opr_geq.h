smart_ptr<BasicVar> opr_geq(smart_ptr<BasicVar> x,smart_ptr<BasicVar> y)
{
	smart_ptr<BasicVar> z=opr_les(x,y);
	z->second.get<bool>()=!z->second.get<bool>();
	return z;
}