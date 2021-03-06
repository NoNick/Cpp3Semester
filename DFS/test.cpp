#include "gtest/gtest.h"
#include "graph.cpp"

#include <algorithm>
#include <array>

namespace {
    const size_t NCreate = 1000, NCheck = 100;
    size_t N;

    std::vector <int> other;
    Graph<int>::NodeHandle curr;
    Graph<int> g;
    void edgeVisitor(Graph<std::string>::EdgeHandle const & x) {
        if (x.first == curr) {
            other.push_back(g[x.second]);
        }
        else {
            other.push_back(g[x.first]);
        }
    }

    bool checkEdges() {
        if (other.size() != N - 1)
            return false;

        for (size_t i = 0; i < N; i++) {
            if (curr != (Graph<std::string>::NodeHandle) i &&
                !std::count(other.begin(), other.end(), i))
                return false;
        }

        return true;
    }

    void fullVisitor(Graph<std::string>::NodeHandle const & x) {
        other.clear();
        curr = x;
        g.forEachEdge(x, edgeVisitor);
        EXPECT_TRUE(checkEdges());
    }

    void createFullNGraph(int n) {
        g = Graph<int>();
        N = n;

        for (size_t i = 0; i < N; i++) {
            g[g.addNode()] = i;
        }
        for (size_t i = 0; i < N; i++) {
            for (size_t j = i + 1; j < N; j++) {
                g.addEdge(i, j);
            }
        }
    }
}

TEST(correctness, CreateFullGraph) {
    createFullNGraph(NCreate);

    EXPECT_EQ(N, g.getNodesCount());
}

TEST(correctness, CreateAndTestFullGraph) {
    createFullNGraph(NCheck);

    EXPECT_EQ(N, g.getNodesCount());

    g.forEachNode(fullVisitor);
}

TEST(correctness, ChangePayload) {
    Graph<int> g1;
    Graph<int>::NodeHandle h = g1.addNode();
    g1[h] = 10;
    EXPECT_EQ(g1[h], 10);
}

TEST(correctness, SaveAndLoad) {
    createFullNGraph(NCheck);

    g.saveToFile("test.graph");
    g = Graph<int>();
    g.loadFromFile("test.graph");

    EXPECT_EQ(N, g.getNodesCount());
    g.forEachNode(fullVisitor);
}

TEST(correctness, InvalidOperations) {
    EXPECT_ANY_THROW(g[N]);
    EXPECT_ANY_THROW(g.forEachEdge(N, edgeVisitor));
    EXPECT_ANY_THROW(g[N + 1]);
    EXPECT_ANY_THROW(g.forEachEdge(N + 1, edgeVisitor));
    EXPECT_ANY_THROW(g[-1]);
    EXPECT_ANY_THROW(g.forEachEdge(-1, edgeVisitor));
}

namespace {
    int currColor;
    int* colors;
    bool bipartite;

    void createSingleVertexGraph() {
        g = Graph<int>();
        g.addNode();
    }

    void createLineGraph() {
        g = Graph<int>();
        Graph<int>::NodeHandle last, curr;
        last = g.addNode();
        for (size_t i = 1; i < NCreate; i++) {
            curr = g.addNode();
            g.addEdge(curr, last);
            last = curr;
        }
    }

    void createEvenCycle() {
        g = Graph<int>();
        Graph<std::string>::NodeHandle last, curr, first;
        last = first = g.addNode();
        for (size_t i = 1; i < (NCreate / 2) * 2; i++) {
            curr = g.addNode();
            g.addEdge(curr, last);
            last = curr;
        }
        g.addEdge(last, first);
    }

    void createOddCycle() {
        g = Graph<int>();
        Graph<std::string>::NodeHandle last, curr, first;
        last = first = g.addNode();
        for (size_t i = 1; i < (NCreate / 2) * 2 + 1; i++) {
            curr = g.addNode();
            g.addEdge(curr, last);
            last = curr;
        }
        g.addEdge(last, first);
    }

    typedef void (*createGraph) (void);
    createGraph create[] = {
        createSingleVertexGraph,
        createLineGraph,
        createEvenCycle,
        createOddCycle
    };
    bool answers[] = {true, true, true, false};

    void bipartiteStart(Graph<std::string>::NodeHandle const & x) {
        if (colors[x] == -1) {
            colors[x] = currColor;
        }
        else {
            currColor = colors[x];
        }
    }

    void bipartiteDiscovery(Graph<std::string>::NodeHandle const & x) {
        if (currColor == -1) {
            // discovery made from some unknown position (out of a node)
            throw new std::exception();
        }

        if (colors[x] == -1) {
            colors[x] = (currColor + 1) % 2;
        }
        else if (colors[x] == currColor) {
            bipartite = false;
        }
    }

    void bipartiteEnd(Graph<std::string>::NodeHandle const & x) {
        currColor = -1;
    }

    bool ifBipartite() {
        currColor = 0;
        colors = new int[g.getNodesCount()];
        std::fill_n(colors, g.getNodesCount(), -1);

        bipartite = true;
        g.dfs(bipartiteStart, bipartiteEnd, bipartiteDiscovery);
        delete[] colors;

        return bipartite;
    }
}

TEST(correctness, BipartiteCheck) {
    for (size_t i = 0; i < 4; i++) {
        create[i]();
        EXPECT_EQ(answers[i], ifBipartite());
    }
}

namespace {
    void moveEdgeVisitor(Graph<std::string>::EdgeHandle const & x) {
        EXPECT_EQ(x.first, g.move(x.second, x));
        EXPECT_EQ(x.second, g.move(x.first, x));
    }

    void moveNodeVisitor(Graph<std::string>::NodeHandle const & x) {
        g.forEachEdge(x, moveEdgeVisitor);
    }
}

TEST(correctness, moveCheck) {
    createFullNGraph(NCheck);
    g.forEachNode(moveNodeVisitor);
}

