#ifndef GRAPH_H
#define GRAPH_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <functional>

template<typename NodePayload>
class Graph {
public:
    typedef unsigned NodeHandle;
    typedef std::pair<NodeHandle, NodeHandle> EdgeHandle;
    typedef std::function<void(NodeHandle const &)> NodeVisitor;
    typedef std::function<void(EdgeHandle const &)> EdgeVisitor;
public:    
    Graph();
    ~Graph();
    void loadFromFile(std::string const & filename);
    void saveToFile(std::string const & filename);
    NodeHandle addNode(NodePayload x);
    void addEdge(NodeHandle const & a, NodeHandle const & b);
    void forEachNode(std::function<void(NodeHandle const &)> const & visitor) const;
    size_t getNodesCount() const;
    void forEachEdge(NodeHandle const & source, EdgeVisitor const & visitor);
    NodeHandle move(NodeHandle const & origin, EdgeHandle const & edge);
    NodePayload & operator[](NodeHandle const & node);
    void dfs(NodeVisitor const & startNode, NodeVisitor const & endNode, NodeVisitor const & discoverNode);
private:
    friend class boost::serialization::access;

    void clear();
    std::vector < std::vector <NodeHandle> > vertex;
    std::vector <NodePayload> payloads;
    template <class Archive>
    void serialize(Archive & ar, const unsigned int version);
};


#endif // GRAPH_H
