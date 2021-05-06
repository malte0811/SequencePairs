#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <cstdint>
#include <vector>
#include <optional>

// TODO rewrite to make inner loop 100% allocation-free
class Digraph {
public:
    using NodeId = std::size_t;
    using Cost = std::uint32_t;

    Digraph(NodeId num_nodes);
    void add_edge(NodeId from, NodeId to, Cost edge_cost);

    std::optional<std::vector<Cost>> compute_longest_paths() const;
private:
    using EdgeId = std::uint32_t;
    struct HalfEdge {
        NodeId other_end;
        Cost edge_cost;
    };
    struct Node {
        std::vector<HalfEdge> incoming_edges;
        std::vector<HalfEdge> outgoing_edges;
    };

    std::vector<Node> _nodes;
};

#endif
