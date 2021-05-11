#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <cstdint>
#include <functional>
#include <vector>
#include <optional>
#include <cassert>

class Digraph {
public:
    using NodeId = std::size_t;
    using Cost = std::uint32_t;
    using CostVector = std::vector<Cost>;
    using CostVectorRef = std::reference_wrapper<CostVector const>;

    Digraph(NodeId num_nodes);

    void add_edge(NodeId from, NodeId to);

    void set_outgoing_edge_cost(NodeId from, Cost cost);

    std::optional<CostVectorRef> compute_longest_paths(Cost stop_if_strictly_longer);
private:
    using EdgeId = std::uint32_t;
    struct Node {
        Cost outgoing_edge_cost;
        std::vector<NodeId> incoming_edges;
    };

    std::vector<Node> _nodes;
    CostVector _longest_paths;
};

// INLINE SECTION

inline void Digraph::add_edge(NodeId const start, NodeId const end) {
    assert(start < end);
    _nodes.at(end).incoming_edges.push_back(start);
}

inline void Digraph::set_outgoing_edge_cost(NodeId const start, Cost const cost) {
    _nodes[start].outgoing_edge_cost = cost;
}

#endif
