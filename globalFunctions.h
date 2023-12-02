//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include "helperStructs.h"

std::vector<Vertex> readInput(uint32_t &cages);
double calculateDistance(const Vertex &a, const Vertex &b);
double calculateTotalDistance(const std::vector<size_t>& tour, const std::vector<Vertex>& vertices);