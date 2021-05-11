#include "Digraph.h"
#include <functional>

Digraph::Digraph(NodeId num_nodes): 
    _nodes(num_nodes),
    _longest_paths(CostVector(num_nodes))
{}

auto Digraph::compute_longest_paths( Cost const stop_if_strictly_longer) -> std::optional<CostVectorRef> {
    // Distance to node 0 is always 0, so start computing at 1
    for (NodeId i = 1; i < _nodes.size(); ++i) {
        auto const& node = _nodes.at(i);
        Cost distance = 0;
        for (auto const& start : node.incoming_edges) {
            auto const cost_via_start = _longest_paths.at(start) + _nodes.at(start).outgoing_edge_cost;
            if (distance < cost_via_start) {
                distance = cost_via_start;
            }
        }
        if (distance + node.outgoing_edge_cost > stop_if_strictly_longer) {
            return std::nullopt;
        }
        _longest_paths.at(i) = distance;
    }
    return _longest_paths;
}
