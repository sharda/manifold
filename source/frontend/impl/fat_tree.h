/* ===========================================================================================
 * File Name: fat_tree.h
 *
 * Description: This class defines the functions for a generic binary fat tree
 * with switch radix k
 * The links have the following naming convention:
 *    links going downwards are a links
 *    links going upwards are b links
 *
 *    Router ports
 *    port 0: Connects to interface
 *    port 1: Connects to direction south	
 *    port 2: Connects to direction south
 *    port 3: Connects to direction north
 *    port 4: Connects to direction north
*
 *        Version:  1.0
 *        Created:  09/05/2010 07:15:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sharda Murthi, sharda.murthi@gatech.edu
 *        Company:  Georgia Institute of Technology
 *
 * ====================================================================================== */

#ifndef  _fattree_h_INC
#define  _fattree_h_INC

#include        "topology.h"

#include        <iostream>
#include        <fstream>
#include        "../../simIris/components/impl/genericRouter4Stg.h"
//#include        "../../simIris/components/impl/genericRouter3Stg.h"
//#include        "../../simIris/components/impl/genericInterfaceVcs.h"
#include        "../../simIris/components/impl/genericTracePktGen.h"
#include        "../../simIris/components/impl/mcFrontEnd.h"
#include        "../../simIris/components/impl/genericFlatMc.h"
#include        "../../simIris/components/impl/genericLink.h"
#include        "../../simIris/components/impl/mcFrontEnd.h"

#include        "../../simIris/data_types/impl/flit.h"
#include        "../../simIris/data_types/impl/highLevelPacket.h"
#include        "../../simIris/components/impl/genericFlatMc.h"
#include        "../../util/genericData.h"
#include        <string.h>
#include        <sys/time.h>
#include        <algorithm>
#include        <stdio.h>
#include        <stdlib.h>
#include        <time.h>
#include        <setjmp.h>
#include        <signal.h>
#include        <sys/types.h>
#include        <unistd.h>
#include        <sys/time.h>
#include        <sys/io.h>

#ifdef USE_ZESTO
#include        "../../zesto/host.h"
#include        "../../zesto/misc.h"
#include        "../../zesto/sim.h"
#include        "../../zesto/machine.h"
#include        "../../zesto/endian.h"
#include        "../../zesto/version.h"
#include        "../../zesto/options.h"
#include        "../../zesto/stats.h"
#include        "../../zesto/regs.h"
#include        "../../zesto/memory.h"
#include        "../../zesto/thread.h"
#endif

extern vector<unsigned int> mc_positions;
extern uint no_of_cores;
extern uint concentration;

class Fat_Tree : public Topology
{
    public:
        Fat_Tree (uint radix, uint level);
        ~Fat_Tree ();

        void init(uint ports, uint vcs, uint credits, uint buffer_size, uint no_nodes, uint grid_size, uint links, uint switches);
        void setup(void);
        void connect_interface_processor(void);
        void connect_interface_routers(void);
        void connect_routers(void);
        string print_stats(void);
        void set_max_phy_link_bits ( uint a);

    protected:

    private:
        uint ports;
        uint vcs;
        uint credits;
        uint buffer_size;
        uint no_nodes;
        uint links;
        uint grid_size;
	// For Fat tree only
	uint switches; // number of siwtches in the fat tree
	uint radix;
	uint level;
}; /* -----  end of class Fat_Tree  ----- */

#endif   /* ----- #ifndef _fattree_h_INC  ----- */

