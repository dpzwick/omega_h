#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>

#include "internal.hpp"

namespace Omega_h {

Graph add_edges(Graph g1, Graph g2);
Graph unmap_graph(LOs a2b, Graph b2c);
Adj unmap_adjacency(LOs a2b, Adj b2c);
template <typename T>
Read<T> graph_reduce(Graph a2b, Read<T> b_data, Int width, Omega_h_Op op);
Reals graph_weighted_average_arc_data(
    Graph a2b, Reals ab_weights, Reals ab_data, Int width);
Reals graph_weighted_average(
    Graph a2b, Reals ab_weights, Reals b_data, Int width);
Graph filter_graph(Graph g, Read<I8> keep_edge);
std::map<Int, Graph> categorize_graph(Graph g, Read<I32> b_categories);
bool operator==(Graph a, Graph b);

template <typename T>
void map_into(Read<T> a_data, Graph a2b, Write<T> b_data, Int width);
template <typename T>
Read<T> map_onto(Read<T> a_data, Graph a2b, LO nb, T init_val, Int width);

#define INST_DECL(T)                                                           \
  extern template Read<T> graph_reduce(Graph, Read<T>, Int, Omega_h_Op);       \
  extern template void map_into(                                               \
      Read<T> a_data, Graph a2b, Write<T> b_data, Int width);                  \
  extern template Read<T> map_onto(                                            \
      Read<T> a_data, Graph a2b, LO nb, T, Int width);
INST_DECL(I8)
INST_DECL(I32)
INST_DECL(I64)
INST_DECL(Real)
#undef INST_DECL

}  // end namespace Omega_h

#endif