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

private:
    struct Binding {
        VertexId from;
        VertexId to;

        bool operator==(const Binding& other) const {
            return std::tie(from, to) == std::tie(other.from, other.to);
        }
    };

    struct BindingHasher {
        size_t operator()(const Binding& binding) const {
            size_t base = 37;
            size_t A = std::hash<VertexId>{}(binding.from);
            size_t B = std::hash<VertexId>{}(binding.to);

            return base * base * A +
                   base * B;
        }
    };

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
        auto& edge_id = has_connection_.at(Binding{from, to});
        auto& edge = GetEdge(edge_id);
        return edge;
    }

    EdgeHolder& GetEdge(VertexId from, VertexId to) {
        auto& edge_id = has_connection_.at(Binding{from, to});
        auto& edge = GetEdge(edge_id);
        return edge;
    }

    std::optional<EdgeId> GetEdgeId(VertexId from, VertexId to) const {
        if (!HasEdge(from, to)) {
            return std::nullopt;
        }
        return has_connection_.at(Binding{from, to});
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
        has_connection_[Binding{from, to}] = id;
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
            return has_connection_.find(Binding{from, to}) != has_connection_.end();
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
    std::unordered_map<VertexId, std::set<EdgeId>> vertex_connections_;
    std::unordered_map<Binding, EdgeId, BindingHasher> has_connection_;


    std::vector<VertexHolder<ValueType>> vertexes_;
    std::vector<EdgeHolder> edges_;


};
#endif //GRAPHS_ORIENTED_GRAPH_H
