#ifndef SIM_ROUTER_H_
#define SIM_ROUTER_H_

#include "flit.h"
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include <vector>
#include <utility>
#include <map>
#include <functional>
#include <fstream>
#include <strstream>
#include <string>
#include <cstring>
#include <set>

// *****************************************************//
// data structure to model the structure and behavior   //
// of routers.                                          //
// *****************************************************//

//module router input vc channel buffers, three dimension//
//top level is physical input ports, second level is vc  //
//channels and third level is number of buffers each vc  // 
//each vc is associated with a state objecth             //

class input_template {
	friend ostream& operator<<(ostream& os, const input_template & Ri);

	private:
		//input buffers: <phy<vc<buffer>>>
		vector<vector<vector<flit_template> > > input_;
		//the state of each input vc, which is determined by the
		//state of the flits within the vc
		//<phy<vc>>
		vector<vector<VC_state_type> > states_;
		//the candidate routing vcs, which is determined by the
		//routing algorithm for each header flit in the vc
		//<<phy<vc<mulirouting>>>
		vector<vector<vector<VC_type> > > routing_;
		//the chosen routing vc, which is determined by vc_arbitration.
		//<<phy<vc>>
		vector<vector<VC_type > > crouting_;
		//this is a flag to show that the buffer of injection is full
		//used to control the packet injection to decrease mem usage
		bool ibuff_full_;

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
	public:
		//input flits methods
		vector<vector<vector<flit_template> > > & input() {return input_;}
		const vector<vector<vector<flit_template> > > & input() const 
													   {return input_;}
		const vector<flit_template> & input(long a, long b) const 
												 {return input_[a][b];} 
		vector<flit_template> & input(long a, long b) {return input_[a][b];} 
//---------------------------------------------------------------------------//
		//add and remove flit
		void add_flit(long a, long b, const flit_template & c)
		{	//cout<<a<<":"<<b<<endl;
			//cout<<input_[a].size()<<endl;
			 input_[a][b].push_back(c);}
		void remove_flit(long a, long b)  
								 {input_[a][b].erase(input_[a][b].begin());}

		//get a flit from the input
		flit_template & get_flit(long a, long b) 
										{Sassert(input_[a][b].size() > 0);
										return(input_[a][b][0]);}
		const flit_template & get_flit(long a, long b) const{
										Sassert(input_[a][b].size() > 0);
										return(input_[a][b][0]);}
		flit_template & get_flit(long a, long b, long c) 
										{Sassert(input_[a][b].size() > c);
										return(input_[a][b][c]);}
		const flit_template & get_flit(long a, long b, long c) const{
										Sassert(input_[a][b].size() > c);
										return(input_[a][b][c]);}

//---------------------------------------------------------------------------//
		//states methods
		vector<vector<VC_state_type> > & states() {return states_;}
		const vector<vector<VC_state_type> > & states() const {return states_;}
		VC_state_type  state(long a, long b) {return states_[a][b];}
		const VC_state_type state(long a, long b) const {return states_[a][b];}

		void state_update(long a, long b, VC_state_type c){states_[a][b] = c;}
//---------------------------------------------------------------------------//

		//crouting methods
		vector<vector<VC_type > > & crouting() {return crouting_;}
		const vector<vector<VC_type > > & crouting() const {return crouting_;}
		VC_type & crouting(long a, long b) {return crouting_[a][b];}
		const VC_type & crouting(long a, long b) const {return crouting_[a][b];}
		void crouting_assign(long a, long b, VC_type c) {crouting_[a][b] = c;}
		void clear_crouting(long a, long b) {crouting_[a][b] = VC_NULL;}
//---------------------------------------------------------------------------//

		//routing methods
		vector<vector<vector<VC_type > > > & routing() 
			{return routing_;}
		const vector<vector<vector<VC_type > > > & routing()  const
			{return routing_;}
		vector<VC_type > & routing(long a, long b)
	   		{return routing_[a][b];}
		const vector<VC_type > & routing(long a, long b) const
	   		{return routing_[a][b];}
		void add_routing(long a, long b, VC_type c) 
				{ routing_[a][b].push_back(c);}
		void clear_routing(long a, long b) {routing_[a][b].clear();}
//---------------------------------------------------------------------------//
		void ibuff_is_full() {ibuff_full_ = true;}
		void ibuff_not_full() {ibuff_full_ = false;}
		bool ibuff_full() {return ibuff_full_;}
		bool ibuff_full() const {return ibuff_full_;}
//---------------------------------------------------------------------------//

		input_template();
		//a: phy ports b: virtual channel
		input_template(long a, long b);
};

//****************************************************************************//
//****************************************************************************//
class output_template {
	private:
		//used for next input
		long buffer_size_;
		vector<vector<long> >counter_;
		//has two states OECC_ or not, used for two pipeline stages
		vector<vector<VC_state_type> >flit_state_;
		//assigned for the input 
		vector<vector<VC_type> > assign_;
		//USED_ FREE_
		vector<vector<VC_usage_type> > usage_;
		//local output buffers
		vector<vector<flit_template> > outbuffers_;
		//output address
		vector<vector<VC_type> > outadd_;
		vector<long> localcounter_;
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
	public:
		long buffer_size() {return buffer_size_;}
		long buffer_size() const {return buffer_size_;}
		vector<vector<long> > & counter() {return counter_;}
		long counter(long a, long b) {return counter_[a][b];}
		long counter(long a, long b) const {return counter_[a][b];}
		void counter_inc(long a, long b) {counter_[a][b]++;}
		void counter_dec(long a, long b) {counter_[a][b]--;}
//---------------------------------------------------------------------------//
		vector<vector<VC_usage_type> > & usage() {return usage_;}
		VC_usage_type usage(long a,long b) {return usage_[a][b];}
		VC_usage_type usage(long a,long b) const {return usage_[a][b];}
		
		VC_type & assign(long a, long b) {return assign_[a][b];}
		const VC_type & assign(long a, long b) const {return assign_[a][b];}

		void acquire(long a, long b, VC_type c) 
			{usage_[a][b] = USED_; assign_[a][b] = c; }
		void release(long a, long b) 
			{usage_[a][b] = FREE_; assign_[a][b] = VC_NULL;}
//---------------------------------------------------------------------------//
		void add_flit(long a, const flit_template& b)
			{ outbuffers_[a].push_back(b); localcounter_[a]--;
			  flit_state_[a].push_back(OECC_);}

		void remove_flit(long a)
			{ outbuffers_[a].erase(outbuffers_[a].begin()); 
			  flit_state_[a].erase(flit_state_[a].begin());
			  localcounter_[a]++;}

		//void change_state();

		VC_state_type flit_state(long a) {return flit_state_[a][0];}
		VC_state_type flit_state(long a) const {return flit_state_[a][0];}

		flit_template & get_flit(long a)
	   		{Sassert(outbuffers_[a].size() > 0);
		   	return outbuffers_[a][0];}

//---------------------------------------------------------------------------//
		vector<flit_template> & outbuffers(long a) 
			{return outbuffers_[a];}
		const vector<flit_template> & outbuffers(long a) const 
			{return outbuffers_[a];}

//---------------------------------------------------------------------------//
		vector<vector<VC_type> > outadd() {return outadd_;}
		const vector<vector<VC_type> > outadd() const {return outadd_;}
		vector<VC_type> outadd(long a) {return outadd_[a];}
		const vector<VC_type> outadd(long a) const {return outadd_[a];}
		VC_type get_add(long a) {return outadd_[a][0];}
		void remove_add(long a) {Sassert(outadd_[a].size() > 0); 
			outadd_[a].erase(outadd_[a].begin());}
		void add_add(long a, VC_type b) {outadd_[a].push_back(b);}

//---------------------------------------------------------------------------//
		vector<long> & localcounter() {return localcounter_;}
		long localcounter(long a) {return localcounter_[a];}
		long localcounter(long a) const {return localcounter_[a];}
		void localcounter_inc(long a) {localcounter_[a]++;}
		void localcounter_dec(long a) {localcounter_[a]--;}
//---------------------------------------------------------------------------//


		output_template();
		output_template(long a, long b, long c, long d);
};

//***************************************************************************//
//***************************************************************************//
//globel states have not been used
class R_global_state_template {
	private:
		vector<vector<long> > vc_priority_;
		vector<vector<long> > sw_priority_;
	
	public:
		vector<vector<long> > & vc_priority() {return vc_priority_;}
		vector<vector<long> > & sw_priority() {return sw_priority_;}

		R_global_state_template();
		R_global_state_template(long a, long b);
};

//***************************************************************************//
//physical port 0 is injection port
class sim_router_template {
	friend ostream& operator<<(ostream& os, const sim_router_template & sr);

	private:
		//router address
		add_type address_;
		//the cycle delay  for each link, which is used to calulate when
		//to send out flit to the next router
		vector<time_type> clk_sync_;
		//the input buffer module
		input_template   input_module_;
		//global stage, has not been used
		R_global_state_template gstate_module_;
		//output module, used to store the flits before to the link
		output_template output_module_;
		long ary_size_;
		long physic_ports_;
		long vc_number_;
		long buffer_size_; //each vc buffer size
		long outbuffer_size_; //each vc buffer size
		//used to speedup the program speed, if the router has flits, it need
		//to be checked
		long router_activity_;
		//the total delay of the flits
		time_type total_delay_;
		//the routing algorithm used
		long routing_alg_;
		//the routing algorithm is now used, this is a pointer, points to
		//the routing procedure
		void (sim_router_template::* curr_algorithm)(const add_type &,
			   const add_type &, long, long);
		//used for the next packet injection time
		time_type local_input_time_;
		//packet counter
		long packet_counter_;
		//input trace file
		ifstream * localinFile_;

	public:
		vector<long> & address() {return address_;}
		const vector<long> & address() const {return address_;}

		//vector<flit> & inject_module() {return inject_module_;}
		//vector<flit> & eject_module() {return eject_module_;}
		vector<time_type>& clk_sync() {return clk_sync_;}
		const vector<time_type>& clk_sync() const {return clk_sync_;}
		time_type clk_sync(long a) {return clk_sync_[a];}
		time_type clk_sync(long a) const {return clk_sync_[a];}
		void clk_sync_update(long a, time_type b) {clk_sync_[a] = b;}

		input_template  & input_module() {return input_module_;}
		R_global_state_template & gstate_module() {return gstate_module_;}
		output_template & output_module() {return output_module_;}	
		long physic_ports() {return physic_ports_;}
		long vc_number() {return vc_number_;}
		long buffer_size() {return buffer_size_;}

		void accept_flit(time_type a, const flit_template & b);

		void activity_inc() {router_activity_++;}
		void activity_dec() {router_activity_--;}
	
		//simulation results and empty checking
		//show the tripple information of link util. buffer age and buffer size
		void show_trip_info() const;
		void show_flits() const;
		//empty check
		void empty_check() const;

		sim_router_template();
		//a: phy ports. b: vc number. c: buffer size. d: output buffer size.
		//e: its address  f: ary_size
		sim_router_template(long a, long b, long c, long d, const add_type & e, long f);
		//sim_router_template( const sim_router_template & a);
		//~sim_router_template(){delete localinFile_;}
		//assign the addresses of input, output, and voltage module to history
		//module
		//-------------------------------------------------------------------//
		//simulator evoking
		void router_sim_npwr();
		//-------------------------------------------------------------------//
		void switch_profile();
		//-------------------------------------------------------------------//
	
		//calculate the total delay	
		void delay_update(time_type a) { total_delay_ += a;}
		time_type total_delay() const {return total_delay_;}
		time_type total_delay() {return total_delay_;}

		//receive credit processing
		void receive_credit(long a, long b);

		//stages 0: add flits
		void receive_packet();
		void inject_packet(long a, add_type & b, add_type & c, time_type d,
				long e);
		void receive_flit(long a, long b, flit_template & c);

		//stage 0: input synchro
		//void input_ECC_synchro();
		//void output_ECC();
		//stages 1: Routing function
		//routing logic for each input header flit, decide possible
		//output vcs and set flit states flag
		//physical port 0 is injection port
		void routing_decision();
		//cycle base routing
		void ChristosPolyxeniGeorge_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);
		//dimension-based routing algorithm
		void XY_VC2_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);
		void XY_VC3_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);

		void XY_VC4_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);
		void XY_VC6_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);
												
		void XY_VC8_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);												
		void TXY_VC4_algorithm(const add_type & des_t, 
							const add_type & sor_t, long s_ph, long s_vc);
		void TXY_VC8_algorithm(const add_type & des_t, 
							const add_type & sor_t, long s_ph, long s_vc);
		//opty-based routing algorithm
		void OPTY_algorithm(const add_type & des_t, const add_type & sor_t, 
												long s_ph, long s_vc);
		void WEST_algorithm(const add_type & des_t, const add_type & sor_t,
												long s_ph, long s_vc);
		void NEGF_algorithm(const add_type & des_t, const add_type & sor_t,
												long s_ph, long s_vc);
		void DOUBLEY_algorithm(const add_type & des_t, 
							const add_type & sor_t, long s_ph, long s_vc);


		//stages 2: VC_arbitration function
		VC_type vc_selection(long a, long b);
		void vc_arbitration();

		//stages 3: SW_arbitration function
		void sw_arbitration();

		//stages 4: send to out buffer
		void flit_outbuffer();

		//stages 5: send out function
		void flit_traversal();
		void flit_traversal(long a);

		//input parse
		ifstream & localinFile() {return * localinFile_;}
		void init_local_file();
};

#endif
		


