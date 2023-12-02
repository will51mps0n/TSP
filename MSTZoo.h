// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "helperStructs.h"
#include "globalFunctions.h"

class MSTZoo
{
private:
    uint32_t numCages;
    std::vector<Vertex> vertices;
public:
    MSTZoo(uint32_t cages, std::vector<Vertex> &v) : 
                      numCages(cages),vertices(v) {}

    void PrimMST();
};

//Normal functions just needed for MST
bool notCompatibleCages(const Vertex &a, const Vertex &b)
{
    uint8_t wild = 0;
    uint8_t outer = 0;
    std::vector<Vertex> validateVertices;
    validateVertices.push_back(a);
    validateVertices.push_back(b);
    for (auto it = validateVertices.begin(); it != validateVertices.end(); ++it)
    {
        if (it->x < 0 && it->y < 0)
        {
            wild++;
        }
        else if ((it->x == 0 && it->y <= 0) || (it->y == 0 && it->x <= 0))
        {
            break;
        }
        else
        {
            outer++;
        }
    }
    return ((wild == 1) && (outer == 1));
}

double calculateCompatibleDistance(const Vertex &a, const Vertex &b)
{
    // Check for direct connection between wild and other animal cages
    if (notCompatibleCages(a, b))
    {
        return std::numeric_limits<double>::infinity();
    }
    // Otherwise, return the Euclidean distance
    return calculateDistance(a, b);
}

// add distance function inside - one takes in MST, one that doesnt
// vertex can just be x and y
// add flag to see if cagetype should be considered
void MSTZoo::PrimMST() {
    // Initialize data structures
    std::vector<double> minimumEdgeWeights(numCages, std::numeric_limits<double>::infinity());
    std::vector<bool> includedInMST(numCages, false);
    std::vector<size_t> cageParent(numCages, std::numeric_limits<size_t>::max());
    std::priority_queue<std::pair<double, size_t>, std::vector<std::pair<double, size_t>>, std::greater<>> pq;

    // Start with the first node
    minimumEdgeWeights[0] = 0.0; 
    pq.push({0.0, 0});

    while (!pq.empty()) {
        size_t selectedCage = pq.top().second;
        pq.pop();

        // Skip if already included in MST
        if (includedInMST[selectedCage]) continue;

        // Include this cage in the MST
        includedInMST[selectedCage] = true;

        // Update the minimum edge weight for each cage connected to the selected cage
        for (size_t adjacentCageIndex = 0; adjacentCageIndex < numCages; ++adjacentCageIndex) {
            // Skip if it's the selected cage itself or if it's already included in the MST
            if (adjacentCageIndex == selectedCage || includedInMST[adjacentCageIndex]) continue;

            double edgeWeight = calculateCompatibleDistance(vertices[selectedCage], vertices[adjacentCageIndex]);

            // Update if a smaller edge weight is found
            if (edgeWeight < minimumEdgeWeights[adjacentCageIndex]) {
                minimumEdgeWeights[adjacentCageIndex] = edgeWeight;
                cageParent[adjacentCageIndex] = selectedCage;
                pq.push({edgeWeight, adjacentCageIndex});
            }
        }
    }

    // Calculate the total weight of the MST and output the edges
    double totalMinimumWeight = 0;
    for (size_t cageIndex = 1; cageIndex < numCages; ++cageIndex) {
        if (cageParent[cageIndex] == std::numeric_limits<size_t>::max()) {
            std::cerr << "Cannot construct MST: Disconnected Graph" << '\n';
            exit(1);
        }
        totalMinimumWeight += minimumEdgeWeights[cageIndex];
    }

    std::cout << totalMinimumWeight << '\n';
    for (size_t cageIndex = 1; cageIndex < numCages; ++cageIndex) {
        size_t smallerInt = std::min(cageParent[cageIndex], cageIndex);
        size_t largerInt = std::max(cageParent[cageIndex], cageIndex);
        std::cout << smallerInt << " " << largerInt << '\n';
    }
}
