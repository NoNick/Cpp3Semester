#include "graph.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stack>

template <typename NodePayload>
Graph<NodePayload>::Graph() {}

template <typename NodePayload>
Graph<NodePayload>::~Graph() {}

template <typename NodePayload>
void Graph<NodePayload>::clear() {
    vertex.clear();
    payloads.clear();
}

template<typename NodePayload> template <class Archive>
void Graph<NodePayload>::serialize(Archive & ar, const unsigned int version) {
    ar & vertex;
    ar & payloads;
}

template <typename NodePayload>
void Graph<NodePayload>::saveToFile(std::string const & filename) {
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
}

template <typename NodePayload>
void Graph<NodePayload>::loadFromFile(std::string const & filename) {
    Graph g;
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);
    ia >> g;
    vertex = g.vertex;
    payloads = g.payloads;
}

template <typename NodePayload>
// Isn't NodeHandle invariant with template parameter?
// Do I have to set one to get typedef?
// Maybe NodeHandle and EdgeHandle should be out of the class in graph.h
Graph<int>::NodeHandle Graph<NodePayload>::addNode(NodePayload x) {
    vertex.push_back(std::vector<NodeHandle>());
    payloads.push_back(x);
    return vertex.size() - 1;
}


template <typename NodePayload>
void Graph<NodePayload>::addEdge(NodeHandle const & a, NodeHandle const & b) {
    // there's no point for creating loops with one vertex in unweighted graph
    if (a != b) {
        if (!std::count(vertex[a].begin(), vertex[a].end(), b)) {
            vertex[a].push_back(b);
        }
        if (!std::count(vertex[b].begin(), vertex[b].end(), a)) {
            vertex[b].push_back(a);
        }
    }
}

template <typename NodePayload>
void Graph<NodePayload>::forEachNode(std::function<void(NodeHandle const &)> const & visitor) const {
    for (size_t i = 0; i < vertex.size(); i++) {
        visitor((NodeHandle) i);
    }
}

template <typename NodePayload>
void Graph<NodePayload>::forEachEdge(NodeHandle const & source, EdgeVisitor const & visitor) {
    if (source >= vertex.size() || source < 0) {
        throw new std::exception();
    }

    for (auto it = vertex[source].begin(); it != vertex[source].end(); it++) {
        visitor((EdgeHandle) std::make_pair(source, *it));
    }
}

template <typename NodePayload>
size_t Graph<NodePayload>::getNodesCount() const {
    return vertex.size();
}

template <typename NodePayload>
NodePayload & Graph<NodePayload>::operator[](NodeHandle const & node) {
    if (node >= payloads.size() || node < 0) {
        throw new std::exception();
    }
    return payloads[node];
}

template <typename NodePayload>
void Graph<NodePayload>::dfs(NodeVisitor const & startNode, NodeVisitor const & endNode, NodeVisitor const & discoverNode) {
    bool* used = new bool[vertex.size()];
    std::fill_n(used, vertex.size(), false);

    NodeHandle curr;
    for (NodeHandle i = 0; i < vertex.size(); i++) {
        if (!used[i]) {
            std::stack<NodeHandle> st;
            st.push(i);
            while (!st.empty()) {
                curr = st.top();
                st.pop();
                startNode(curr);

                for (auto it = vertex[curr].begin(); it != vertex[curr].end(); it++) {
                    discoverNode(*it);
                    if (!used[*it]) {
                        used[*it] = true;
                        st.push(*it);
                    }
                }

                endNode(curr);
            }
        }
    }
    delete[] used;
}

template <typename NodePayload>
Graph<int>::NodeHandle Graph<NodePayload>::move(const NodeHandle &origin, const EdgeHandle &edge) {
    return origin == edge.first ? edge.second : edge.first;
}
