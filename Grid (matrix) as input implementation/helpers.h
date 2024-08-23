vector<vector<char>> csv_to_2D_Vector(const string &filename) {
    vector<vector<char>> result;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << endl;
        return result;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "[" || line == "]") {
            continue;  // Skip empty lines and lines with only outer brackets
        }
        
        vector<char> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            for (char ch : cell) {
                if (ch != '[' && ch != ']' && ch != ' ') {
                    row.push_back(ch);  // Skip inner brackets and spaces
                }
            }
        }

        result.push_back(row);
    }

    file.close();
    return result;
}