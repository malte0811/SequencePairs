#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <cstdint>
#include <functional>
#include <vector>
#include <optional>
#include "FixedMaxVector.h"

// TODO rewrite to make inner loop 100% allocation-free
class Digraph {
public:
    using NodeId = std::size_t;
    using Cost = std::uint32_t;
    using CostVector = std::vector<Cost>;
    using CostVectorRef = std::reference_wrapper<CostVector const>;

    Digraph(NodeId num_nodes);

    void add_edge(NodeId from, NodeId to, Cost edge_cost);

    void reset();

    std::optional<CostVectorRef> compute_longest_paths() const;
private:
    using EdgeId = std::uint32_t;
    struct HalfEdge {
        NodeId other_end;
        Cost edge_cost;
    };
    struct Node {
        FixedMaxVector<HalfEdge> incoming_edges;
        FixedMaxVector<HalfEdge> outgoing_edges;
    };
    struct PreallocatedStructures {
        CostVector longest_paths;
        std::vector<EdgeId> num_unprocessed_predecessors;
        FixedMaxVector<NodeId> all_predecessors_done;
    };

    std::vector<Node> _nodes;
    PreallocatedStructures mutable _prealloc;
};

#endif
