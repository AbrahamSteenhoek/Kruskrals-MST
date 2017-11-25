#ifndef ADJ_LIST
#define ADJ_LIST

#include <vector>
#include <iostream>
#include <ostream>
#include <queue>
#include <list>
#include "disjoint.h"

// cd /mnt/c/Users/AJ\ Steenhoek/Documents/Fall_2017/Data_Structures/Prog4/Prog4_v2

using namespace std;

template <typename Weight, bool Directed=false>
class AdjList {
public:
    typedef int Vertex;
    struct Edge3 { Vertex u; Vertex v; Weight w; };
private:
    std::vector<std::vector<Edge3> > adj;
    unsigned int nEdges;
    DisjointSet dSet;
    list<Edge3> mst;
public:
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    // min priority queue that stores Edge3's
    // not a direct heap tree structure, just a linked list
    class Edge3PQ {
        struct Node {
            Node *link;
            Edge3 edge;
            Node(Edge3 _edge): edge(_edge), link(nullptr) { }
        };
        unsigned int size;
    public:
        Node* front;

        Edge3PQ(): front(nullptr), size(0) { ; }

        void push(Edge3 edge) {
            //cout << "push edge {" << edge.u << ", " << edge.v << ", " << edge.w << "}" << endl;
            Node *temp, *node;
            temp = new Node(edge);
            // empty pq
            if (front == nullptr) {
                front = temp;
            }
            // edge has lower weight than min
            else if (edge.w < front->edge.w) {
                temp->link = front;
                front = temp;
            }
            else {
                node = front;
                while (node->link != nullptr && node->link->edge.w <= edge.w) {
                    node = node->link;
                }
                temp->link = node->link;
                node->link = temp;
            }
            size++;
        }

        Edge3 top() { return front->edge; }

        void pop() {
            if(size > 0) {
                Node* temp = front->link;
                free(front);
                front = temp;
                size--;
            }
        }   

        bool empty() { return size == 0; }

        void print() {
            cout << "size: " << size << endl;
            if(front == nullptr)
                cout << "empty" << endl;
            else {
                Node *temp = front;
                while (temp != nullptr) {
                    cout << "U: " << temp->edge.u << " V: " << temp->edge.v << " Weight: " << temp->edge.w << endl;
                    temp = temp->link;
                }
            }
        }
    }; // end of class Edge3PQ
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    Edge3PQ minpq;

    unsigned int EdgeCount() const { return nEdges; }
    unsigned int VertexCount() const { return adj.size(); }
    
    void addVertex(Vertex u) {
        if (VertexCount() <= u) {
            adj.resize(u + 1);
        }     
    }

    void printPQ() { minpq.print(); }

    void addEdge(Vertex u, Vertex v, Weight w) {
        minpq.push({u,v,w});
        addVertex(std::max(u,v));
        adj[u].push_back({u,v,w});
        if (!Directed) {
            adj[v].push_back({u,v,w});
        }
        nEdges++;

    }

    AdjList() : nEdges(0) { ; }
    AdjList(std::istream &is) {
        Vertex u,v; Weight w;
        while (is >> u >> v >> w) {
            addEdge(u,v,w);
        }
        for(int i = 0; i < adj.size(); i++) {
            dSet.addSingleton(i);
        }
        //dSet.print();
    }

    void print(std::ostream &os) {
        std::cout << "Graph:" << std::endl;
        for(Vertex u = 0; u < VertexCount(); u++) {
            os << u << ":";
            for(auto e : adj[u]) {
                os << " {" << e.u << ", " << e.v << ", " << e.w << "}";
            }
            os << std::endl;
        }
        //std::cout << "\nParents:" << std::endl;
        
    }

    void printMST() {
        Weight mstWeight = calculateMST();
        for (Edge3 edge : mst) {
            cout << edge.u << " " << edge.v << endl;
        }
        cout << "The total weight is " << mstWeight << endl;
    }

    bool isCycle(Vertex x, Vertex y) {
        return dSet.find(x) == dSet.find(y);
    }

    struct EdgeComp {
        bool operator() (Edge3 edge1, Edge3 edge2) {
            return edge1.w > edge2.w;
        }
    };
    //Kruskrals
    Weight calculateMST() {
        mst.clear();
        Weight totalWeight = 0;
        
        while (!minpq.empty()) {
            Edge3 edge = minpq.top();
            minpq.pop();
            // if next edge creates a cycle
            if (isCycle(edge.u, edge.v))
                continue;
            // else
            dSet.merge(edge.u, edge.v);
            mst.push_back(edge);
            totalWeight += edge.w;
        }
        return totalWeight;
    }

}; // end of class AdjList

#endif