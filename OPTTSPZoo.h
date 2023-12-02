#include "helperStructs.h"
#include "globalFunctions.h"

class OPTZoo
{
    private:
    uint32_t numCages;
    std::vector<Vertex> vertices;
    std::vector<size_t>initialOptTour;
    double initialOptLength;

    public:
    OPTZoo(uint32_t cages, std::vector<Vertex> &verts, 
            std::vector<size_t> &IOT, double IOL) : 
            numCages(cages), vertices(verts),
            initialOptTour(IOT),initialOptLength(IOL) {}
                
};