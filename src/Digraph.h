#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <cstdint>
#include <functional>
#include <vector>
#include <optional>

class Digraph {
public:
    using NodeId = std::size_t;
    using Cost = std::uint32_t;
    using CostVector = std::vector<Cost>;
    using CostVectorRef = std::reference_wrapper<CostVector const>;

    Digraph(NodeId num_nodes);

    void add_edge(NodeId from, NodeId to);

    void set_outgoing_edge_cost(NodeId from, Cost cost);

    std::optional<CostVectorRef> compute_longest_paths(Cost stop_if_strictly_longer) const;
private:
    using EdgeId = std::uint32_t;
    struct Node {
        std::size_t num_incoming_edges;
        Cost outgoing_edge_cost;
        std::vector<NodeId> outgoing_edges;
    };
    struct PreallocatedStructures {
        CostVector longest_paths;
    };

    std::vector<Node> _nodes;
    PreallocatedStructures mutable _prealloc;
};

#endif
