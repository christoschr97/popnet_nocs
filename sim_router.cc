#include <math.h>
#include <iomanip>
#include <sstream>
#include "sim_router.h"
#include "sim_foundation.h"
#include "mess_queue.h"
#include "mess_event.h"
#include "SRGen.h"
#include "SStd.h"

// *****************************************************//
// data structure to model the structure and behavior   //
// of routers.                                          //
// *****************************************************//
//
// router top level data structure and methods          //
ostream & operator<<(ostream& os, const sim_router_template & sr) 
{
	long k = sr.address_.size();
	for(long i = 0; i < k; i++) {
		os<<sr.address_[i]<<" ";
	}
	os<<endl;
	for(long i = 0; i < sr.physic_ports_; i++) {
		for(long j = 0; j < sr.vc_number_; j++) {
			os<<sr.input_module_.input(i,j).size()<<endl;
		}
	}
	return os;
}

//**************************************************************************//
sim_router_template::sim_router_template():
	address_(),
	clk_sync_(),
	input_module_(),
	gstate_module_(),
	output_module_(),
	ary_size_(),
	physic_ports_(),
	vc_number_(),
	buffer_size_(),
	outbuffer_size_(),
	router_activity_(),
	total_delay_(),
	routing_alg_(),
	curr_algorithm(),
	local_input_time_(),
	packet_counter_(),
	localinFile_()
	
{
}
//***************************************************************************//
//a : physical ports, b: vc number, c: buffer size,  d: output
//buffer size, e: address  f: ary_size_
sim_router_template::sim_router_template(long a, long b, long c,
	long d, const add_type & e, long f):
	address_(e),
	clk_sync_(),
	input_module_(a, b),
	gstate_module_(a, b),
	output_module_(a, b, c, d),
	ary_size_(f),
	physic_ports_(a),
	vc_number_(b),
	buffer_size_(c),
	outbuffer_size_(d),
	router_activity_(0),
	total_delay_(0),
	routing_alg_(0),
	curr_algorithm(0),
	local_input_time_(),
	packet_counter_(),
	localinFile_()
{
	//get the trace file name
	init_local_file();
    localinFile() >> local_input_time_;
	//local counter init. 0
	packet_counter_= 0;

	clk_sync_.resize(a, PIPE_DELAY_);
	routing_alg_ = configuration::ap().routing_alg();
	switch (routing_alg_) {

		case ChristosPolyxeniGeorge:
		curr_algorithm = & sim_router_template::ChristosPolyxeniGeorge_algorithm;
		break;
		
		case XY_VC2 :
		curr_algorithm =  & sim_router_template::XY_VC2_algorithm;
		break;
		
		case XY_VC3 :
		curr_algorithm =  & sim_router_template::XY_VC3_algorithm;
		break;
		
		case XY_VC4 :
		curr_algorithm =  & sim_router_template::XY_VC4_algorithm;
		break;
		
		case XY_VC6 :
		curr_algorithm =  & sim_router_template::XY_VC6_algorithm;
		break;
        		
		case XY_VC8 :
		curr_algorithm =  & sim_router_template::XY_VC8_algorithm;
		break;		

		case TXY_VC4 :
		curr_algorithm = & sim_router_template::TXY_VC4_algorithm;
		break;

		case TXY_VC8 :
		curr_algorithm = & sim_router_template::TXY_VC8_algorithm;
		break;

		case WEST_ :
		curr_algorithm = & sim_router_template::WEST_algorithm;
		break;

		case NEGF_ :
		curr_algorithm = & sim_router_template::NEGF_algorithm;
		break;

		case OPTY_ :
		curr_algorithm = & sim_router_template::OPTY_algorithm;
		break;

		case DOUBLEY_ :
		curr_algorithm = & sim_router_template::DOUBLEY_algorithm;
		break;

		default :
		Sassert(0);
		break;
	}
}

//***************************************************************************//
void sim_router_template::init_local_file(){
    string name_t = configuration::wap().trace_fname();
    for(long i = 0; i < address_.size(); i++) {
       ostringstream n_t;
       n_t << address_[i];
       name_t =name_t +  "." + n_t.str();
    }
	localinFile_ = new ifstream;
    localinFile_->open(name_t.c_str());
    if(!localinFile_) {
       cerr<<"can not open trace file :"<<name_t<<endl;
       assert(0);
   }
}

//***************************************************************************//
ostream & operator<<(ostream& os, const input_template& Ri)
{
	for(long i=0; i < (Ri.input_).size(); i++) {
		for(long j=0; j < ((Ri.input_)[i]).size(); j++) {
			for(long k=0; k < ((Ri.input_)[i][j]).size(); k++) {
				os<<Ri.input_[i][j][k]<<" ";
			} os<<"|";
		} os<<endl;
	}
	return os;
}

//***************************************************************************//

input_template::input_template():
	input_(),
	states_(),
	routing_(),
	crouting_(),
	ibuff_full_()
{
}

//***************************************************************************//
//a: phy ports. b: virtual number
input_template::input_template(long a, long b):
	input_(),
	states_(),
	routing_(),
	crouting_(),
	ibuff_full_(false)
{
	input_.resize(a);
	for(long i = 0; i < a; i++) {
		input_[i].resize(b);
	}
	states_.resize(a);
	for(long i = 0; i < a; i++) {
		states_[i].resize(b, INIT_);
	}
	routing_.resize(a);
	for(long i = 0; i < a; i++) {
		routing_[i].resize(b);
	}
	crouting_.resize(a);
	for(long i = 0; i < a; i++) {
		crouting_[i].resize(b, VC_NULL);
	}
}

//***************************************************************************//
//output buffer counter module
output_template::output_template():
	buffer_size_(),
  	counter_(),
	flit_state_(),
	assign_(),
	usage_(),
	outbuffers_(),
	outadd_(),
	localcounter_()
{
}
//a: phy size. b: vir number. c: input buffer size. d: output buffer size
output_template::output_template(long a, long b, long c, long d):
	buffer_size_(c),
	counter_(),
	flit_state_(),
	assign_(),
	usage_(),
	outbuffers_(),
	outadd_(),
	localcounter_()
{
	counter_.resize(a);
	for(long i = 0; i < a; i++) {
		counter_[i].resize(b, c);
	}
	localcounter_.resize(a, d);
	assign_.resize(a);
	for(long i = 0; i < a; i++) {
		assign_[i].resize(b, VC_NULL);
	}
	usage_.resize(a);
	for(long i = 0; i < a; i++) {
		usage_[i].resize(b, FREE_);
	}
	outbuffers_.resize(a);
	flit_state_.resize(a);
	outadd_.resize(a);
}

//***************************************************************************//
//global state module                       
R_global_state_template::R_global_state_template():
	vc_priority_(),
	sw_priority_()
{
}

R_global_state_template::R_global_state_template(long a, long b):
	vc_priority_(),
	sw_priority_()
{
	vc_priority_.resize(a);
	for(long i =0; i < a; i++) {
		vc_priority_[i].resize(b, 0);
	}
	sw_priority_.resize(a);
	for(long i = 0; i < a; i++) {
		sw_priority_[i].resize(b, 0);
	}
}

//***************************************************************************//
void sim_router_template::receive_credit(long a, long b)
{
	output_module_.counter_inc(a, b);
}

//***************************************************************************//
void sim_router_template::receive_packet()
{
	time_type event_time = mess_queue::m_pointer().current_time();
	long cube_s = sim_foundation::wsf().cube_size();
	add_type sor_addr_t;
	add_type des_addr_t;
	long pack_size_t;
	long pack_c;
	while((input_module_.ibuff_full() == false) && (local_input_time_ <=
				event_time + S_ELPS)) {
		sor_addr_t.clear();
		des_addr_t.clear();
	    for(long i = 0; i < cube_s; i++) {
	        long t; localinFile() >> t;
	        if(localinFile().eof()) {
				return;
			}
	        sor_addr_t.push_back(t);
	    }
	    //read destination address
    	for(long i = 0; i < cube_s; i++) {
  	        long t; localinFile() >> t;
	        if(localinFile().eof()) {
				return;
			}
   	        des_addr_t.push_back(t);
  	    }
    	//read packet size
    	localinFile() >> pack_size_t;
	    inject_packet(packet_counter_, sor_addr_t,
	            des_addr_t, local_input_time_, pack_size_t);
	    packet_counter_++;

   	 	//second, create next EVG_ event
	    if(!localinFile().eof()) {
       	 	localinFile() >> local_input_time_;
       		if(localinFile().eof()) {
				return;
			}
    	}
	}
}
//***************************************************************************//
//a : flit id. b: sor add. c: des add. d: start time. e: size
void sim_router_template::inject_packet(long a, add_type & b, add_type & c,
			time_type d, long e)
{
	// if it is the HEADER_ flit choose the shortest waiting vc queue
	// next state, it should choose routing
	VC_type vc_t; 
	for(long l = 0; l < e; l++) {
		if(l == 0) {
			vc_t = pair<long, long> (0, input_module_.input(0,0).size());
			//cout<<"Size vc_t second is: "<<vc_t.second<<endl;
            for(long i = 0; i < vc_number_; i++) {
				long t = input_module_.input(0,i).size();
				if(vc_t.second > t){
				vc_t = pair<long, long>(i, t);
				}
			}
			//if the input buffer is empty, set it to be ROUTING_
			if(input_module_.input(0, vc_t.first).size() == 0) {
				input_module_.state_update(0, vc_t.first, ROUTING_);
			}
			//if the input buffer has more than predefined flits, then
			// add the flits and sign a flag
			if(input_module_.input(0, vc_t.first).size() > 100) {
				input_module_.ibuff_is_full();
			}
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, HEADER_, b, c, d, 0));
			//increase the router activity, if the activity > 0, this
			//router will be checked
			activity_inc();
		}else if(l == (e - 1)){
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, TAIL_, b, c, d, 0));
		}else {
			input_module_.add_flit(0, (vc_t.first),  
								flit_template(a, BODY_, b, c, d, 0));
		}
	}

}

//***************************************************************************//
//receive a flit from other router
void sim_router_template::receive_flit(long a, long b, flit_template & c)
{
	//increase the router activity, if it > 0, this router
	//will be check 
	//cout<<address_[0]<<":"<<address_[1]<<":"<<c.sor_addr()[0]<<":"<<c.sor_addr()[1]<<":"<<
	//	c.des_addr()[0]<<":"<<c.des_addr()[1]<<endl;
	//cout<<"EE"<<address_[0]<<":"<<address_[1]<<endl;
	//cout<<input_module_.input().size()<<":"<<endl;
	//cout<<c<<endl;
	//cout<<input_module_.input().size()<<endl;
	input_module_.add_flit(a, b, c);
	if(c.type() == HEADER_) {
		activity_inc();
		if(input_module_.input(a,b).size() == 1) {
			//if HEADER_ flit in empty buffer, it should set it to be
			//ROUTING_
			input_module_.state_update(a, b, ROUTING_);
		}else {
			//do not do anything, the TAIL_ flit for previous packet should
			//take care about it : set it to ROUTING_ when it goes
		}
	}else {
		//if it is set to be INIT_, it means that before it comes, other
		//flits from the same packet has gone
		if(input_module_.state(a,b) == INIT_) {
			input_module_.state_update(a, b, SW_AB_);
		}
	}
}

//***************************************************************************//
//switch arbitration pipeline stage
void sim_router_template::sw_arbitration() {
	//first, choose one possible input vc
	map<long, vector<VC_type> > vc_o_map;
	//for each physical virtual channel i
	for(long i = 0; i < physic_ports_; i++) {
		vector<long> vc_i_t1, vc_i_t2;
		//for each virtual channel in physical channel i
		for(long j = 0; j < vc_number_; j++) {
			//if this vc require switch path
			if(input_module_.state(i, j) == SW_AB_) {
				VC_type out_t = input_module_.crouting(i, j);
				//if output has empty buffers
				//may not be useful
				if((output_module_.counter(out_t.first, out_t.second) > 0) &&
				   (output_module_.localcounter(out_t.first) >= 0)) {
					vc_i_t1.push_back(j);
				}
			}
		}
		
		for(long j = vc_number_; j < vc_number_; j++) {
			//if this vc require switch path
			if(input_module_.state(i, j) == SW_AB_) {
				VC_type out_t = input_module_.crouting(i, j);
				//if output has empty buffers
				//may not be useful
				if((output_module_.counter(out_t.first, out_t.second) > 0) &&
				   (output_module_.localcounter(out_t.first) >= 0)) {
					vc_i_t2.push_back(j);
				}
			}
		}
		long vc_size_t = vc_i_t1.size();
		if(vc_size_t > 1) {
			//find the index of win vc
			long win_t = SRGen::wrg().flat_l(0, vc_size_t);
			//find it's output vc
			VC_type r_t = input_module_.crouting(i, vc_i_t1[win_t]);
			//update output physic port contension
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t1[win_t]));
		}else if(vc_size_t == 1) {
			VC_type r_t = input_module_.crouting(i, vc_i_t1[0]);
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t1[0]));
		}
		
		vc_size_t = vc_i_t2.size();
		if(vc_size_t > 1) {
			//find the index of win vc
			long win_t = SRGen::wrg().flat_l(0, vc_size_t);
			//find it's output vc
			VC_type r_t = input_module_.crouting(i, vc_i_t2[win_t]);
			//update output physic port contension
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t2[win_t]));
		}else if(vc_size_t == 1) {
			VC_type r_t = input_module_.crouting(i, vc_i_t2[0]);
			vc_o_map[r_t.first].push_back(VC_type(i, vc_i_t2[0]));
		}
	}

	if(vc_o_map.size() == 0) {
		return;
	}

	//second, choose one possible output vc
	for(long i = 0; i < physic_ports_; i++) {
		long vc_size_t = vc_o_map[i].size();
		if(vc_size_t > 0) {
			VC_type vc_win = vc_o_map[i][0];
			if(vc_size_t > 1) {
				vc_win = vc_o_map[i][SRGen::wrg().flat_l(0,
						vc_size_t)];
			}
			input_module_.state_update(vc_win.first, vc_win.second,
					SW_TR_);
			flit_template & flit_t = input_module_.get_flit(
					vc_win.first, vc_win.second);
		}
	}
}

//***************************************************************************//
//flit out buffer to the output buffer
void sim_router_template::flit_outbuffer()
{
	for(long i = 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			if(input_module_.state(i, j) == SW_TR_) {
				//get the output vc routing
				VC_type out_t = input_module_.crouting(i, j);
				//output buffer --
				//output_module_.counter_dec(out_t.first, out_t.second);

				//input buffer ++ for procedent router
				//current time used for message events generation
				time_type event_time = mess_queue::m_pointer().current_time();
				if(i != 0) {
					//if not inject module send out credit
					//precedent router address
					add_type cre_add_t = address_;
					long cre_pc_t = i;
					if((i % 2) == 0) {
						cre_pc_t = i - 1;
						cre_add_t[(i-1)/2] ++;
						if(cre_add_t[(i-1)/2] == ary_size_) {
							cre_add_t[(i-1)/2] = 0;
						}
					}else {
						cre_pc_t = i + 1;
						cre_add_t[(i-1)/2] --;
						if(cre_add_t[(i-1)/2] == -1) {
							cre_add_t[(i-1)/2] = ary_size_ - 1;
						}
					}
					mess_queue::wm_pointer().add_message(
						mess_event(event_time + CREDIT_DELAY_, 
						CREDIT_, address_, cre_add_t, cre_pc_t, j));  
				}

				long in_size_t = input_module_.input(i,j).size();
				Sassert(in_size_t >= 1);
				//more than one flits in the input vc
				//get the flit
				flit_template flit_t(input_module_.get_flit(i,j));
				//remove the flit
				input_module_.remove_flit(i, j);
				add_type des_t = flit_t.des_addr();
				//add flit to the output buffer
				if(i == 0) {
					if(input_module_.ibuff_full() == true) {
						if(input_module_.input(0,j).size() < 1000) {
							input_module_.ibuff_not_full();
							receive_packet();
						}
					}
				}
				if(address_==des_t){
				      accept_flit(event_time, flit_t);
				      if(flit_t.type() == TAIL_){
				            output_module_.release(out_t.first, out_t.second);
				      }
				}
				else{
				        output_module_.counter_dec(out_t.first, out_t.second);
					output_module_.add_flit(out_t.first, flit_t);
				        output_module_.add_add(out_t.first, out_t);
					if(flit_t.type() == TAIL_) {
						output_module_.release(out_t.first, out_t.second);
					}
				}
				//add output routing
				//release the vc 
				if(in_size_t > 1) {
					//if this the TAIL_ flit, then set ROUTING for other flits
					//for other packets
					if(flit_t.type() == TAIL_) {
						if(configuration::ap().vc_share() == MONO_) {
							if(i != 0){
							if(in_size_t != 1) {
								cout<<i<<":"<<in_size_t<<endl;
							}
							Sassert(in_size_t == 1);}
						}
						input_module_.state_update(i, j, ROUTING_);
					//if this is not TAIL_, then set SW_AB_ for other flits
					//within the same packet
					}else {
						input_module_.state_update(i, j, SW_AB_);
					}
				}else {
					input_module_.state_update(i, j, INIT_);
				}
			}
		}
	}
}

//***************************************************************************//
//flit traversal through the link stage
void sim_router_template::flit_traversal(long i)
{
	time_type event_time = mess_queue::m_pointer().current_time();
	//for(long i = 1; i < physic_ports_; i++) {
	if(output_module_.outbuffers(i).size() > 0) {
		// keep statistic
        sim_foundation::wsf().link_util_inc();
		time_type flit_delay_t = WIRE_DELAY_ + event_time;
		add_type wire_add_t = address_;
		long wire_pc_t ;
		if((i % 2) == 0) {
			wire_pc_t = i - 1;
			wire_add_t[(i - 1) / 2] ++;
			if(wire_add_t[(i-1) / 2] == ary_size_) {
				wire_add_t[(i-1) / 2] = 0;
			}
		}else {
			wire_pc_t = i + 1;
			wire_add_t[(i - 1) / 2] --;
			if(wire_add_t[(i-1) / 2] == -1) {
				wire_add_t[(i-1) / 2] = ary_size_ - 1;
			}
		}
		//get the flit
		flit_template flit_t(output_module_.get_flit(i));
		//flit_t.times_update(event_time, T_TR_);
		VC_type outadd_t = output_module_.get_add(i);
       // this is for debugging
       // this is basically the VC index
       //cout<<"In flit traversal outadd_t second is: "<<outadd_t.second<<endl;

		//remove the flit
		output_module_.remove_flit(i);
		output_module_.remove_add(i);
		//increase link usage
		//add the message of flit traversal
		mess_queue::wm_pointer().add_message(mess_event(flit_delay_t,
			WIRE_, address_, wire_add_t, wire_pc_t, 
			outadd_t.second, flit_t));
		if(flit_t.type() == TAIL_) {
			activity_dec();
		}
	}
}

//***************************************************************************//
//receive the flit at the destination router
void sim_router_template::accept_flit(time_type a, const flit_template & b)
{
    if(b.type() == TAIL_) {
		mess_queue::wm_pointer().TotFin_inc();
		time_type t = a - b.start_time();
		delay_update(t);
	}
}

//***************************************************************************//
//flit traversal through link 
void sim_router_template::flit_traversal()
{
	for(long i = 1; i < physic_ports_; i++) {
		//calculate the link clk edge
		clk_sync_[i] -= PIPE_DELAY_;
		if(clk_sync_[i] <= PIPE_DELAY_ + S_ELPS) {
			if(router_activity_ > 0) {
				flit_traversal(i);
			}
			//update the clock delay
			clk_sync_[i] += LINK_DELAY_;
		}
	}
}
//***************************************************************************//
//routing pipeline stages without voltage scaling
void sim_router_template::router_sim_npwr()
{
	Sassert(router_activity_ >=0);

	//stage 5 flit traversal
	flit_traversal();
	if(router_activity_ > 0) {
		//stage 4 flit output buffer
		flit_outbuffer();
		//stage 3 switch arbitration
		sw_arbitration();
		//stage 2, vc arbitration
		vc_arbitration();
		//stage 1, routing decision
		routing_decision();
	}
}


//***************************************************************************//
void sim_router_template::show_flits() const
{
	cout<<"--------------------------------"<<endl;
	cout<<address_[0]<<"---"<<address_[1]<<endl;
	for(long i = 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			cout<<input_module_.input(i,j).size()<<":";
			if(input_module_.input(i,j).size() > 0) {
				const flit_template & a1 = input_module_.get_flit(i,j);
				if(a1.type() == HEADER_) {
					cout<<input_module_.get_flit(i,j);
				}
				cout<<"::<"<<input_module_.state(i,j)<<":"<<
					input_module_.crouting(i,j).first<<":"<<input_module_.
				crouting(i,j).second;
			}cout<<endl;
		}
	}
}


//***************************************************************************//

void sim_router_template::empty_check() const
{
	for(long i = 0; i < physic_ports_; i ++) {
		for(long j = 0; j < vc_number_; j ++) {
			if(input_module_.input(i, j).size() > 0) {
				cout<<"Input is not empty"<<endl;
				//Sassert(0);
			}
			if(input_module_.state(i,j) != INIT_) {
				cout<<"Input state is wrong"<<endl;
				//Sassert(0);
			}cout <<output_module_.counter(i,j)<<":";
			if(output_module_.counter(i,j) != buffer_size_) {
				cout<<"Output vc counter is wrong"<<endl;
				//Sassert(0);
			}
			if(output_module_.usage(i,j) != FREE_) {
				cout<<"Output is not freed"<<endl;
				//Sassert(0);
			}
			if(output_module_.assign(i,j) != VC_NULL) {
				cout<<"Output is not freed"<<endl;
				//Sassert(0);
			}
		}
		if(output_module_.outbuffers(i).size() > 0) {
			cout<<"Output temp buffer is not empty"<<endl;
			//Sassert(0);
		}
		if(output_module_.outadd(i).size() > 0) {
			cout<<"Output temp buffer is not empty"<<endl;
			//Sassert(0);
		}
		if(output_module_.localcounter(i) != outbuffer_size_) {
			cout<<"Output local counter is not reset"<<endl;
			//Sassert(0);
		}
	}
}
