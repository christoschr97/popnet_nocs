#ifndef NETWORK_INDEX_H_
#define NETWORK_INDEX_H_

#include <vector>
#include <utility>
#include <cmath>
using namespace std;

enum mess_type {EVG_, ROUTER_, WIRE_, CREDIT_};
enum routing_type {ChristosPolyxeniGeorge, XY_VC2, XY_VC3, XY_VC4, XY_VC6, XY_VC8, TXY_VC4, TXY_VC8, WEST_, NEGF_, OPTY_, DOUBLEY_};
enum VC_state_type {INIT_, ROUTING_, VC_AB_, SW_AB_, SW_TR_, HOME_, SYNC_, IECC_, OECC_, WR_TR_};
enum flit_type {HEADER_, BODY_, TAIL_};
enum switch_type {mem_write_, mem_read_, sw_travel_, link_travel_};
enum vc_share_type {SHARE_, MONO_};
enum VC_usage_type {USED_, FREE_};
enum flit_time_type {T_RECEIVE_, T_VC_AB_, T_SW_AB_, T_TR_};
typedef double time_type;
const double S_ELPS = 0.00000001;
const double REPORT_PERIOD = 10000;
const int CUBE_SIZE = 2;
typedef vector<long> add_type;
typedef pair<long, long> VC_type;
const VC_type VC_NULL = VC_type(-1, -1); 
const time_type PIPE_DELAY_ = 1.0;
const time_type CREDIT_DELAY_ = 0.9;
const time_type WIRE_DELAY_ = 0.9;
const time_type LINK_DELAY_ = 0.9999999999999;
const double INVALID_ = -1.0;
#endif
