// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#ifndef HELPER_STRUCTS_H
#define HELPER_STRUCTS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <queue>

// may get rid of cage type - make function that checks in prims..

struct Vertex {
    int x;
    int y;

    bool operator==(const Vertex& other) const {
        return x == other.x && y == other.y;
    }
};

struct Edge {
    uint32_t from;
    uint32_t to;
    double weight;
    Edge(uint32_t f, uint32_t t, double w) : from(f), to(t), weight(w) {}
    
    // Comparator for priority queue in Kruskals
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

    struct OptimalPrintInfo
    {
        bool visitied = false;
        double distance = std::numeric_limits<double>::infinity();
        uint32_t parentIndex;
        uint32_t nodeIndex;
    };

#endif // HELPER_STRUCTS_H