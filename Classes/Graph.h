#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <stack>


template<class T>
class Edge;


/************************* Vertex  **************************/

template<class T>
class Vertex {
public:
    Vertex(T in);

    bool operator<(Vertex<T> &vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;

    std::vector<Edge<T> *> getAdj() const;

    bool isVisited() const;

    bool isProcessing() const;

    unsigned int getIndegree() const;

    double getDist() const;

    Edge<T> *getPath() const;

    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);

    void setVisited(bool visited);

    void setProcesssing(bool processing);

    void setIndegree(unsigned int indegree);

    void setDist(double dist);

    void setPath(Edge<T> *path);

    Edge<T> *addEdge(Vertex<T> *dest, double w);

    bool removeEdge(T in);

    void removeOutgoingEdges();

    //friend class MutablePriorityQueue<Vertex>;
    int queueIndex = 0;
protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    // required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template<class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);

    Vertex<T> *getDest() const;

    double getWeight() const;

    bool isSelected() const;

    Vertex<T> *getOrig() const;

    Edge<T> *getReverse() const;

    double getFlow() const;

    void setSelected(bool selected);

    void setReverse(Edge<T> *reverse);

    void setFlow(double flow);

protected:
    Vertex<T> *dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template<class T>
class Graph {
public:

    ~Graph();

    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(const T &in) const;

    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);

    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double w);

    bool removeEdge(const T &source, const T &dest);

    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    int getNumVertex() const;

    std::vector<Vertex<T> *> getVertexSet() const;

    std::vector<T> dfs() const;

    std::vector<T> dfs(const T &source) const;

    void dfsVisit(Vertex<T> *v, std::vector<T> &res) const;

    std::vector<T> bfs(const T &source) const;

    bool isDAG() const;

    bool dfsIsDAG(Vertex<T> *v) const;

    std::vector<T> topsort() const;

    std::vector<Edge<T>> kruskalMST(const T &source);


    double getEdgeWeight(const T &source, const T &destination) const;


protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    std::unordered_map<int, Vertex<T> *> vertexMap;

    double **distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;

    void TSP_BacktrackingUtil(std::vector<T> &shortestRoute, std::vector<T> &currentRoute, std::vector<bool> &visited,
                              double &shortestLength, double currentLength) const;

    void resetNodes();


    std::unordered_map<std::string, Vertex<T> *> getVertexMap() const;
};

void deleteMatrix(int **m, int n);

void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template<class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        } else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template<class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> &vertex) const {
    return this->dist < vertex.dist;
}

template<class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template<class T>
std::vector<Edge<T> *> Vertex<T>::getAdj() const {
    return this->adj;
}

template<class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template<class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template<class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template<class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template<class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template<class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template<class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template<class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template<class T>
void Vertex<T>::setProcesssing(bool processing) {
    this->processing = processing;
}

template<class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template<class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template<class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template<class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        } else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template<class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

template<class T>
Vertex<T> *Edge<T>::getDest() const {
    return this->dest;
}

template<class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

template<class T>
Vertex<T> *Edge<T>::getOrig() const {
    return this->orig;
}

template<class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template<class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template<class T>
double Edge<T>::getFlow() const {
    return flow;
}

template<class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template<class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template<class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template<class T>
std::unordered_map<std::string, Vertex<T> *> Graph<T>::getVertexMap() const {
    return vertexMap;
}


template<class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const {
    auto it = vertexMap.find(in);
    if (it != vertexMap.end()) {
        return it->second;
    }
    return nullptr;
}


template<class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}


template<class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    Vertex<T> *newVertex = new Vertex<T>(in);
    vertexSet.push_back(newVertex);
    vertexMap[in] = newVertex;
    /*nodesMAP.insert({in, newVertex});*/
    return true;
}



template<class T>
bool Graph<T>::removeVertex(const T &in) {
    auto it = vertexMap.find(in);
    if (it == vertexMap.end()) {
        return false;
    }
    Vertex<T> *v = it->second;
    for (auto u: vertexSet) {
        if (u != v) {
            u->removeEdge(v->getInfo());
        } else {
            u->removeOutgoingEdges();
        }
    }
    vertexSet.erase(std::remove(vertexSet.begin(), vertexSet.end(), v), vertexSet.end());
    vertexMap.erase(it);
    delete v;
    return true;
}


template<class T>
void Graph<T>::resetNodes() {
    for (auto v: vertexSet) {
        v->setVisited(false);
    }
}


template<class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}


template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> *srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template<class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}


template<class T>
std::vector<T> Graph<T>::dfs() const {
    std::vector<T> res;
    for (auto v: vertexSet)
        v->setVisited(false);
    for (auto v: vertexSet)
        if (!v->isVisited())
            dfsVisit(v, res);
    return res;
}


template<class T>
std::vector<T> Graph<T>::dfs(const T &source) const {
    std::vector<int> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }
    // Set that no vertex has been visited yet
    for (auto v: vertexSet) {
        v->setVisited(false);
    }
    // Perform the actual DFS using recursion
    dfsVisit(s, res);

    return res;
}


template<class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> &res) const {
    v->setVisited(true);
    res.push_back(v->getInfo());
    for (auto &e: v->getAdj()) {
        auto w = e->getDest();
        if (!w->isVisited()) {
            dfsVisit(w, res);
        }
    }
}


template<class T>
std::vector<T> Graph<T>::bfs(const T &source) const {
    std::vector<int> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }

    // Set that no vertex has been visited yet
    for (auto v: vertexSet) {
        v->setVisited(false);
    }

    // Perform the actual BFS using a queue
    std::queue<Vertex<T> *> q;
    q.push(s);
    s->setVisited(true);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->getInfo());
        for (auto &e: v->getAdj()) {
            auto w = e->getDest();
            if (!w->isVisited()) {
                q.push(w);
                w->setVisited(true);
            }
        }
    }
    return res;
}

template<class T>
bool Graph<T>::isDAG() const {
    for (auto v: vertexSet) {
        v->setVisited(false);
        v->setProcesssing(false);
    }
    for (auto v: vertexSet) {
        if (!v->isVisited()) {
            if (!dfsIsDAG(v)) return false;
        }
    }
    return true;
}

template<class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    v->setVisited(true);
    v->setProcesssing(true);
    for (auto e: v->getAdj()) {
        auto w = e->getDest();
        if (w->isProcessing()) return false;
        if (!w->isVisited()) {
            if (!dfsIsDAG(w)) return false;
        }
    }
    v->setProcesssing(false);
    return true;
}


template<class T>
std::vector<T> Graph<T>::topsort() const {
    std::vector<int> res;

    for (auto v: vertexSet) {
        v->setIndegree(0);
    }
    for (auto v: vertexSet) {
        for (auto e: v->getAdj()) {
            unsigned int indegree = e->getDest()->getIndegree();
            e->getDest()->setIndegree(indegree + 1);
        }
    }

    std::queue<Vertex<T> *> q;
    for (auto v: vertexSet) {
        if (v->getIndegree() == 0) {
            q.push(v);
        }
    }

    while (!q.empty()) {
        Vertex<T> *v = q.front();
        q.pop();
        res.push_back(v->getInfo());
        for (auto e: v->getAdj()) {
            auto w = e->getDest();
            w->setIndegree(w->getIndegree() - 1);
            if (w->getIndegree() == 0) {
                q.push(w);
            }
        }
    }

    if (res.size() != vertexSet.size()) {
        res.clear();
        return res;
    }

    return res;
}


template<class T>
class DisjointSets {
    std::unordered_map<T, T> parent;
    std::unordered_map<T, int> rank;

public:
    void makeSet(T const &item){
        parent[item] = item;
        rank[item] = 0;
    }

    T findSet(T const &item){
        if (parent[item] != item) {
            parent[item] = findSet(parent[item]);
        }
        return parent[item];
    }

    void unionSets(T const &set1, T const &set2) {
        T root1 = findSet(set1);
        T root2 = findSet(set2);

        if (root1 != root2){
            if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }
};

template<class T>
std::vector<Edge<T>> Graph<T>::kruskalMST(const T &source) {
    std::vector<Edge<T>> edges;
    std::vector<T> vertices;
    for (auto v : vertexSet) {
        vertices.push_back(v->getInfo());
        for (auto e : v->getAdj()) {
            edges.push_back(*e);
        }
    }

    std::sort(edges.begin(), edges.end(), [&source](const Edge<T>& a, const Edge<T>& b) {
        if (a.getOrig()->getInfo() == source && b.getOrig()->getInfo() != source) {
            return true;
        } else if (a.getOrig()->getInfo() != source && b.getOrig()->getInfo() == source) {
            return false;
        } else {
            return a.getWeight() < b.getWeight();
        }
    });

    DisjointSets<T> ds;
    for (const T &v : vertices) {
        ds.makeSet(v);
    }

    std::vector<Edge<T>> mst;
    for (const Edge<T> &e : edges) {
        T u = e.getOrig()->getInfo();
        T v = e.getDest()->getInfo();
        if (ds.findSet(u) != ds.findSet(v)) {
            mst.push_back(e);
            ds.unionSets(u, v);
        }
    }

    std::vector<Edge<T>> result;
    if (std::find(vertices.begin(), vertices.end(), source) != vertices.end()) {
        T sourceComponent = ds.findSet(source);
        for (const Edge<T> &e : mst) {
            if (ds.findSet(e.getOrig()->getInfo()) == sourceComponent || ds.findSet(e.getDest()->getInfo()) == sourceComponent) {
                result.push_back(e);
            }
        }
    }
    return result;
}

template<class T>
double Graph<T>::getEdgeWeight(const T &source, const T &destination) const {
    Vertex<T> *v = findVertex(source);
    if (v == nullptr) {
        return std::numeric_limits<double>::max();
    }
    for (auto e: v->getAdj()) {
        if (e->getDest()->getInfo() == destination) {
            return e->getWeight();
        }
    }
    return std::numeric_limits<double>::max();

}

inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete[] m[i];
        delete[] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete[] m[i];
        delete[] m;
    }
}

template<class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());

}


#endif /* DA_TP_CLASSES_GRAPH */