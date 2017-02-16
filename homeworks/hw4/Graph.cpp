//
// Created by rdelfin on 2/15/17.
//

#include "Graph.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <map>

Graph::Graph(std::vector<Edge> edges)
    : edges(edges){

}

Graph::Graph(std::string fileName) {
    std::ifstream input(fileName, std::ifstream::in);
    input >> len;       // Read length, followed by every edge

    while(!input.eof()) {
        Edge e;
        input >> e.start;
        input >> e.end;

        if(!input.eof())
            edges.push_back(e);
    }

    input.close();
}

void Graph::initAdjacentyMatrix() {
    adjMat = std::vector<std::vector<bool>>(len, std::vector<bool>(len, false));

    for(auto it = edges.begin(); it != edges.end(); ++it) {
        adjMat[it->start][it->end] = true;
    }
}

void Graph::print() {
    std::cout << "Total vertices: " << len << "\n========================\n" << std::endl;
    for(auto it = edges.begin(); it != edges.end(); ++it) {
        std::cout << "E (" << it->start << ", " << it->end << ")" << std::endl;
    }
}

std::vector<std::vector<bool>> Graph::transitiveClosureMatrix() {
    std::vector<std::vector<bool>> result(adjMat);

    std::map<int, bool> unsearchedNodes;
    std::vector<int> currentSearch;
    std::unordered_map<int, bool> searchedNodes;

    // Inserts all nodes as "unsearched"
    for(int i = 0; i < unsearchedNodes.size(); i++)
        unsearchedNodes.insert({i, true});

    // Pop out items from the unsearched list until all have been searched
    // Escencially traverse from every node to every node
    while(!unsearchedNodes.empty()) {
        int n = unsearchedNodes.begin()->first;
        unsearchedNodes.erase(unsearchedNodes.begin());
        currentSearch.push_back(n);

        std::unordered_map<int, bool> newEdgeMap;
        for(int i = 0; i < result[n].size(); i++) {
            if(searchedNodes.count(i) != 0 && result[n][i] == 1) {
                for(int j = 0; j < currentSearch.size(); j++) {
                    result[currentSearch[j]][i] = 1;
                }

                currentSearch.insert()

            }
        }
    }
}

Graph::~Graph() {

}


