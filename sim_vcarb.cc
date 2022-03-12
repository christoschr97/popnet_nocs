#include "sim_router.h"
#include "sim_foundation.h"
#include "mess_queue.h"
#include "mess_event.h"
#include "SRGen.h"
#include "SStd.h"


// *****************************************************//
// vc arbitration algorithms, which is the implementation//
// of selection function in routing                     //
// *****************************************************//
//***************************************************************************//
//choose one of the candidate routing vc
pair<long, long> sim_router_template::vc_selection(long a, long b) {

	vector<VC_type > & vc_can_t = input_module_.routing(a, b);
	long r_size_t = vc_can_t.size();
	Sassert(r_size_t > 0);
	vector<VC_type > vc_acq_t;

	for(long i = 0; i < r_size_t; i++) {
		VC_type v_t = vc_can_t[i];
		//for different routing algorithms, some can share vc some cannot
		if(configuration::ap().vc_share() == SHARE_) {
			// the vc buffer do no tneed to be empty, since we are going to 
			// share the vc between different packets
			if( (output_module_.usage(v_t.first, v_t.second) == FREE_)) {
				vc_acq_t.push_back(vc_can_t[i]);
			}
		}else {
			if(output_module_.counter(v_t.first, v_t.second) == buffer_size_) {
				if( (output_module_.usage(v_t.first, v_t.second) == FREE_)) {
						vc_acq_t.push_back(vc_can_t[i]);
				}
			}
		}
	}

	r_size_t = vc_acq_t.size();
	long vc_t = 0;
	//the candidate VC is free, then try to get it in the second
	//step of vc_arbitration
	if(r_size_t > 0) {
		if(r_size_t > 1) {
			vc_t = SRGen::wrg().flat_l(0, r_size_t);
		}
		return (vc_acq_t[vc_t]);
	//the VC is not free, then wait for next cycle try again
	}else {
		return VC_type(-1, -1);
	}
}

//***************************************************************************//
void sim_router_template::vc_arbitration() {
	//first choose one possible output vc
	map<VC_type, vector<VC_type> > vc_o_i_map;

	for(long i = 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			VC_type vc_t;
			if(input_module_.state(i, j) == VC_AB_) {
				vc_t = vc_selection(i,j);
				//make sure it is a valid vc candidate
				if((vc_t.first >= 0) && (vc_t.second >= 0)) {
					vc_o_i_map[vc_t].push_back(VC_type(i, j));
				}
			}
		}
	}
	if(vc_o_i_map.size() == 0) {
		return;
	}

	//second, arbitrate for each output vc
	//physical port 0 is used for injection port
	for(long i= 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			if(output_module_.usage(i, j) == FREE_) {
				long cont_temp = vc_o_i_map[VC_type(i,j)].size();
				if(cont_temp > 0) {
					VC_type vc_win = vc_o_i_map[VC_type(i,j)][0];
					if(cont_temp > 1) {
						vc_win = vc_o_i_map[VC_type(i,j)][SRGen::wrg().
							flat_l(0, cont_temp)];
					}
					input_module_.state_update(vc_win.first, vc_win.second,
							SW_AB_);
					input_module_.crouting_assign(vc_win.first, vc_win.second,
							VC_type(i,j));
					output_module_.acquire(i, j, vc_win);
					flit_template & flit_t = input_module_.get_flit(
						vc_win.first, vc_win.second);
					//flit_t.times_update(event_time, T_VC_AB_);
				}
			}
		}
	}
}
//***************************************************************************//

