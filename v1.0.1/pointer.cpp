//pointer.cpp
//class pointer的实现
//作者：张子辰
#include "declaration.h"
using namespace std;

namespace
{
	void smartfree(void* p,int type)
	{
		switch(type)
		{
		case -1:
		case -2:
		case -4:
		case -8:
		case -16:
			free(p);
			break;
		case 1:
			delete (TLargeFloat*)p;
			break;
		case 2:
			delete (string*)p;
			break;
		case  3:
			delete (vector<BasicVar>*)p;
			break;
		default:
			//不可能
			throw;
			break;
		}
	}
}

pointer New_size1()
{
	pointer ptr;
	ptr.m_data=malloc((size_t)1);
	memset(ptr.m_data,0,(size_t)1);
	ptr.shared_count=new int(1);
	ptr.ptr_type=-1;
	return ptr;
}

pointer New_size2()
{
	pointer ptr;
	ptr.m_data=malloc((size_t)2);
	memset(ptr.m_data,0,(size_t)2);
	ptr.shared_count=new int(1);
	ptr.ptr_type=-2;
	return ptr;
}

pointer New_size4()
{
	pointer ptr;
	ptr.m_data=malloc((size_t)4);
	memset(ptr.m_data,0,(size_t)4);
	ptr.shared_count=new int(1);
	ptr.ptr_type=-4;
	return ptr;
}

pointer New_size8()
{
	pointer ptr;
	ptr.m_data=malloc((size_t)8);
	memset(ptr.m_data,0,(size_t)8);
	ptr.shared_count=new int(1);
	ptr.ptr_type=-8;
	return ptr;
}

pointer New_size16()
{
	pointer ptr;
	ptr.m_data=malloc((size_t)16);
	memset(ptr.m_data,0,(size_t)16);
	ptr.shared_count=new int(1);
	ptr.ptr_type=-16;
	return ptr;
}

pointer New_tlargefloat()
{
	pointer ptr;
	ptr.m_data=new TLargeFloat;
	ptr.shared_count=new int(1);
	ptr.ptr_type=1;
	return ptr;
}

pointer New_string()
{
	pointer ptr;
	ptr.m_data=new string;
	ptr.shared_count=new int(1);
	ptr.ptr_type=2;
	return ptr;
}

pointer New_vector()
{
	pointer ptr;
	ptr.m_data=new vector<BasicVar>;
	ptr.shared_count=new int(1);
	ptr.ptr_type=3;
	return ptr;
}

void Delete(const pointer &ptr)
{
	//abs(shared_count)表示m_data被共享的次数
	//shared_count>0表示m_data对应的地址未被释放
	//shared_count<0表示m_data对应的地址已被释放
	if(ptr.shared_count==nullptr||*ptr.shared_count<0)
		return;
	smartfree(ptr.m_data,ptr.ptr_type);
	(*ptr.shared_count)=-(*ptr.shared_count);
}

pointer::pointer()
{
	m_data=nullptr;
	shared_count=nullptr;
	ptr_type=-1;
}

pointer::pointer(const pointer &other)
{
	m_data=other.m_data;
	shared_count=other.shared_count;
	if(shared_count!=nullptr)
		(*shared_count)>0?++(*shared_count):--(*shared_count);
	ptr_type=other.ptr_type;
}

pointer::~pointer()
{
	if(shared_count==nullptr)return;
	//this已被Delete删除过
	if(*shared_count<0)
	{
		if(++(*shared_count)==0)
		{
			delete shared_count;
		}
	}
	else
	{
		if(--(*shared_count)==0)
		{
			smartfree(m_data,ptr_type);
			delete shared_count;
		}
	}
}

pointer& pointer::operator=(const pointer &other)
{
	if(other.shared_count!=nullptr)
		(*other.shared_count)>0?++(*other.shared_count):--(*other.shared_count);
	if(shared_count!=nullptr)
	{
		if((*shared_count)>0)
		{
			if(--(*shared_count)==0)
			{
				smartfree(m_data,ptr_type);
				delete shared_count;
			}
		}
		else
		{
			if(++(*shared_count)==0)
			{
				delete shared_count;
			}
		}
	}
	m_data=other.m_data;
	shared_count=other.shared_count;
	ptr_type=other.ptr_type;
	return *this;
}

pointer MemCopy(const pointer &p)
{
	pointer q;
	switch (p.ptr_type)
	{
	case -1:
		q=New_size1();
		memcpy(q.m_data,p.m_data,1);
		break;
	case -2:
		q=New_size2();
		memcpy(q.m_data,p.m_data,2);
		break;
	case -4:
		q=New_size4();
		memcpy(q.m_data,p.m_data,4);
		break;
	case -8:
		q=New_size8();
		memcpy(q.m_data,p.m_data,8);
		break;
	case -16:
		q=New_size16();
		memcpy(q.m_data,p.m_data,16);
		break;
	case 1:
		q=New_tlargefloat();
		(*((TLargeFloat*)q.m_data))=(*((TLargeFloat*)p.m_data));
		break;
	case 2:
		q=New_string();
		(*((string*)q.m_data))=(*((string*)p.m_data));
		break;
	case 3:
		q=New_vector();
		(*((vector<BasicVar>*)q.m_data))=(*((vector<BasicVar>*)p.m_data));
		break;
	default:
		//不可能
		throw;
	}
	return q;
}