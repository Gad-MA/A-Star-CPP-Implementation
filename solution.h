/*
  This is a function to read a CSV file
  Adjust this code to skip the comment lines and take a BONUS!
  OR leave it as is and delete the comment lines in the CSV file yourself (NO BONUS for you!)
*/
vector<vector<string>> readCSV(const string &filename)
{
  vector<vector<string>> data;
  ifstream file(filename);
  string line;

  while (getline(file, line))
  {
    if (line[0] == '#' || line.empty()) // checks if the line is a comment or empty to skip this iteration
    {
      continue;
    }
    stringstream lineStream(line);
    string cell;
    vector<string> row;

    while (getline(lineStream, cell, ','))
    {
      row.push_back(cell);
    }
    data.push_back(row);
  }

  return data;
}

/*
  This is a function to write a CSV file
*/

void writeCSV(const vector<int> &data, const string &filename)
{
  ofstream file(filename);

  for (const auto &cell : data)
  {
    file << cell;
    file << ',';
  }
  file << '\n';
}
