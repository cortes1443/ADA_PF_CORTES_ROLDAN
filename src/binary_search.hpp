#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

#include <vector>
#include "parser.hpp"

// Retorna índice o -1 si no existe
int binarySearchFirstGE(const std::vector<Solicitud>& arr, int left, int right, int k);

#endif