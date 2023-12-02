#include "helperStructs.h"
#include "globalFunctions.h"
#include <random>
#include <vector>
#include <limits>

class FASTZoo {
private:
    uint32_t numCages;
    std::vector<Vertex> vertices;
    std::mt19937 gen; // Random number generator
    std::vector<std::vector<double>> distanceMatrix; // Precomputed distance matrix

    size_t getRandomNode(const std::vector<char>& includedInMST) {
        std::uniform_int_distribution<> distr(0, numCages - 1);
        size_t randomNode;
        do {
            randomNode = distr(gen);
        } while (includedInMST[randomNode]);
        return randomNode;
    }

    void precomputeDistances() {
        distanceMatrix.resize(numCages, std::vector<double>(numCages));
        for (size_t i = 0; i < numCages; ++i) {
            for (size_t j = 0; j < numCages; ++j) {
                if (i != j) {
                    distanceMatrix[i][j] = calculateDistance(vertices[i], vertices[j]);
                }
            }
        }
    }

public:
    FASTZoo(uint32_t cages, std::vector<Vertex> &v) 
        : numCages(cages), vertices(v), gen(std::random_device()()) {
        precomputeDistances();
    }

    std::vector<size_t> createOptimalPath() {
        std::vector<char> includedInMST(numCages, false);
        std::vector<size_t> cageComingFrom(numCages, std::numeric_limits<size_t>::max());

        size_t startNode = getRandomNode(includedInMST);
        includedInMST[startNode] = true;
        cageComingFrom[startNode] = startNode;
        size_t nodesIncluded = 1;

        while (nodesIncluded < numCages) {
            size_t randomNode = getRandomNode(includedInMST);
            double minDistanceIncrease = std::numeric_limits<double>::max();
            size_t insertAfter = startNode;
            size_t insertBefore = cageComingFrom[startNode];

            for (size_t i = 0; i < numCages; i++) {
                if (includedInMST[i] && cageComingFrom[i] != std::numeric_limits<size_t>::max()) {
                    size_t nextI = cageComingFrom[i];
                    double distanceToI = distanceMatrix[randomNode][i];
                    double distanceFromItoNext = distanceMatrix[i][nextI];
                    double newDistanceIfInserted = distanceToI + distanceMatrix[randomNode][nextI] - distanceFromItoNext;

                    if (newDistanceIfInserted < minDistanceIncrease) {
                        minDistanceIncrease = newDistanceIfInserted;
                        insertAfter = i;
                        insertBefore = nextI;
                    }
                }
            }

            cageComingFrom[insertAfter] = randomNode;
            cageComingFrom[randomNode] = insertBefore;

            includedInMST[randomNode] = true;
            nodesIncluded++;
        }

        return cageComingFrom;
    }

    void outputPath(const std::vector<size_t> &cageParent) {
        double totalWeight = 0;
        std::vector<size_t> tour;
        tour.reserve(numCages); 
        size_t currentCage = 0; 

        for (size_t i = 0; i < numCages; ++i) {
            tour.push_back(currentCage);
            totalWeight += distanceMatrix[currentCage][cageParent[currentCage]];
            currentCage = cageParent[currentCage];
        }

        totalWeight += distanceMatrix[currentCage][0];
        std::cout << totalWeight << '\n';

        for (size_t i = 0; i < tour.size(); ++i) {
            std::cout << tour[i];
            if (i < tour.size() - 1) {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
};
