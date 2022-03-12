/***********************************************************/
/*                      configuration parse                */
/***********************************************************/
#include "configuration.h"
#include <vector>
#include "SRGen.h"
#include <string>
#include "SString.h"
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <strstream>

//define the static member
configuration * configuration::ap_ = 0;

configuration::configuration(int argc, char * const argv []):
//throw(init_error &):
	ary_number_(16),
	cube_number_(2),
	virtual_channel_number_(2),
	buffer_size_(4),
	outbuffer_size_(4),
	channel_width_(8),
	phit_size_(8),
	flit_size_(32),
	packet_size_(128),
	ran_seed_(1),
	sim_length_(1000),
	trace_fname_(),
	his_win_size_(10000),
	his_win_counter_(3),
	routing_alg_(0),
	vc_share_(MONO_)
{
	ap_ = this;
	const char * opt_str = "h:?:A:c:V:B:C:p:F:P:L:I:O:H:S:R:f:";
	string usage = string("usage: ") + argv[0] + " [" +
		opt_str + "] \n";

	if(argc <= 1) {
// Vassos: comment the following line out, compilation error, replace with next two lines
//		throw init_error(help_);
                cout<<help_;
                exit;

	}

	while (1) {
		long ch = getopt(argc, argv, opt_str);

		if(ch == -1) {
			break;
		}

		vector<string> vec;

		switch (ch) {

			case 'h':
// Vassos: comment the following line out, compilation error, replace with next two lines
//				throw init_error(help_);
                                cout<<help_;
                                abort();
				break;

			case 'A':
				ary_number_ = Conv(optarg);
				break;

			case 'c':
				cube_number_ = Conv(optarg);
				break;

			case 'V':
				virtual_channel_number_ = Conv(optarg);
				break;

			case 'B':
				buffer_size_ = Conv(optarg);
				break;

			case 'C':
				channel_width_ = Conv(optarg);
				break;

			case 'p':
				phit_size_ = Conv(optarg);
				break;

			case 'F':
				flit_size_ = Conv(optarg);
				break;

			case 'P':
				packet_size_ = Conv(optarg);
				break;

			case 'L':
				sim_length_ = Conv(optarg);
				break;

			case 'I':
				trace_fname_ = optarg;
				break;
		
			case 'O':
				outbuffer_size_ = Conv(optarg);
				break;

			case 'H':
				his_win_size_ = Conv(optarg);
				break;
	
			case 'S':
				ran_seed_ = Conv(optarg);
				break;

			case 'R':
				routing_alg_ = Conv(optarg);
				break;

			case 'f':
				his_win_counter_ = Conv(optarg);
				break;
	
			case '?':
// Vassos: comment the following line out, compilation error, replace with next two lines
//				throw init_error(help_);
                                cout<<help_;
                                abort();
			    break;

			default :
// Vassos: comment the following line out, compilation error, replace with next two lines
//				throw init_error(usage);
                                cout<<usage;
                                abort();
                                cout<<"here"<<endl;
				break;
		}
	}
	SRGen::wrg().set_seed(ran_seed_);
	if(routing_alg_ == OPTY_ ) {
		vc_share_ = MONO_;
	}else {
		vc_share_ = SHARE_;
	}
}
string configuration:: help_ =
	string("h: help\n") +
	"?: help\n" +
	"A: array size\n" +
	"c: cube dimension\n" +
	"V: virtual channel number\n" +
    "B: buffer size\n" +
    "O: outbuffer size\n" +
    "C: channel width\n" + 
    "p: phit size\n" +
	"F: flit size\n" +
    "P: packet size\n" +
	"L: simulation length\n" +
	"S: random seed\n" +
	"I: trace file\n" +
	"R: routing algorithm: 0-dimension 1-opty\n" +
	"H: history window size\n" +
	"f: history windows counter\n";

ostream & operator<<(ostream & os, const configuration & cf) {
	os <<"****************configuration********************\n"<<
		"ary size:"<<cf.ary_number_<<"\n"<<
		"cube dimension:"<<cf.cube_number_<<"\n"<<
		"virtual channel number:"<<cf.virtual_channel_number_<<"\n"<<
		"buffer size:"<<cf.buffer_size_<<"\n"<<
		"outbuffer size:"<<cf.outbuffer_size_<<"\n"<<
		"channel width:"<<cf.channel_width_<<"\n"<<
		"phit size:"<<cf.phit_size_<<"\n"<<
		"flit size:"<<cf.flit_size_<<"\n"<<
		"packet size:"<<cf.packet_size_<<"\n"<<
		"simulation length:" <<cf.sim_length_<<"\n"<<
		"trace file:"<<cf.trace_fname_.c_str()<<"\n"<<
		"Routing algorithm:"<<cf.routing_alg_<<"\n"<<
		"History window size:"<<cf.his_win_size_<<"\n"<<
		"history windows counter:"<<cf.his_win_counter_<<"\n";
	return os;
}
