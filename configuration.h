#ifndef NETWORK_CONFIG_H_
#define NETWORK_CONFIG_H_

/*******************************************************/
/*           Interconnection Network Simulator parse         */
/*******************************************************/

#include <string>
#include <exception>
#include "SStd.h"
#include "SRGen.h"
#include "index.h"
/*******************************************************/
class configuration {
	friend ostream & operator<<(ostream &, const configuration & cf);
	private:
		// interconnection network setup
		long ary_number_; //number of routers in one ary k-ary
		long cube_number_;  //dimension  n-cube
		long virtual_channel_number_; //number of virtual channels
		long buffer_size_; //buffer size for each vc
		long outbuffer_size_; //output buffer size
		long channel_width_; //channel width , i.e. 32 bits
		long phit_size_; //phit size, i.e. 8 bits
		long flit_size_; //flit size, i.e. 32 bits
		long packet_size_; //packet size, i.e. 5 flits of each packet
		long ran_seed_; //random seed used for generate random number
		time_type sim_length_; //simulation length i.e. 10,000,000 cycles.
	 	string trace_fname_; //simulation input file name
		long his_win_size_; //history based window size 
		long his_win_counter_; //history based ratio of voltage scaling decisions
		long routing_alg_; //choose one of the routing algorithms
		vc_share_type vc_share_;//for some routing algorithms, vc can be shared,
								//for others can not
		static string help_;
		static configuration * ap_;
	public:
		//Vassos: comment the next two classes out, compilation error
		/*class init_error: public exception {
			public:
				init_error(const string & err) : what_(err) {}
				virtual const char * what() const {return what_.c_str();}

			private:
				string what_;

		};
		class parse_error: public exception {
			public:
				parse_error(const string & err) : what_(err) {}
				virtual const char * what() const { return what_.c_str();}
			
			private:
				string what_;
		};*/

		//configuration(int argc, char * const argv [], SRGen & rgen) throw(init_error &);
		configuration(int argc, char * const argv []);

		static const configuration  & ap() {return *ap_;}
		static configuration  & wap() {return *ap_;}

		long ary_number() const {return ary_number_;}
		long ary_number() {return ary_number_;}
		long cube_number() const {return cube_number_;}
		long cube_number() {return cube_number_;}
		long virtual_channel_number() const {return virtual_channel_number_;}
		long virtual_channel_number() {return virtual_channel_number_;}
		long buffer_size() const {return buffer_size_;}
		long buffer_size() {return buffer_size_;}
		long outbuffer_size() const {return outbuffer_size_;}
		long outbuffer_size() {return outbuffer_size_;}
		long channel_width() const {return channel_width_;}
		long channel_width() {return channel_width_;}
		long phit_size() const {return phit_size_;}
		long phit_size() {return phit_size_;}
		long flit_size() const {return flit_size_;}
		long flit_size() {return flit_size_;}
		long packet_size() const {return packet_size_;}
		long packet_size() {return packet_size_;}
		time_type sim_length() const {return sim_length_;}
		time_type sim_length() {return sim_length_;}
		string trace_fname() {return trace_fname_;}
		string trace_fname() const {return trace_fname_;}
		long his_win_size() {return his_win_size_;}
		long his_win_size() const {return his_win_size_;}
		long his_win_counter() {return his_win_counter_;}
		long his_win_counter() const {return his_win_counter_;}
		long routing_alg() {return routing_alg_;}
		long routing_alg() const {return routing_alg_;}
		vc_share_type vc_share() {return vc_share_;}
		vc_share_type vc_share() const {return vc_share_;}
		~configuration() { ap_ = 0;}
};
#endif
