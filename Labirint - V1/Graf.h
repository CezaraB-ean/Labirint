#pragma once

#include <vector>

using namespace std;
extern int nrNoduri;

class Nod {
public:
    int i, j;
    Nod* right;
    Nod* down;

    Nod(int row, int col) : i(row), j(col), right(nullptr), down(nullptr) {}
};

class Graf
{
public:
	int rows, cols;
	std::vector<Nod*> nodes; // 1D vector
	
    Graf(int r, int c) : rows(r), cols(c) {
        nodes.resize(rows * cols);

        // Crearea nodurilor sub forma de matrice
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                nodes[index(i, j)] = new Nod(i, j);
            }
        }

        // Crearea muchiilor grafului
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i < rows - 1)
                    nodes[index(i, j)]->down = nodes[index(i + 1, j)];
                if (j < cols - 1)
                    nodes[index(i, j)]->right = nodes[index(i, j + 1)];
            }
        }
    }

    int index(int i, int j) const;
    Nod* get(int i, int j);
    ~Graf();

};

