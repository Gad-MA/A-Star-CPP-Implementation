#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#include "helpers.h"
#include "a_star_algorithm.h"

int main(int argc, char const *argv[])
{
    vector<vector<char>> grid = csv_to_2D_Vector(argv[1]);

    int S_row = 0;
    int S_col = 2;
    int E_row = 19;
    int E_column = 2;

    AStar(grid, S_row, S_col, E_row, E_column);
}
