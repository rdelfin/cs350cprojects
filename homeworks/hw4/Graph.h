//
// Created by rdelfin on 2/15/17.
//

#ifndef HW4_GRAPH_H
#define HW4_GRAPH_H

#include <vector>
#include <string>

class Edge {
public:
    int start;
    int end;
};

class Graph {
public:
    Graph(std::vector<Edge> edges);
    Graph(std::string fileName);

    void print();

    // C++ will use a bitvector representation for maximum efficiency
    std::vector<std::vector<bool>> transitiveClosureMatrix();

    ~Graph();
private:
    void initAdjacentyMatrix();

    std::vector<Edge> edges;
    std::vector<std::vector<bool>> adjMat;
    int len;
};


#endif //HW4_GRAPH_H
