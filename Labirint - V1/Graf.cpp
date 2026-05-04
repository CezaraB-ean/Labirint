#include "pch.h"
#include "Graf.h"
#include <iostream>
#include <vector>

int Graf::index(int i, int j) const {
    return i * cols + j;
}

Nod* Graf::get(int i, int j) {
    return nodes[index(i, j)];
}

Graf::~Graf() {
    for (auto n : nodes)
        delete n;
}
