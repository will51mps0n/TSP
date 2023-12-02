//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "globalFunctions.h"

class FASTZoo
{
private:
    uint32_t numCages;
    std::vector<Vertex> vertices;

public:
    FASTZoo(uint32_t cages, std::vector<Vertex> &v) : numCages(cages), vertices(v) {}
    void outputPath(std::vector<size_t> &cageParent);
    std::vector<size_t> createOptimalPath();
};

// Using nearest insertion technique for path creation
std::vector<size_t> FASTZoo::createOptimalPath()
{
    std::vector<bool> includedInMST(numCages, false);
    std::vector<size_t> cageComingFrom(numCages, std::numeric_limits<size_t>::max());

    includedInMST[0] = true;
    double minDistance = std::numeric_limits<double>::max();
    size_t nearestNodeToStart = std::numeric_limits<size_t>::max();

    // Find the nearest node to the start
    for (size_t i = 1; i < numCages; i++)
    {
        double currentDistance = calculateDistance(vertices[0], vertices[i]);
        if (currentDistance < minDistance)
        {
            minDistance = currentDistance;
            nearestNodeToStart = i;
        }
    }

    if (nearestNodeToStart == std::numeric_limits<size_t>::max())
    {
        std::cerr << "First node not initialized for nearest insertion" << '\n';
        exit(1);
    }

    // Initialize the tour with the start node and the nearest node
    cageComingFrom[0] = nearestNodeToStart;
    cageComingFrom[nearestNodeToStart] = 0;
    includedInMST[nearestNodeToStart] = true;
    size_t nodesIncluded = 2;

    // Construct the tour
    while (nodesIncluded < numCages)
    {
        minDistance = std::numeric_limits<double>::max();
        size_t nearestNode = std::numeric_limits<size_t>::max();
        size_t insertAfter = std::numeric_limits<size_t>::max();
        size_t insertBefore = std::numeric_limits<size_t>::max();

        // Find the nearest node to any node in the tour
        for (size_t i = 0; i < numCages; i++)
        {
            if (!includedInMST[i])
            {
                for (size_t j = 0; j < numCages; j++)
                {
                    if (includedInMST[j])
                    {
                        double distanceToJ = calculateDistance(vertices[i], vertices[j]);
                        size_t nextJ = cageComingFrom[j];
                        double distanceFromJtoNext = calculateDistance(vertices[j], vertices[nextJ]);
                        double newDistanceIfInserted = distanceToJ + calculateDistance(vertices[i], vertices[nextJ]) - distanceFromJtoNext;
                        if (newDistanceIfInserted < minDistance)
                        {
                            minDistance = newDistanceIfInserted;
                            nearestNode = i;
                            insertAfter = j;
                            insertBefore = nextJ;
                        }
                    }
                }
            }
        }

        if (nearestNode == std::numeric_limits<size_t>::max())
        {
            std::cerr << "Error: No suitable node found to add" << '\n';
            exit(1);
        }

        // Insert the nearest node at the best position in the tour
        cageComingFrom[insertAfter] = nearestNode;
        cageComingFrom[nearestNode] = insertBefore;

        includedInMST[nearestNode] = true;
        nodesIncluded++;
    }
    return cageComingFrom;
}

void FASTZoo::outputPath(std::vector<size_t> &cageParent) {
    double totalWeight = 0;
    size_t currentCage = 0; // Start from the first cage

    // Calculate the total weight
    for (size_t i = 0; i < numCages; ++i) {
        totalWeight += calculateDistance(vertices[currentCage], vertices[cageParent[currentCage]]);
        currentCage = cageParent[currentCage];
    }
    // Add weight to return to start
    totalWeight += calculateDistance(vertices[currentCage], vertices[0]);

    // Output the total weight
    std::cout << totalWeight << '\n';

    // Output the tour sequence
    currentCage = 0; // Reset to start from the first cage again
    for (size_t i = 0; i < numCages; ++i) {
        std::cout << currentCage;
        if (i < numCages - 1) {
            std::cout << ' ';
        }
        currentCage = cageParent[currentCage];
    }
    std::cout << '\n';
}

