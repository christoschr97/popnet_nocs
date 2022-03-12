#ifndef NETWORK_MESS_QUEUE_H_
#define NETWORK_MESS_QUEUE_H_

#include "SStd.h"
#include "index.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_event.h"
#include <exception>
#include <set>
#include <cmath>
#include <utility>
#include <iostream>
#include <string>


bool operator<(const mess_event & a, const mess_event & b);


class mess_queue {
	friend ostream & operator<<(ostream& os, const mess_queue & sgq);
	private:
		typedef multiset <mess_event> message_q;
		time_type current_time_; //the time of the top message
		time_type last_time_; //the time of the previous message
		long mess_counter_;
		message_q m_q_;
		long total_finished_;
		static mess_queue * m_pointer_;
		static string mess_error_;

	public:
// Vassos: comment this class out as it causes problems with the compilation
//		class pro_error: public exception {
//			public:
//				pro_error(const string & err) : what_(err) {}
//				virtual const char * what() const {return what_.c_str();}
//
//			private:
//				string what_;
//		}; 
		
		typedef message_q::size_type size_type; 
		typedef message_q::iterator iterator;
		static const mess_queue & m_pointer() {return * m_pointer_;}
		static mess_queue & wm_pointer() {return * m_pointer_;}
// Vassos: comment thie following line out, error from the compiler
		//mess_queue(time_type start_time) throw(pro_error &);
		mess_queue(time_type start_time);
		~mess_queue(){m_pointer_ = 0;}

		//return current simulation time stamp
		time_type current_time() const {return current_time_;}

		//return last simulation time stamp
		time_type last_time() const {return last_time_;}

		long mess_counter() const {return mess_counter_;}

		//events simulator main function
		void simulator();

		long total_finished() {return total_finished_;}
		long total_finished() const {return total_finished_;}
		void TotFin_inc() {total_finished_++;}

		//retrive a message from the message queue
		iterator get_message() {return m_q_.begin();}
		//delete the indexed message from message queue
		void remove_message(iterator pos) {m_q_.erase(pos);}
		//delete top message from message queue
		void remove_top_message() {m_q_.erase(m_q_.begin());}
		//retrive message queue size
		size_type message_queue_size() const {return m_q_.size();}
		//add a new message into the message queue
		void add_message(const mess_event & x) 
					{mess_counter_++; m_q_.insert(x);}
};
#endif
