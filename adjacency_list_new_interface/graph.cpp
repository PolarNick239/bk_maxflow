/* graph.cpp */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdexcept>
#include "graph.h"

template <typename captype, typename tcaptype, typename flowtype>
	Graph<captype, tcaptype, flowtype>::Graph(int _node_num_max, int edge_num_max, void (*err_function)(const char *))
	: node_num(0), node_num_max(_node_num_max)
{
	error_function = err_function;
	nodes = (node*) malloc(node_num_max*sizeof(node));
	if (!nodes)
		throw std::bad_alloc();
	flow = 0;
}

template <typename captype, typename tcaptype, typename flowtype>
	Graph<captype, tcaptype, flowtype>::~Graph()
{
	free(nodes);
}

template <typename captype, typename tcaptype, typename flowtype>
	typename Graph<captype, tcaptype, flowtype>::node_id Graph<captype, tcaptype, flowtype>::add_node(int num)
{
	node_id i = node_num;
	node_num += num;

	if (node_num > node_num_max)
	{
		printf("Error: the number of nodes is exceeded!\n");
		exit(1);
	}
	memset(nodes + i, 0, num*sizeof(node_st));

	return i;
}

template <typename captype, typename tcaptype, typename flowtype>
	void Graph<captype, tcaptype, flowtype>::add_edge(node_id _from, node_id _to, captype cap, captype rev_cap)
{
	arc *a, *a_rev;
	node* from = nodes + _from;
	node* to = nodes + _to;

	assert(from->narcs < MAX_ARCS_NUMBER);
	assert(to->narcs < MAX_ARCS_NUMBER);

	a = from->arcs + from->narcs;
	a_rev = to->arcs + to->narcs;
	++from->narcs;
	++to->narcs;

	a -> head = NODE_ID(to);
	a_rev -> head = NODE_ID(from);
	a -> r_cap = cap;
	a_rev -> r_cap = rev_cap;
}

template <typename captype, typename tcaptype, typename flowtype>
	void Graph<captype, tcaptype, flowtype>::set_tweights(node_id _i, tcaptype cap_source, tcaptype cap_sink)
{
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	nodes[_i] . tr_cap = cap_source - cap_sink;
}

template <typename captype, typename tcaptype, typename flowtype>
	void Graph<captype, tcaptype, flowtype>::add_tweights(node_id _i, captype cap_source, captype cap_sink)
{
	register captype delta = nodes[_i] . tr_cap;
	if (delta > 0) cap_source += delta;
	else           cap_sink   -= delta;
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	nodes[_i] . tr_cap = cap_source - cap_sink;
}

#include "instances.inc"
