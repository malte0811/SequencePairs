#include "Digraph.h"
#include <functional>

Digraph::Digraph(NodeId num_nodes): _nodes(
        num_nodes, Node{
            .incoming_edges = FixedMaxVector<HalfEdge>(num_nodes - 1),
            .outgoing_edges = FixedMaxVector<HalfEdge>(num_nodes - 1),
        }),
    _prealloc{
        .longest_paths = CostVector(num_nodes),
        .num_unprocessed_predecessors = std::vector<EdgeId>(num_nodes),
        .all_predecessors_done = FixedMaxVector<NodeId>(_nodes.size()),
    }
{}

void Digraph::add_edge(NodeId const start, NodeId const end, Cost const cost) {
    _nodes.at(start).outgoing_edges.push_back({end, cost});
    _nodes.at(end).incoming_edges.push_back({start, cost});
}

auto Digraph::compute_longest_paths() const -> std::optional<CostVectorRef> {
    auto& all_predecessors_done = _prealloc.all_predecessors_done;
    auto& num_unprocessed_predecessors = _prealloc.num_unprocessed_predecessors;
    std::fill(num_unprocessed_predecessors.begin(), num_unprocessed_predecessors.end(), 0);
    assert(all_predecessors_done.empty());
    for (NodeId i = 0; i < _nodes.size(); ++i) {
        if (_nodes.at(i).incoming_edges.empty()) {
            all_predecessors_done.push_back(i);
        }
        num_unprocessed_predecessors.at(i) = _nodes.at(i).incoming_edges.size();
    }
    auto& longest_paths = _prealloc.longest_paths;
    std::fill(longest_paths.begin(), longest_paths.end(), 0);
    NodeId num_processed_nodes = 0;
    while (not all_predecessors_done.empty()) {
        auto const next_node = all_predecessors_done.pop_back();
        ++num_processed_nodes;
        auto const& node = _nodes.at(next_node);
        auto const distance = longest_paths.at(next_node);
        for (auto const& [end, cost] : node.outgoing_edges) {
            --num_unprocessed_predecessors.at(end);
            if (num_unprocessed_predecessors.at(end) == 0) {
                all_predecessors_done.push_back(end);
            }
            auto const cost_via_current_node = distance + cost;
            auto& end_distance = longest_paths.at(end);
            if (cost_via_current_node > end_distance) {
                end_distance = cost_via_current_node;
            }
        }
    }
    if (num_processed_nodes == _nodes.size()) {
        return longest_paths;
    } else {
        return std::nullopt;
    }
}

void Digraph::reset() {
    for (auto& [incoming, outgoing] : _nodes) {
        incoming.reset();
        outgoing.reset();
    }
}
