//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "globalFunctions.h"

std::vector<Vertex> readInput(uint32_t &cages)
{
    std::cin >> cages;
    std::vector<Vertex> vertices;
    vertices.reserve(cages);
    int x, y;
    for (uint32_t i = 0; i < cages; ++i)
    {
        std::cin >> x >> y;
        vertices.push_back({x, y});
    }
    return vertices;
}

double calculateDistance(const Vertex &a, const Vertex &b)
{
    // Make int64 for extremely large values (could be negative)
    int64_t xDiff = (a.x - b.x);
    int64_t yDiff = (a.y - b.y);
    return std::sqrt(xDiff * xDiff + yDiff * yDiff);
}

double calculateTotalDistance(const std::vector<size_t>& tour, const std::vector<Vertex>& vertices) {
    double totalDistance = 0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        totalDistance += calculateDistance(vertices[tour[i]], vertices[tour[i + 1]]);
    }
    // Add distance to return to start
    totalDistance += calculateDistance(vertices[tour.back()], vertices[tour.front()]);
    return totalDistance;
}

