/*
 * =====================================================================================
 *
 *       Filename:  fat_tree.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/05/2010 11:19:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sharda Murthi, sharda.murthi@gatech.edu
 *        Company:  Georgia Institute of Technology
 * 	 
 *	Following notaation is used:
 *	link_a represents upward links
 *	link_b represents downward links
 *
 * ================================================================================================== */

#ifndef  _fattree_cc_INC
#define  _fattree_cc_INC

#include	"fat_tree.h"

Fat_Tree :: Fat_Tree(uint rad, uint lev)
{
	radix = rad;
	level = lev;
}

Fat_Tree :: ~Fat_Tree()
{
    for ( uint i=0 ; i<no_nodes; i++ )
    {
        delete processors[i];
        delete interfaces[i];
        //delete routers[i];
    }

   for ( uint i=0 ; i<switches; i++ )
   {
	delete routers[i];
   }

    for ( uint i=0 ; i<links; i++ )
    {
        delete link_a[i];
        delete link_b[i];
    }
}

void Fat_Tree :: init (uint p, uint v, uint c, uint bs, uint n, uint k, uint l, uint sw)
{
    ports = p;
    vcs = v;
    credits  = c;
    buffer_size = bs;
    no_nodes = n; 
    grid_size = k;
    links = l;
    switches = sw;
}


string Fat_Tree :: print_stats()
{
    stringstream str;
    for ( uint i=0 ; i<no_nodes ; i++ )
    {
        //str << routers[i]->print_stats()<< endl;
        str << interfaces[i]->print_stats()<< endl;
        str << processors[i]->print_stats()<< endl;
    }
    for ( uint i = 0; i<switches; i++ )
    	str << routers[i]->print_stats()<< endl;

    return str.str();
}


void Fat_Tree :: connect_interface_processor()
{
    /* Connect the interfaces and the processors. And set the links for the
     * interfaces */
    for ( uint i=0 ; i<no_nodes; i++ )
    {
        processors[i]->interface_connections.push_back(interfaces[i]);
        interfaces[i]->processor_connection = static_cast<NetworkComponent*>(processors[i]);
        interfaces[i]->input_connection = link_b[i];
        interfaces[i]->output_connection = link_a[i];
    }

    return;
}

void Fat_Tree :: connect_interface_routers()
{
    //Input and output link connections
    for ( uint i=0 ; i<no_nodes ; i++ )
    {
        link_a[i]->input_connection = interfaces[i];
        link_a[i]->output_connection = routers[i/2];
        link_b[i]->input_connection = routers[i/2];
        link_b[i]->output_connection = interfaces[i];
        routers[i]->input_connections.push_back(link_a[i]);
        routers[i]->output_connections.push_back(link_b[i]);
    }
    return;
}

void Fat_Tree :: connect_routers()
{
        uint rows, cols, i = 0, j = 1, n = 0 ;
	uint k = radix/2;
        cols = (int)pow(k,level-1);
#ifdef DEBUG
	cout << "cols = " << cols << "   k = " << k << endl;
#endif
	uint last_link_id = no_nodes ; 
        /* Has to be done layer-wise since fat tree is not square topology like mesh & torus */

        for ( i = 0 ; i < switches ; i++ ) 
        {
		for ( n = 1; n < level; n++)
		{
			if ( (i >= (n-1)*cols) && (i < n*cols))
			{
				if ( (i % (int)pow(2,n)) < (int)pow(2, n-1))
				{
#ifdef DEBUG
                                        cout << " i = " << i << "  dest = " << i+cols << endl;
#endif
                                        link_a[last_link_id]->input_connection = routers[i];
                                        link_a[last_link_id]->output_connection = routers[i+cols];
                                        link_b[last_link_id]->input_connection = routers[i+cols];
                                        link_b[last_link_id]->output_connection = routers[i];
                                        north_links.insert(make_pair(i, i+cols));
                                        last_link_id++;

#ifdef DEBUG
					cout << " i = " << i << "  dest = " << i+cols+(int)pow(2,(i/(int)cols)) << endl;
#endif
		                        link_a[last_link_id]->input_connection = routers[i];
                		        link_a[last_link_id]->output_connection = routers[i+cols+(int)pow(2,(i/(int)cols))];
		                        link_b[last_link_id]->input_connection = routers[i+cols+(int)pow(2,(i/(int)cols))];
		                        link_b[last_link_id]->output_connection = routers[i];
		                        south_links.insert(make_pair(i, i+cols+(int)pow(2,(i/(int)cols))));
		                        last_link_id++;
				}
				else
				{
#ifdef DEBUG
                                        cout << " i = " << i << "  dest = " << i+cols << endl;
#endif
                                        link_a[last_link_id]->input_connection = routers[i];
                                        link_a[last_link_id]->output_connection = routers[i+cols];
                                        link_b[last_link_id]->input_connection = routers[i+cols];
                                        link_b[last_link_id]->output_connection = routers[i];
                                        north_links.insert(make_pair(i, i+cols));
                                        last_link_id++;

#ifdef DEBUG
					cout << " i = " << i << "  dest = " << i+cols-(int)pow(2,(i/(int)cols)) << endl;
#endif
	                	        link_a[last_link_id]->input_connection = routers[i];
        		                link_a[last_link_id]->output_connection = routers[i+cols-(int)pow(2,(i/(int)cols))];
	                	        link_b[last_link_id]->input_connection = routers[i+cols-(int)pow(2,(i/(int)cols))];
        	                	link_b[last_link_id]->output_connection = routers[i];
	                	        south_links.insert(make_pair(i,i+cols-(int)pow(2,(i/(int)cols)) ));
        		                last_link_id++;
				}
				break;
			}
		}
	}
	for ( i = switches-cols; i<switches; i++ )
	{
	        link_a[last_link_id]->input_connection = routers[i];
                link_a[last_link_id]->output_connection = routers[i];
                link_b[last_link_id]->input_connection = routers[i];
                link_b[last_link_id]->output_connection = routers[i];
                last_link_id++;
	}
#ifdef DEBUG
	cout << "******************* map north *******************" << endl;
	map<uint , uint >::iterator it1;
	map<uint , uint >::iterator it2;
	it1 = north_links.begin();
	it2 = south_links.begin();
	for (; it1 != north_links.end(); it1++ )
	{
		cout << " value 1 = " << (*it1).first << "  value 2 = " << (*it1).second << endl;
	}
	cout << "************** map south ********************" << endl;
	for (; it2 != south_links.end(); it2++ )
	{
		cout << " value 1 = " << (*it2).first << "  value 2 = " << (*it2).second << endl;
	}
#endif
}

void Fat_Tree :: setup()
{
    /* Call setup on all components */
    for ( uint i=0 ; i<no_nodes ; i++ )
    {
        processors[i]->setup( no_nodes, vcs, max_sim_time);
        interfaces[i]->setup(vcs, credits, buffer_size);
        //routers[i]->init(ports, vcs, credits, buffer_size);
    }

    for ( uint i=0 ; i<switches ; i++ )
    {
	routers[i]->init(ports, vcs, credits, buffer_size);
    }

    for ( uint i=0 ; i<links; i++ )
    {
        link_a[i]->setup();
        link_b[i]->setup();
    }
    return;

}


#endif


