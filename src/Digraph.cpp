#include "Digraph.h"

Digraph::Digraph(NodeId num_nodes): _nodes(num_nodes) {}

void Digraph::add_edge(NodeId const start, NodeId const end, Cost const cost) {
    _nodes.at(start).outgoing_edges.push_back({end, cost});
    _nodes.at(end).incoming_edges.push_back({start, cost});
}

auto Digraph::compute_longest_paths() const -> std::optional<std::vector<Cost>> {
    std::vector<NodeId> all_predecessors_done;
    std::vector<EdgeId> num_unprocessed_predecessors(_nodes.size(), 0);
    for (NodeId i = 0; i < _nodes.size(); ++i) {
        if (_nodes.at(i).incoming_edges.empty()) {
            all_predecessors_done.push_back(i);
        }
        num_unprocessed_predecessors.at(i) = _nodes.at(i).incoming_edges.size();
    }
    std::vector<Cost> longest_paths(_nodes.size(), 0);
    NodeId num_processed_nodes = 0;
    while (not all_predecessors_done.empty()) {
        auto const next_node = all_predecessors_done.back();
        all_predecessors_done.pop_back();
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
