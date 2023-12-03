// 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "helperStructs.h"
#include "globalFunctions.h"

class OPTZoo
{
private:
    std::vector<Vertex> vertices;
    std::vector<size_t> optTour;             // Optimal tour found so far
    std::vector<size_t> currentTour;         // Current tour being evaluated
    double optLength;                        // Length of the optimal path

public:
    OPTZoo(std::vector<Vertex> &verts, std::vector<size_t> &IOT, double IOL)
        : vertices(verts), optTour(IOT), optLength(IOL)
    {
        currentTour.reserve(vertices.size());
        // Initialize currentTour with a default path, starting from 0
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            currentTour.push_back(i);
        }
    }

    void algorithm();
    bool promising(size_t permLength);
    void updateOptimalPath(const std::vector<size_t> &path);
    void genPerms(size_t permLength);
};

void OPTZoo::algorithm()
{
    genPerms(1); // Start from 1 as 0 is fixed (starting point)

    std::cout << optLength << '\n';

    // Output the tour sequence
    for (size_t i = 0; i < optTour.size(); ++i) {
        std::cout << optTour[i];
        if (i < optTour.size() - 1) {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
    return;
}

void OPTZoo::genPerms(size_t permLength)
{
    // function missing something
    if (permLength == currentTour.size())
    {
        // Base case: Complete path
        updateOptimalPath(currentTour);
        return;
    }

    if (!promising(permLength))
    {
        // Path is not promising, prune this branch
        return;
    }

    for (size_t i = permLength; i < currentTour.size(); ++i)
    {
        std::swap(currentTour[permLength], currentTour[i]);
        genPerms(permLength + 1);
        std::swap(currentTour[permLength], currentTour[i]);
    }
}

bool OPTZoo::promising(size_t permLength)
{
    std::vector<Vertex> MSTRemainingVertices;
    MSTRemainingVertices.reserve(currentTour.size() - permLength);
    for (size_t i = permLength; i < currentTour.size(); i++) {
        MSTRemainingVertices.push_back(vertices[currentTour[i]]);
    }
    double MSTWeight = GetMstWeight(MSTRemainingVertices);
    double currentPathWeight = calculatePathDistance(std::vector<size_t>(currentTour.begin(),currentTour.begin() + permLength), vertices);
    double distanceToMST = std::numeric_limits<double>::max();
    double distanceFromMST = std::numeric_limits<double>::max();

    for (const Vertex &v : MSTRemainingVertices) 
    {
        distanceFromMST = std::min(distanceFromMST,calculateDistance(v, vertices[currentTour[0]]));
        distanceToMST = std::min(distanceToMST,calculateDistance(v, vertices[currentTour[permLength - 1]]));
    }
    double totalDistance = MSTWeight + currentPathWeight + distanceToMST + distanceFromMST;

    return totalDistance < optLength;
}

void OPTZoo::updateOptimalPath(const std::vector<size_t> &path)
{
    double pathLength = calculateTourDistance(path, vertices);
    optLength = pathLength;
    optTour = path;
}