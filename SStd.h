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

#ifndef NETWORK_STD_H_
#define NETWORK_STD_H_

/*###########################################################################*/
#include <iostream>
using namespace std;
extern std::ostream scerrhex;
extern std::ostream scouthex;

// Use for a sane and fast vector of bools.
typedef int big_bool;

/*===========================================================================*/
// Debugging

// Macros need to be used to accomplish this.  Inlines won't work.

// Print position in code.
void spos(const char * file, int line);
#define Spos() spos(__FILE__, __LINE__)

// Print position in code and abort after printing message.
void sabort(const char * file, int line,
	const char * message = "Program aborted.\n");

void sexit(const char * message = "Exiting.\n");

#define Rabort() sabort(__FILE__, __LINE__)

// Abort, printing position in code, if (! x).
void sassert(bool x, const char * file, int line);
#define Sassert(x) sassert((x), __FILE__, __LINE__)

inline void sconfirm(bool x, const char * message) { if (! x) sexit(message); }

/*===========================================================================*/
/*###########################################################################*/
#endif
