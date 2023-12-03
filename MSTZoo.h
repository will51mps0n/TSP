// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "helperStructs.h"
#include "globalFunctions.h"

class MSTZoo
{
private:
    uint32_t numCages;
    std::vector<Vertex> vertices;
    enum CageType
    {
        Wild,
        Border,
        Outer
    };
    std::vector<CageType> cageTypes;
    void setCageTypes();

public:
    MSTZoo(uint32_t cages, std::vector<Vertex> &v) : numCages(cages), vertices(v) {cageTypes.resize(numCages);}

    void PrimMST();
};

void MSTZoo::setCageTypes()
{
    for (size_t i = 0; i < numCages; ++i) {
        if (vertices[i].x < 0 && vertices[i].y < 0) 
        {
            cageTypes[i] = Wild;
        }
        else if ((vertices[i].x == 0 && vertices[i].y <= 0) || (vertices[i].y == 0 && vertices[i].x <= 0)) {
            cageTypes[i] = Border;
        }
        else {
            cageTypes[i] = Outer;
        }
    }
    return;
}

// Normal functions just needed for MST


// add distance function inside - one takes in MST, one that doesnt
// vertex can just be x and y
// add flag to see if cagetype should be considered
void MSTZoo::PrimMST()
{
    setCageTypes();
    std::vector<double> minimumEdgeWeights(numCages, std::numeric_limits<double>::infinity());
    std::vector<bool> includedInMST(numCages, false);
    std::vector<size_t> cageParent(numCages, std::numeric_limits<size_t>::max());
    minimumEdgeWeights[0] = 0.0; // Initialize starting vertex's weight to 0 to pick first

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

        includedInMST[selectedCage] = true; // Include this cage in the MST

        // Update the minimum edge weight for each cage connected to the selected cage
        for (size_t adjacentCageIndex = 0; adjacentCageIndex < numCages; ++adjacentCageIndex)
        {
            if (adjacentCageIndex != selectedCage && !includedInMST[adjacentCageIndex])
            {
                double edgeWeight = std::numeric_limits<double>::max();

                // Check for direct connection between wild and other animal cages
                if (!((cageTypes[selectedCage] == Wild && cageTypes[adjacentCageIndex] == Outer) 
                    || (cageTypes[selectedCage] == Outer && cageTypes[adjacentCageIndex] == Wild)))
                {
                    edgeWeight = calculateDistance(vertices[selectedCage], vertices[adjacentCageIndex]);
                }

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

    std::cout << totalMinimumWeight << '\n';
    for (size_t cageIndex = 1; cageIndex < numCages; ++cageIndex)
    {
        size_t smallerInt = std::min(cageParent[cageIndex], cageIndex);
        size_t largerInt = std::max(cageParent[cageIndex], cageIndex);
        std::cout << smallerInt << " " << largerInt << '\n';
    }
}


