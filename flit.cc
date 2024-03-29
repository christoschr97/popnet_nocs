#include "flit.h"

ostream& operator<<(ostream& os, const flit_template & ft)
{
	os<<ft.flit_id_<<"::"<<static_cast<long> (ft.type_)<<":";
	long j = ft.sor_addr_.size();
	for(long i = 0 ; i <j; i++) {
		os << ft.sor_addr_[i]<<" ";
	}
	for(long i = 0 ; i <j; i++) {
		os << ft.des_addr_[i]<<" ";
	} os<<endl;
	return os;
}


flit_template::flit_template():
	flit_id_(),
	type_(HEADER_),
	sor_addr_(),
	des_addr_(),
	start_time_(),
	finish_time_(),
	data_(0)
{
}

flit_template::flit_template(long a, flit_type b, add_type & c, 
				 add_type & d, time_type e, long f):
	flit_id_(a),
	type_(b),
	sor_addr_(c),
	des_addr_(d),
	start_time_(e),
	data_(f)
{
}

flit_template::flit_template(const flit_template & a):
	flit_id_(a.flit_id()),
	type_(a.type()),
	sor_addr_(a.sor_addr()),
	des_addr_(a.des_addr()),
	start_time_(a.start_time()),
	finish_time_(a.finish_time()),
	data_(a.data())
{
}
