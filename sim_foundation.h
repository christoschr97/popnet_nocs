#ifndef SIM_FOUNDATION_H_
#define SIM_FOUNDATION_H_

#include "sim_router.h"
#include "flit.h"
#include "mess_event.h"
#include <vector>
#include "configuration.h"
#include "SRGen.h"
#include <exception>
#include <utility>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>

class sim_foundation {

	friend ostream& operator<<(ostream&, const sim_foundation&);

	private:

		vector<sim_router_template> inter_network_;
		long ary_size_;
		long cube_size_;
		//total number of routers in the network
		long router_counter_;
		//may not be necessory
		long packet_counter_;

		//input trace file
		ifstream inFile_;

		static string file_name_;
		static sim_foundation * s_f_;
       
       // keep statistic for link utilization
       long total_link_util_count_;
       // keep statistic for input buffer utilization
       long total_input_buf_count_;

	public:
 // Vassos: comment this class out as it causes problems with the compilation
//		class file_error: public exception {
//			public:
//				file_error(const string & err) : what_(err) {}
//				virtual const char * what() const {return what_.c_str();}
//
//			private:
//				string what_;
//		};

		static const sim_foundation & sf() {return *s_f_;}
		static sim_foundation & wsf() {return *s_f_;}
		sim_foundation();
		~sim_foundation(){s_f_ = 0;}
		long ary_size() const {return ary_size_;}
		long cube_size() const {return cube_size_;}
		long packet_counter() {return packet_counter_;}
		long packet_counter() const {return packet_counter_;}


// following for stats ---------------------------------------------------------
        void link_util_inc() {total_link_util_count_++;}        
        double read_link_util() {return total_link_util_count_;}
        double read_link_util() const {return total_link_util_count_;} 

        void in_buf_util_add(long a) {total_input_buf_count_ += a;}        
        double read_in_buf_util() {return total_input_buf_count_;}
        double read_in_buf_util() const {return total_input_buf_count_;}
        //-------------------------------------------------

		vector<sim_router_template> & inter_network() {return inter_network_;}
		const vector<sim_router_template> & inter_network() const
	   												{return inter_network_;}
		sim_router_template & router(const add_type& a);
		const sim_router_template & router(const add_type& a) const;
		bool valid_address(const add_type & a) const;

		void receive_EVG_message(mess_event mesg);
		void receive_ROUTER_message(mess_event mesg);
		void receive_WIRE_message(mess_event mesg);
		void receive_CREDIT_message(mess_event mesg);

		void router_power_exchange();
		
		void simulation_results();
		void simulation_check();

		void init_file();

};
#endif
