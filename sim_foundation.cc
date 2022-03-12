#include "sim_foundation.h"
#include "mess_queue.h"
#include "SStd.h"
#include <string>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <strstream>
#include <cstring>

//***************************************************************************//
//***************************************************************************//
sim_foundation * sim_foundation::s_f_ = 0;
void init_file(ifstream & inFile_);
//***************************************************************************//
sim_foundation::sim_foundation(): 
	inter_network_(),
	ary_size_(0),
	cube_size_(0),
	router_counter_(0),
	packet_counter_(0),
	inFile_(),
    total_link_util_count_(0),
    total_input_buf_count_(0)
{
	s_f_ = this;
	//ary size
	ary_size_ = configuration::ap().ary_number();
	//cube size
	cube_size_ = configuration::ap().cube_number();
	//virtual channel size
	long vc_size = configuration::ap().virtual_channel_number();
	//buffer size of each virtual channel
	long buff_size = configuration::ap().buffer_size();
	//output buffer size
	long outbuff_size = configuration::ap().outbuffer_size();

	// + 1 means, one for injection
	long phy_ports_t = cube_size_ * 2 + 1;
	router_counter_ = ary_size_;
	for(long i = 0; i < cube_size_ - 1; i++) {
		//calculate the total amount of routers
		router_counter_ = router_counter_ * ary_size_;
	}
	//initalize each router, and assign the address for each of them
	add_type add_t;
	add_t.resize(cube_size_, 0);
	//initalize each router
	for(long i = 0; i < router_counter_; i++) {
		inter_network_.push_back(sim_router_template
			(phy_ports_t, vc_size, buff_size, outbuff_size, add_t, ary_size_)); 
		//assign the address of the router
		add_t[cube_size_ - 1]++;
		for(long j = cube_size_ -1; j > 0; j--) {
			if(add_t[j] == ary_size_) {
				add_t[j] = 0;
				add_t[j-1]++;
			}
		}
			
	}
	init_file();
}

//***************************************************************************//
void sim_foundation::init_file()
{
	inFile_.open(configuration::wap().trace_fname().c_str());
    if(!inFile_) {
       cerr<<"can not open source file ."<<endl;
       assert(0);
    }
}
//***************************************************************************//
ostream& operator<<(ostream& os, const sim_foundation& sf)
{
	os <<"************Router list*************"<<endl;
	vector<sim_router_template>::const_iterator first = (sf.inter_network_)
																	.begin();
	vector<sim_router_template>::const_iterator last = (sf.inter_network_)
																	  .end();
	for(; first != last; first++) {
		os<<(*first);
	}
	return os;
}

//***************************************************************************//
//calculate the router position from its address
//the lower address bit has the highest dimension
// 0   1   2   3   4   5...
//8*5 8*4 8*3 8*2 8*1 8*0
sim_router_template & sim_foundation::router(const add_type & a) 
{
	add_type::const_iterator first = a.begin();
	add_type::const_iterator last = a.end();
	long i = (* first); first++;
	for(; first!= last; first++) {
		i = i * ary_size_ + (*first);
	}
	return (inter_network_[i]);
}

//***************************************************************************//
const sim_router_template & sim_foundation::router(const 
			add_type & a) const 
{
	add_type::const_iterator first = a.begin();
	add_type::const_iterator last = a.end();
	long i = (* first); first ++;
	for(; first!= last; first++) {
		i = i * ary_size_ + (*first);
	}
	return (inter_network_[i]);
}

//***************************************************************************//
//receive a EVG information, it means a new packet is going to be injected into
//the network.
void sim_foundation::receive_EVG_message(mess_event mesg)
{
	//first, inject the flits 
    add_type sor_addr_t;
    add_type des_addr_t;
    long pack_size_t;
	//read source address
    for(long i = 0; i < cube_size_; i++) {
		long t; inFile_ >> t;
		Sassert(!inFile_.eof());
		sor_addr_t.push_back(t);
	}
	//read destination address
    for(long i = 0; i < cube_size_; i++) {
		long t; inFile_ >> t;
		Sassert(!inFile_.eof());
		des_addr_t.push_back(t);
	}
	//read packet size
	inFile_ >> pack_size_t;
	Sassert(!inFile_.eof());
	//this router will based on the current traffic condition to
	//decide whether to read the packet or not
	router(sor_addr_t).receive_packet();
	packet_counter_++;

	//second, create next EVG_ event
    if(!inFile_.eof()) {
        time_type event_time_t;
		inFile_ >> event_time_t;
		if(!inFile_.eof()) {
			mess_queue::wm_pointer().add_message(mess_event(
						event_time_t, EVG_));
		}
	}

}
//***************************************************************************//
//evaluate the address
bool sim_foundation::valid_address(const add_type & a) const {
	if(a.size() != cube_size_) {
		return false;
	}
	for(long i = 0; i < a.size(); i++) {
		if((a[i] >= ary_size_) || (a[i] < 0)) {
			return false;
		}
	}
	return true;
}

//***************************************************************************//
//the pipeline stage for each router
void sim_foundation::receive_ROUTER_message(mess_event mesg)
{
	//first add, next ROUTER_ message
	mess_queue::wm_pointer().add_message(mess_event(
				mesg.event_start() + PIPE_DELAY_, ROUTER_));

	for(long i = 0; i < router_counter_; i++) {
		inter_network_[i].router_sim_npwr();
	}
}

//***************************************************************************//
//for routers to receive flits from other routers
void sim_foundation::receive_WIRE_message(mess_event mesg)
{
	add_type des_t = mesg.des();
	long pc_t = mesg.pc();
	long vc_t = mesg.vc();
	flit_template & flits_t = mesg.get_flit();
	//cout<<"RRR"<<des_t[0]<<":"<<des_t[1]<<endl;
	router(des_t).receive_flit(pc_t, vc_t, flits_t);
}

//***************************************************************************//
//receive credit-based flow control credit flit
void sim_foundation::receive_CREDIT_message(mess_event mesg)
{
	add_type des_t = mesg.des();
	long pc_t = mesg.pc();
	long vc_t = mesg.vc();
	router(des_t).receive_credit(pc_t, vc_t);
}

//***************************************************************************//
void sim_foundation::simulation_results()
{
	vector<sim_router_template>::const_iterator first = 
									inter_network_.begin();
	vector<sim_router_template>::const_iterator last = 
									inter_network_.end();
	double total_delay = 0;
	//calculate the total delay
	first = inter_network_.begin();
	for(; first != last; first++) {
		total_delay += first->total_delay();
	}
	long tot_f_t = mess_queue::wm_pointer().total_finished();
    double c_time =  mess_queue::wm_pointer().current_time();

    long total_links;
    long total_in_bufs_space;
    long one_buf_size = configuration::ap().buffer_size();
    long num_vcs = configuration::ap().virtual_channel_number();
    // hard code here, do not use the formula to save some time
    if(ary_size_ == 8){total_links = 224;}
	if(ary_size_ == 7){total_links = 168;}
    total_in_bufs_space = total_links * num_vcs * one_buf_size;

    double in_buf_util = 100*(read_in_buf_util() /((double)c_time * (double)total_in_bufs_space));
    double link_util = 100*(read_link_util() / ((double)total_links * (double)c_time)); 

	cout.precision(6);
	cout<<"****************************************************"<<endl;
	cout<<"total finished:"<<tot_f_t<<endl;
	cout<<"total Delay is: "<< total_delay/tot_f_t<<endl;
    cout<<"Average percent (Input buffer, link) utilizations: ("<<in_buf_util<<", "<<link_util<<")"<<endl;	
	cout<<"****************************************************"<<endl;

}

//***************************************************************************//
//security check after the simulation, to check if the network is back to 
//the normal case.
void sim_foundation::simulation_check()
{
	vector<sim_router_template>::const_iterator first = 
									inter_network_.begin();
	vector<sim_router_template>::const_iterator last = 
									inter_network_.end();
	for(; first != last; first++) {
		first->empty_check();
	}
	cout<<"simulation empty check is correct.\n";
}

//***************************************************************************//
string sim_foundation:: file_name_ =
 string("Invaid file name.\n");

//***************************************************************************//

