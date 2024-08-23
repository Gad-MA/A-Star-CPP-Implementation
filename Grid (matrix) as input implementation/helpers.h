vector<vector<char>> csv_to_2D_Vector(const string &filename, int &S_row, int& S_col, int& E_row, int& E_col) {
    vector<vector<char>> result;
    ifstream file(filename);

    int row_index = 0;
    int col_index = 0;
    
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << endl;
        return result;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "[" || line == "]") {
            continue;  // Skip empty lines and lines with only outer brackets
        }
        
        col_index = 0;
        vector<char> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            for (char ch : cell) {
                if (ch != '[' && ch != ']' && ch != ' ') {
                    row.push_back(ch);  // Skip inner brackets and spaces
                    if(ch == 'S') {
                        S_row = row_index;
                        S_col = col_index;
                    }
                    if(ch == 'E') {
                        E_row = row_index;
                        E_col = col_index;
                    }
                    col_index++;
                }
            }
        }

        result.push_back(row);
        row_index++;
    }

    file.close();
    return result;
}