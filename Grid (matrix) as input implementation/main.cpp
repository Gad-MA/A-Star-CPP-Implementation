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
    // the positions of starting node and the end node are 
    // assigned while reading the csv file and populationg a 2D vector (csv_to_2D_Vector)
    int S_row;
    int S_col;
    int E_row;
    int E_col;

    vector<vector<char>> grid = csv_to_2D_Vector(argv[1], S_row, S_col, E_row, E_col);


    AStar(grid, S_row, S_col, E_row, E_col);
}
