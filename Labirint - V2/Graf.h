#pragma once
#include <tuple>
#include <vector>
using namespace std;
extern int nrNoduri;

class Nod {
public:
    int i, j;
    Nod* right;
    Nod* down;
    bool inSolutie = false;
    bool isDeadEnd = false;

    Nod(int row, int col) : i(row), j(col), right(nullptr), down(nullptr) {}
};

class Graf
{
public:
	int rows, cols;
	std::vector<Nod*> nodes; // 1D vector
	
    Graf(int r, int c) : rows(r), cols(c) {
        nodes.resize(rows * cols);

        // Create nodes
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                nodes[index(i, j)] = new Nod(i, j);
            }
        }
    }

    int index(int i, int j) const;
    Nod* get(int i, int j);
    ~Graf();

};

