#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
#include "solution.h"
#include "algorithm.h"
int main(int argc, char* argv[]) {
    //////////////////////////////////////// DO NOT Change this code ///////////////////////////////////////////////////
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <csv_file_path>" << std::endl;
        return 1;
    }
    std::vector<std::string> csvFilePaths;
    for (int i = 1; i < argc; ++i) {
        csvFilePaths.emplace_back(argv[i]);
    }
    vector<vector<string>> edges = readCSV(csvFilePaths[0]); // edges.csv (give the absolute address to this edges.csv as an input )
    vector<vector<string>> nodes = readCSV(csvFilePaths[1]); // nodes.csv (give the absolute address to this nodes.csv as an input )

    vector<int> path = AStarAlgorithm(edges,nodes);
    cout << "Path: "<<path.size(); // check that there is an output path
    writeCSV(path, csvFilePaths[2]); // path.csv (give the absolute address to this path.csv as an input )
    //////////////////////////////////////// DO NOT Change this code ///////////////////////////////////////////////////


    return 0;
}
