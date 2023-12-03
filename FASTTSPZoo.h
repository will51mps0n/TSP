//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9 
#include "helperStructs.h"
#include "globalFunctions.h"
#include <random>

class FASTZoo
{
private:
    uint32_t numCages;
    std::vector<Vertex> vertices;

public:
    FASTZoo(uint32_t cages, std::vector<Vertex> &v) : numCages(cages), vertices(v) {}
    void outputPath(std::vector<size_t> &cageParent);
    std::vector<size_t> createOptimalPath();
    std::pair<std::vector<size_t>, double> getOptTourAndLength(std::vector<size_t> &cageParent);
};

size_t getRandomNode(const std::vector<bool>& includedInMST, uint32_t numCages) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, numCages - 1);

    size_t randomNode;
    do {
        randomNode = distr(gen);
    } while (includedInMST[randomNode]); // Ensure the node is not already included in the tour

    return randomNode;
}

// Using random random insertion heuristic for path creation
std::vector<size_t> FASTZoo::createOptimalPath() {
    std::vector<bool> includedInMST(numCages, false);
    std::vector<size_t> cageComingFrom(numCages, std::numeric_limits<size_t>::max());

    // Start from a random node
    size_t startNode = getRandomNode(includedInMST, numCages);
    includedInMST[startNode] = true;
    cageComingFrom[startNode] = startNode; // Points to itself initially
    size_t nodesIncluded = 1;

    // Construct the tour
    while (nodesIncluded < numCages) {
        size_t randomNode = getRandomNode(includedInMST, numCages);
        double minDistanceIncrease = std::numeric_limits<double>::max();
        size_t insertAfter = startNode; // Default to start node
        size_t insertBefore = cageComingFrom[startNode]; // Default to the second node in the path

        // Find the best position to insert the random node
        for (size_t i = 0; i < numCages; i++) {
            if (includedInMST[i] && cageComingFrom[i] != std::numeric_limits<size_t>::max()) {
                size_t nextI = cageComingFrom[i];
                double distanceToI = calculateDistance(vertices[randomNode], vertices[i]);
                double distanceFromItoNext = calculateDistance(vertices[i], vertices[nextI]);
                double newDistanceIfInserted = distanceToI + calculateDistance(vertices[randomNode], vertices[nextI]) - distanceFromItoNext;
                if (newDistanceIfInserted < minDistanceIncrease) {
                    minDistanceIncrease = newDistanceIfInserted;
                    insertAfter = i;
                    insertBefore = nextI;
                }
            }
        }

        // Insert the random node at the best position in the tour
        cageComingFrom[insertAfter] = randomNode;
        cageComingFrom[randomNode] = insertBefore;

        includedInMST[randomNode] = true;
        nodesIncluded++;
    }

    return cageComingFrom;
}


void FASTZoo::outputPath(std::vector<size_t> &cageParent) {
    double totalWeight = 0;
    std::vector<size_t> tour;
    tour.reserve(numCages); 
    size_t currentCage = 0; 
    // Reconstruct the tour from cageParent
    for (size_t i = 0; i < numCages; ++i) {
        tour.push_back(currentCage);
        totalWeight += calculateDistance(vertices[currentCage], vertices[cageParent[currentCage]]);
        currentCage = cageParent[currentCage];
    }
    //add weight to return to start
    totalWeight += calculateDistance(vertices[currentCage], vertices[0]);
    // Output the total weight
    std::cout << totalWeight << '\n';

    // Output the tour sequence
    for (size_t i = 0; i < tour.size(); ++i) {
        std::cout << tour[i];
        if (i < tour.size() - 1) {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}


std::pair<std::vector<size_t>, double> FASTZoo::getOptTourAndLength(std::vector<size_t> &cageParent) 
{
    double totalWeight = 0;
    std::vector<size_t> tour;
    tour.reserve(numCages); 
    size_t currentCage = 0; 
    // Reconstruct the tour from cageParent
    for (size_t i = 0; i < numCages; ++i) {
        tour.push_back(currentCage);
        totalWeight += calculateDistance(vertices[currentCage], vertices[cageParent[currentCage]]);
        currentCage = cageParent[currentCage];
    }
    //add weight to return to start
    totalWeight += calculateDistance(vertices[currentCage], vertices[0]);
    // Output the total weight
    std::pair<std::vector<size_t>, double> optTourAndLength(tour, totalWeight);
    return optTourAndLength;
}