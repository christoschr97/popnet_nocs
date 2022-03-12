#include "sim_router.h"
#include "sim_foundation.h"
#include "mess_queue.h"
#include "mess_event.h"
#include "SRGen.h"
#include "SStd.h"


//***************************************************************************//
void sim_router_template::ChristosPolyxeniGeorge_algorithm(const add_type & des_t, const add_type & sor_t, long s_ph, long s_vc) {
  //cout<<"XY mesh routing, 2VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];
	int x = address_[0] % 2;
	int y = address_[1] % 2;
	time_type event_time = mess_queue::m_pointer().current_time();
	int cycle = (int) event_time;
        //cerr<<"Cycle:"<<event_time<<endl;

  if (xoffset != 0 && yoffset != 0) {
		if ((cycle % 2 != 0 && x != 0 && y != 0) || (cycle % 2 != 0 && x == 0 && y != 0) ||(cycle % 2 == 0 && x != 0 && y != 0) || (cycle % 2 == 0 && x == 0 && y != 0)){
	 		if (yoffset < 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(3, 0));
				input_module_.add_routing(s_ph, s_vc, VC_type(3, 1));
				input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
				input_module_.add_routing(s_ph, s_vc, VC_type(3, 3));
			} 	
			else if (yoffset > 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(4, 0));
				input_module_.add_routing(s_ph, s_vc, VC_type(4, 1));
				input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
				input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
			}
	}
   	else if ((cycle % 2 != 0 && x != 0 && y == 0) ||(cycle % 2 != 0 && x == 0 && y == 0) ||(cycle % 2 == 0 && x != 0 && y == 0) ||(cycle % 2 == 0 && x == 0 && y == 0)) {
			if (xoffset < 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
				input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
				input_module_.add_routing(s_ph, s_vc, VC_type(1, 2));
				input_module_.add_routing(s_ph, s_vc, VC_type(1, 3));
			} 	
			else if (xoffset > 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
				input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
				input_module_.add_routing(s_ph, s_vc, VC_type(2, 2));
				input_module_.add_routing(s_ph, s_vc, VC_type(2, 3));
			}
		}
 	}		
  else {
	if (yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3, 0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3, 1));
		input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
		input_module_.add_routing(s_ph, s_vc, VC_type(3, 3));
	} 
	else if (yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4, 0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4, 1));
		input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
		input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
	} 
	else if (xoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
		input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
		input_module_.add_routing(s_ph, s_vc, VC_type(1, 2));
		input_module_.add_routing(s_ph, s_vc, VC_type(1, 3));
	} 
	else if (xoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
		input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
		input_module_.add_routing(s_ph, s_vc, VC_type(2, 2));
		input_module_.add_routing(s_ph, s_vc, VC_type(2, 3));
  } 
	else {
		for (int iter = 0; iter < vc_number_; iter++) {
			input_module_.add_routing(s_ph, s_vc, VC_type(0, iter));
		}
	} 
 }
}

//***************************************************************************/
void sim_router_template::XY_VC2_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"XY mesh routing, 2VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	if(yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
	}else if(yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
		}else if (xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
		}
		else{
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// last else	
	}
}
//***************************************************************************//
void sim_router_template::XY_VC3_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"XY mesh routing, 3VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	if(yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,2));
	}else if(yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,2));
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,2));
		}else if (xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,2));
		}
		else{
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// last else			
	}
}
//***************************************************************************//
void sim_router_template::XY_VC4_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"XY mesh routing, 4VCs per link"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	if(yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,3));
	}else if(yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,3));
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,3));
		}else if (xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,3));
		}
		else{
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// last else			
	}
}
//***************************************************************************//
void sim_router_template::XY_VC6_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"XY mesh routing, 6VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	if(yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,3));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,4));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,5));     		
	}else if(yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,3));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,4));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,5));       		
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,3));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,4));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,5));           			
		}else if (xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,3));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,4));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,5));           			
		}
		else{
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// last else			
	}
}
//***************************************************************************//
void sim_router_template::XY_VC8_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"XY mesh routing, 8VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	if(yoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,3));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,4));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,5));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,6));
		input_module_.add_routing(s_ph, s_vc, VC_type(3,7));        		
	}else if(yoffset > 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,2));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,3));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,4));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,5));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,6));
		input_module_.add_routing(s_ph, s_vc, VC_type(4,7));        		
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,3));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,4));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,5));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,6));
			input_module_.add_routing(s_ph, s_vc, VC_type(1,7));            			
		}else if (xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,2));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,3));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,4));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,5));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,6));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,7));           			
		}
		else{
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// last else			
	}
}
//***************************************************************************//			
void sim_router_template::TXY_VC4_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"TXY routing, 4VCs"<<endl;
    long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];
	bool xdirection = (abs(static_cast<int>(xoffset)) * 2 
									<= ary_size_)? true: false; 
	bool ydirection = (abs(static_cast<int>(yoffset)) * 2 
									<= ary_size_)? true: false; 

	if(xdirection) {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
		}else if(xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
		}else {
			if(ydirection) {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
				}
			}else {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 3)); 
				}
			}
		}
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
		}else if(xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
		}else {
			if(ydirection) {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
				}
			}else {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
				}else if(yoffset> 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 3)); 
				}
            }
         }
       } // end of ydirection
		if(xoffset ==0 && yoffset ==0) {
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		} // x and y offsets are zero, i.e. to eject							
}
//***************************************************************************//			
void sim_router_template::TXY_VC8_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"TXY routing, 8VCs"<<endl;
    long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];
	bool xdirection = (abs(static_cast<int>(xoffset)) * 2 
									<= ary_size_)? true: false; 
	bool ydirection = (abs(static_cast<int>(yoffset)) * 2 
									<= ary_size_)? true: false; 

	if(xdirection) {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 4));
		}else if(xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 5));
		}else {
			if(ydirection) {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 6));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 7));
				}
			}else {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 6));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 3)); 
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 7)); 
				}
			}
		}
	}else {
		if(xoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2, 4));
		}else if(xoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
			input_module_.add_routing(s_ph, s_vc, VC_type(1, 5));
		}else {
			if(ydirection) {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 2));
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 6));
				}else if(yoffset > 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 3));
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 7));
				}
			}else {
				if(yoffset < 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 2));
					input_module_.add_routing(s_ph, s_vc, VC_type(4, 6));
				}else if(yoffset> 0) {
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 3)); 
					input_module_.add_routing(s_ph, s_vc, VC_type(3, 7)); 
				}
            }
          }
       }   // end of ydirection
		if(xoffset ==0 && yoffset ==0){
			for(int iter=0; iter<vc_number_; iter++){
				input_module_.add_routing(s_ph, s_vc, VC_type(0,iter));
			}
		}	// x and y offsets are zero, i.e. to eject					
}

//***************************************************************************//
void sim_router_template::NEGF_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc)
{
	//cout<<"NEGF mesh routing routing, 2VCs"<<endl;	
    long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	//X- 1 X+ 2 Y- 3 Y+ 4
	//VC 0 and 1
	if(xoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}
	}else if(xoffset > 0) { 
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else {
			input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
			if(yoffset> 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
			}
		}
	}else {
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}
}

//***************************************************************************//
void sim_router_template::WEST_algorithm(const add_type & des_t,
		const add_type & sor_t, long s_ph, long s_vc )
{
	//cout<<"WEST mesh routing routing, 2VCs"<<endl;
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	//X- 1 X+ 2 Y- 3 Y+ 4
	//VC 0 and 1
	if(xoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
	}else if(xoffset > 0) { 
		input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}else {
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}
}

//***************************************************************************//
void sim_router_template::DOUBLEY_algorithm(const add_type & des_t, const 
		add_type & sor_t, long s_ph, long s_vc)
{
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];
	long xpora = des_t[0] - sor_t[0];

	//X- 1 X+ 2 Y- 3 Y+ 4
	//VC 0 and 1
	vector<VC_type> xcand_port_t;
	vector<VC_type> ycand_port_t;
	if(xoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
		}
	}else if(xoffset > 0) { 
		input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}else {
		if(yoffset < 0) {
			if(xpora < 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			}else if(xpora > 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
			}else {
				input_module_.add_routing(s_ph, s_vc, VC_type(3,s_vc));
			}
		}else if(yoffset > 0) {
			if(xpora < 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			}else if(xpora > 0) {
				input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
			}else {
				input_module_.add_routing(s_ph, s_vc, VC_type(4,s_vc));
			}
		}
	}
}

//***************************************************************************//
void sim_router_template::OPTY_algorithm(const add_type & des_t,
	   const add_type & sor_t,	long s_ph, long s_vc)
{
	long xoffset = des_t[0] - address_[0];
	long yoffset = des_t[1] - address_[1];

	//X- 1 X+ 2 Y- 3 Y+ 4
	//VC 0 and 1
	if(xoffset < 0) {
		input_module_.add_routing(s_ph, s_vc, VC_type(1,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(1,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}else if(xoffset > 0) { 
		input_module_.add_routing(s_ph, s_vc, VC_type(2,0));
		input_module_.add_routing(s_ph, s_vc, VC_type(2,1));
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}else {
		if(yoffset < 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(3,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(3,1));
		}else if(yoffset > 0) {
			input_module_.add_routing(s_ph, s_vc, VC_type(4,0));
			input_module_.add_routing(s_ph, s_vc, VC_type(4,1));
		}
	}
}

// //***************************************************************************//
// void sim_router_template::CPG_algorithm(const add_type & des_t,
// 		const add_type & sor_t, long s_ph, long s_vc)
// {
//   //cout<<"XY mesh routing, 2VCs"<<endl;
// 	long xoffset = des_t[0] - address_[0];
// 	long yoffset = des_t[1] - address_[1];
// 	int x = address_[0] % 2;
// 	int y = address_[1] % 2;
// 	time_type event_time = mess_queue::m_pointer().current_time();
// 	int cycle = (int) event_time;
//         //cerr<<"Cycle:"<<event_time<<endl;

//    if (xoffset != 0 && yoffset != 0) 
// {

//   if  ((cycle % 2 != 0 && x != 0 && y != 0) ||(cycle % 2 != 0 && x == 0 && y != 0)
//       ||(cycle % 2 == 0 && x != 0 && y != 0) ||(cycle % 2 == 0 && x == 0 && y != 0))
// 	{
// 	 		if (yoffset < 0) 
// 			{
//       				input_module_.add_routing(s_ph, s_vc, VC_type(3, 0));
//       				input_module_.add_routing(s_ph, s_vc, VC_type(3, 1));
//     			} 	
// 			else if (yoffset > 0) 
// 			{
//       				input_module_.add_routing(s_ph, s_vc, VC_type(4, 0));
//       				input_module_.add_routing(s_ph, s_vc, VC_type(4, 1));
//       			}
// 	}

//    else if   ((cycle % 2 != 0 && x != 0 && y == 0) ||(cycle % 2 != 0 && x == 0 && y == 0)
//             ||(cycle % 2 == 0 && x != 0 && y == 0) ||(cycle % 2 == 0 && x == 0 && y == 0))
// 	{
		
		
// 	 		if (xoffset < 0) 
// 			{
//       				input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
//       				input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
//     			} 	
// 			else if (xoffset > 0) 
// 			{
//       				input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
//       				input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
//       			}
		
// 	}
//  }		
//  else 
// {
// if (yoffset < 0) 
// 	{
//      		input_module_.add_routing(s_ph, s_vc, VC_type(3, 0));
//      		input_module_.add_routing(s_ph, s_vc, VC_type(3, 1));
//    	} 
// 	else if (yoffset > 0) 
// 	{
//      		input_module_.add_routing(s_ph, s_vc, VC_type(4, 0));
//      		input_module_.add_routing(s_ph, s_vc, VC_type(4, 1));
//   	} 
// 	else if (xoffset < 0) 
// 		{
//        		input_module_.add_routing(s_ph, s_vc, VC_type(1, 0));
//        		input_module_.add_routing(s_ph, s_vc, VC_type(1, 1));
//      		} 
// 	        else if (xoffset > 0)
// 		{
//       		input_module_.add_routing(s_ph, s_vc, VC_type(2, 0));
//        		input_module_.add_routing(s_ph, s_vc, VC_type(2, 1));
//      		} 
//                 else {
//        for (int iter = 0; iter < vc_number_; iter++) 
// 	{
// 	 input_module_.add_routing(s_ph, s_vc, VC_type(0, iter));
//         }
    
//  } 
//  }
// }

//***************************************************************************//
//only two-dimension is supported
void sim_router_template::routing_decision()
{
	time_type event_time = mess_queue::m_pointer().current_time();
        /*
	//for injection physical port 0
	for(long j = 0; j < vc_number_; j++) {
		flit_template flit_t;
		//for the HEADER_ flit
		if(input_module_.state(0,j) == ROUTING_) {
			//get the flit, which need to be routed
			flit_t = input_module_.get_flit(0,j);
			add_type des_t = flit_t.des_addr();
			add_type sor_t = flit_t.sor_addr();
			//the destination
			if(address_ == des_t) {
				//accept this flit
				accept_flit(event_time, flit_t);
				input_module_.remove_flit(0, j);
				input_module_.state_update(0, j, HOME_);
			//not destination
			}else {
				input_module_.clear_routing(0,j);
				input_module_.clear_crouting(0,j);
				(this->*curr_algorithm)(des_t, sor_t, 0, j);
				input_module_.state_update(0, j, VC_AB_);
			}
		//the BODY_ or TAIL_ flits
		}else if(input_module_.state(0,j) == HOME_)  {
			//may be empty, since other flits may be blocked in
			//previous routers
			if(input_module_.input(0, j).size() > 0) {
				flit_t = input_module_.get_flit(0, j);
				Sassert(flit_t.type() != HEADER_);
				accept_flit(event_time, flit_t);
				input_module_.remove_flit(0, j);
				if(flit_t.type() == TAIL_) {
					if(input_module_.input(0, j).size() > 0) {
						input_module_.state_update(0, j, ROUTING_);
					}else {
						input_module_.state_update(0, j, INIT_);
					}
				}
			}
		}
	}
*/
	//for other physical ports
	for(long i = 0; i < physic_ports_; i++) {
		for(long j = 0; j < vc_number_; j++) {
			flit_template flit_t;
			//send back CREDIT message
			if(input_module_.input(i,j).size() > 0) {
      // Vassos: keep some statistics about buffer utilization here
          long mm_util_now = input_module_.input(i,j).size();
          sim_foundation::wsf().in_buf_util_add(mm_util_now);
      //---------------------------------------------------                                                
				flit_t = input_module_.get_flit(i,j);
				add_type des_t = flit_t.des_addr();
				/*
				if(address_ == des_t) {
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

			*/
			}
			//for HEADER_ flit
			if(input_module_.state(i, j) == ROUTING_) {
				flit_t = input_module_.get_flit(i, j);
				//cout<<address_<<"ROUTING"<<flit_t<<endl;
				Sassert(flit_t.type() == HEADER_);
				add_type des_t = flit_t.des_addr();
				add_type sor_t = flit_t.sor_addr();
				//reach destination
				/*
				if(address_ == des_t) {
					//accept this flit
					accept_flit(event_time, flit_t);
					input_module_.remove_flit(i, j);
					input_module_.state_update(i, j, HOME_);
				//not destination, so choose one route.
				}else {*/
					input_module_.clear_routing(i, j);
					input_module_.clear_crouting(i, j);
					(this->*curr_algorithm)(des_t, sor_t, i, j);
					input_module_.state_update(i, j, VC_AB_);
			//}
			//for BODY_ or TAIL_ flits
			}
			/*else if(input_module_.state(i, j) == HOME_) {
				if(input_module_.input(i, j).size() > 0) {
			flit_t = input_module_.get_flit(i, j);
					Sassert(flit_t.type() != HEADER_);
					accept_flit(event_time, flit_t);
					input_module_.remove_flit(i, j);
					if(flit_t.type() == TAIL_) {
						//flits from other packets may stay in this  <p, v>
						if(input_module_.input(i, j).size() > 0) {
							input_module_.state_update(i, j, ROUTING_);
						//no other flits, so reinit it
						}else {
							input_module_.state_update(i, j, INIT_);
						}
					}
				}
			}*/
		}
	}
}

//***************************************************************************//
