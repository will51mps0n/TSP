// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
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
double calculatePathDistance(const std::vector<size_t> &tour, const std::vector<Vertex> &vertices)
{
    double totalDistance = 0;
    for (size_t i = 0; i < tour.size() - 1; i++)
    {
        totalDistance += calculateDistance(vertices[tour[i]], vertices[tour[i + 1]]);
    }
    return totalDistance;
}
double calculateTourDistance(const std::vector<size_t> &tour, const std::vector<Vertex> &vertices)
{
    double totalDistance = calculatePathDistance(tour, vertices);
    // Add distance to return to start
    totalDistance += calculateDistance(vertices[tour.back()], vertices[tour.front()]);
    return totalDistance;
}

double GetMstWeight(const std::vector<Vertex> &vertices)
{
    size_t numCages = vertices.size();
    std::vector<double> minimumEdgeWeights(numCages, std::numeric_limits<double>::infinity());
    std::vector<bool> includedInMST(numCages, false);
    std::vector<size_t> cageParent(numCages, std::numeric_limits<size_t>::max());
    minimumEdgeWeights[0] = 0.0; // Initialize starting vertex's weight to 0

    for (size_t i = 0; i < numCages; ++i)
    {
        size_t selectedCage = std::numeric_limits<size_t>::max();

        // Find the cage with the smallest connecting edge not already included in the MST
        for (size_t nextCageIndex = 0; nextCageIndex < numCages; ++nextCageIndex)
        {
            if (!includedInMST[nextCageIndex] &&
                (selectedCage == std::numeric_limits<size_t>::max() || minimumEdgeWeights[nextCageIndex] < minimumEdgeWeights[selectedCage]))
            {
                selectedCage = nextCageIndex;
            }
        }

        includedInMST[selectedCage] = true;

        // Update the minimum edge weight for each cage connected to the selected cage
        for (size_t adjacentCageIndex = 0; adjacentCageIndex < numCages; ++adjacentCageIndex)
        {
            if (adjacentCageIndex != selectedCage && !includedInMST[adjacentCageIndex])
            {
                double edgeWeight = calculateDistance(vertices[selectedCage], vertices[adjacentCageIndex]);

                if (edgeWeight < minimumEdgeWeights[adjacentCageIndex])
                {
                    cageParent[adjacentCageIndex] = selectedCage;
                    minimumEdgeWeights[adjacentCageIndex] = edgeWeight;
                }
            }
        }
    }

    double totalMinimumWeight = 0;
    // Calculate the total weight of the MST
    for (size_t cageIndex = 1; cageIndex < numCages; ++cageIndex)
    {
        if (cageParent[cageIndex] == std::numeric_limits<size_t>::max())
        {
            std::cerr << "Cannot construct MST: Disconnected Graph" << '\n';
            exit(1);
        }
        totalMinimumWeight += minimumEdgeWeights[cageIndex];
    }
    
    return totalMinimumWeight;
}