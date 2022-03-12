/* =========FOR INTERNAL USE ONLY. NO DISTRIBUTION PLEASE ========== */

/*********************************************************************
 Copyright 2000-2001, Princeton University.  All rights reserved.
 By using this software the USER indicates that he or she has read,
 understood and will comply with the following:

 --- Princeton University hereby grants USER nonexclusive permission
 to use, copy and/or modify this software for internal, noncommercial,
 research purposes only. Any distribution, including commercial sale
 or license, of this software, copies of the software, its associated
 documentation and/or modifications of either is strictly prohibited
 without the prior consent of Princeton University.  Title to copyright
 to this software and its associated documentation shall at all times
 remain with Princeton University.  Appropriate copyright notice shall
 be placed on all software copies, and a complete copy of this notice
 shall be included in all copies of the associated documentation.
 No right is  granted to use in advertising, publicity or otherwise
 any trademark,  service mark, or the name of Princeton University.


 --- This software and any associated documentation is provided "as is"

 PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS
 OR IMPLIED, INCLUDING THOSE OF MERCHANTABILITY OR FITNESS FOR A
 PARTICULAR PURPOSE, OR THAT  USE OF THE SOFTWARE, MODIFICATIONS, OR
 ASSOCIATED DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS,
 TRADEMARKS OR OTHER INTELLECTUAL PROPERTY RIGHTS OF A THIRD PARTY.

 Princeton University shall not be liable under any circumstances for
 any direct, indirect, special, incidental, or consequential damages
 with respect to any claim by USER or any third party on account of
 or arising from the use, or inability to use, this software or its
 associated documentation, even if Princeton University has been advised
 of the possibility of those damages.
*********************************************************************/
/********************************************************************/
/* 				Interconnection Network Simulator					*/
/* 				Author: Li Shang 									*/
/*				Princeton University								*/
/********************************************************************/
#include <exception>
#include <iostream>
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_queue.h"

int main(int argc, char *argv[])
{
	try {
		SRGen random_gen;
		configuration c_par(argc, argv);
		cout<<c_par;
		sim_foundation sim_net;
		mess_queue network_mess_queue(0.0);
		network_mess_queue.simulator();
		sim_net.simulation_results();
		sim_net.simulation_check();
	} catch (exception & e) {
		cerr << e.what();
	}
}
