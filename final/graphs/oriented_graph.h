//
// Created by ortur on 23.03.2022.
//
#ifndef GRAPHS_ORIENTED_GRAPH_H
#define GRAPHS_ORIENTED_GRAPH_H

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <sstream>

template<typename ValueType>
class Vertex {
public:
    Vertex(const ValueType& value) : value_(value){}

    const ValueType& GetValue() const {
        return value_;
    }

    std::string ToString() const {
        std::stringstream out;
        out << "[ " << value_ << " ]";
        return out.str();
    }

    ValueType& GetValue() {
        return value_;
    }

private:
    ValueType value_;
};

template<typename ValueType>
using VertexHolder = std::unique_ptr<Vertex<ValueType>>;



namespace std {
    template<typename ValueType>
    struct hash<Vertex<ValueType>> {
        size_t operator()(const Vertex<ValueType>& vertex) const {
            return std::hash<ValueType>{}(vertex.GetValue());
        }
    };

    template<typename ValueType>
    struct hash<VertexHolder<ValueType>> {
        size_t operator()(const VertexHolder<ValueType>& vertex) const {
            return std::hash<ValueType>{}(vertex->GetValue());
        }
    };


}

template<typename ValueType>
struct VertexHasher {

    size_t operator()(const Vertex<ValueType>& vertex) const {
        return std::hash<ValueType>{}(vertex.GetValue());
    }

};

template<typename ValueType>
VertexHolder<ValueType> MakeVertex(const ValueType& val) {
    return std::make_unique<Vertex<ValueType>>(val);
}

template<typename ValueType>
bool operator==(const Vertex<ValueType>& lhs, const Vertex<ValueType>& rhs) {
    return lhs.GetValue() == rhs.GetValue();
}

template<typename ValueType>
bool operator==(const VertexHolder<ValueType>& lhs, const VertexHolder<ValueType>& rhs) {
    return lhs->GetValue() == rhs->GetValue();
}




template<typename ValueType, typename WeightType>
class OrientedGraph {
public:
public:
    using VertexId = size_t;
    using EdgeId = size_t;


    struct Edge {
        const VertexId from;
        const VertexId to;
        WeightType weight;

        Edge(VertexId from, VertexId to, WeightType weight = 0)
            : from(from), to(to), weight(weight) {}

    };

    using EdgeHolder = std::unique_ptr<Edge>;

    EdgeHolder MakeEdge(VertexId from, VertexId to, WeightType weight) const {
        return std::make_unique<Edge>(from, to, weight);
    }
public:

    OrientedGraph() {
        vertexes_.reserve(1000);
        edges_.reserve(1000);
    }

    VertexId AddVertex(VertexHolder<ValueType> vertex) {
        vertexes_.push_back(std::move(vertex));
        vertex_connections_[vertexes_.size() - 1];
        return vertexes_.size() - 1;         // выдача id
    }

    VertexHolder<ValueType>& GetVertex(VertexId id) {
        return vertexes_[id];
    }

    const VertexHolder<ValueType>& GetVertex(VertexId id) const {
        return vertexes_.at(id);
    }

    EdgeHolder& GetEdge(EdgeId id) {
        return edges_[id];
    }


    const EdgeHolder& GetEdge(VertexId from, VertexId to) const {
        for (auto& edge_id : vertex_connections_.at(from)) {
            const auto& edge = GetEdge(edge_id);
            if (edge->to == to) {
                return edge;
            }
        }
        return (nullptr);
    }

    EdgeHolder& GetEdge(VertexId from, VertexId to) {
        for (auto& edge_id : vertex_connections_.at(from)) {
            const auto& edge = GetEdge(edge_id);
            if (edge->to == to) {
                return edge;
            }
        }
        return nullptr;
    }

    std::optional<EdgeId> GetEdgeId(VertexId from, VertexId to) const {
        if (!HasEdge(from, to)) {
            return std::nullopt;
        }

        for (size_t i = 0; i < edges_.size(); i++) {
            const auto& edge = edges_.at(i);
            if (edge->from == from && edge->to == to) {
                return std::make_optional(i);
            }
        }

        return std::nullopt;
    }



    const EdgeHolder& GetEdge(EdgeId id) const {
        if (HasEdge(id)) {
            return edges_[id];
        } else {
            return nullptr;
        }

    }


    EdgeId Bind(VertexId from, VertexId to, WeightType weight = 0) {
        edges_.push_back(std::move(MakeEdge(from, to, weight)));
        EdgeId id = edges_.size() - 1;
        vertex_connections_[from].insert(id);

        return id;
    }

    void SetWeight(EdgeId edge_id, WeightType weight) {
        auto& edge = GetEdge(edge_id);
        edge->weight = weight;
    }

    bool HasVertex(VertexId id) const {
        return id < vertexes_.size();
    }

    bool HasEdge(VertexId from, VertexId to) const {
        if (HasVertex(from) && HasVertex(to)) {
            auto connections = vertex_connections_.at(from);
            for (auto edge_id : connections) {
                const auto& edge =  GetEdge(edge_id);
                if (edge->to == to) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HasEdge(EdgeId id) const {
        return id < edges_.size();
    }

    const std::set<EdgeId>& GetVertexConnections(VertexId id) const {
        return vertex_connections_.at(id);
    }

private:

public:
    std::unordered_map<VertexId, std::set<EdgeId>> vertex_connections_;

    std::vector<VertexHolder<ValueType>> vertexes_;
    std::vector<EdgeHolder> edges_;


};
#endif //GRAPHS_ORIENTED_GRAPH_H
