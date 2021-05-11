#include "Digraph.h"
#include <functional>
#include <cassert>

Digraph::Digraph(NodeId num_nodes): 
    _nodes(num_nodes),
    _prealloc{
        .longest_paths = CostVector(num_nodes),
    }
{}

void Digraph::add_edge(NodeId const start, NodeId const end) {
    assert(start < end);
    _nodes.at(start).outgoing_edges.push_back(end);
    ++_nodes.at(end).num_incoming_edges;
}

void Digraph::set_outgoing_edge_cost(NodeId const start, Cost const cost) {
    _nodes.at(start).outgoing_edge_cost = cost;
}

auto Digraph::compute_longest_paths(
    Cost const stop_if_strictly_longer
) const -> std::optional<CostVectorRef> {
    auto& longest_paths = _prealloc.longest_paths;
    std::fill(longest_paths.begin(), longest_paths.end(), 0);
    for (NodeId next_node = 0; next_node < _nodes.size(); ++next_node) {
        auto const& node = _nodes.at(next_node);
        auto const distance = longest_paths.at(next_node);
        for (auto const& end : node.outgoing_edges) {
            auto const cost_via_current_node = distance + node.outgoing_edge_cost;
            auto& end_distance = longest_paths.at(end);
            if (cost_via_current_node > end_distance) {
                end_distance = cost_via_current_node;
                if (end_distance + _nodes.at(end).outgoing_edge_cost > stop_if_strictly_longer) {
                    return std::nullopt;
                }
            }
        }
    }
    return longest_paths;
}
