#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <array>

class Node
{
public:
    int row;
    int col;
    int g_score;
    int heuristic;
    int f_score; // f_score = g_score + heuristic
    Node *parent;
    Node(int row, int col)
    {
        this->row = row;
        this->col = col;
        this->g_score = numeric_limits<int>::max();
        this->heuristic = numeric_limits<int>::max();
        this->f_score = numeric_limits<int>::max();
        this->parent = nullptr;
    }

    Node()
    {
        this->row = 0;
        this->col = 0;
        this->g_score = numeric_limits<int>::max();
        this->heuristic = numeric_limits<int>::max();
        this->f_score = numeric_limits<int>::max();
        this->parent = nullptr;
    }
};

class nodes_comparator
{
public:
    int operator()(const Node *n1, const Node *n2)
    {
        return n1->f_score > n2->f_score;
    }
};

// Functions Declaration
int nodeID_hash(int x, int y, int width);
int calc_heuristic(const Node &currNode, int E_row, int E_col);
void construct_path(int E_row, int E_col, vector<vector<char>> &grid, unordered_map<int, Node> &nodes);
void print_map(vector<vector<char>> &grid, unordered_set<int> &pathPoints);
vector<Node *> get_neighbours(int currRow, int currCol, int E_row, int E_col, vector<vector<char>> &grid, unordered_map<int, Node> &nodes);

// A-Star pathfinding algorithm implementation
void AStar(vector<vector<char>> &grid, int S_row, int S_col, int E_row, int E_col)
{
    unordered_map<int, Node> nodes;
    priority_queue<Node *, vector<Node *>, nodes_comparator> openList;
    int grid_width = grid[0].size();

    // Init starting node
    Node startingNode = Node(S_row, S_col);
    int startingNodekey = nodeID_hash(S_row, S_col, grid_width);
    startingNode.g_score = 0;
    startingNode.heuristic = calc_heuristic(startingNode, E_row, E_col);
    startingNode.f_score = startingNode.heuristic;
    nodes[startingNodekey] = startingNode;
    openList.push(&nodes[startingNodekey]);

    while (!openList.empty())
    {
        Node *currNode = openList.top();
        openList.pop();

        if (currNode->row == E_row && currNode->col == E_col)
            return construct_path(E_row, E_col, grid, nodes);

        vector<Node *> neighbours = get_neighbours(currNode->row, currNode->col, E_row, E_col, grid, nodes);
        int tentative_g_score = currNode->g_score + 1;
        for (Node *neighbour : neighbours)
        {
            if (tentative_g_score < neighbour->g_score)
            {
                neighbour->g_score = tentative_g_score;
                neighbour->f_score = tentative_g_score + neighbour->heuristic;
                neighbour->parent = currNode;
                openList.push(neighbour);
            }
        }
    }
    cout << "path not found" << endl;
}

int nodeID_hash(int x, int y, int width)
{
    return (x * width + y);
}

int calc_heuristic(const Node &currNode, int E_row, int E_col)
{
    return abs(currNode.row - E_row) + abs(currNode.col - E_col);
}

void construct_path(int E_row, int E_col, vector<vector<char>> &grid, unordered_map<int, Node> &nodes)
{
    unordered_set<int> pathPoints;
    vector<array<int, 2>> path;
    int grid_width = grid[0].size();
    Node endNode = nodes[nodeID_hash(E_row, E_col, grid_width)];
    Node currNode = endNode;
    while (currNode.parent != nullptr)
    {
        array<int, 2> coordinates = {currNode.row, currNode.col};
        path.push_back(coordinates);
        pathPoints.emplace(currNode.row * grid_width + currNode.col);
        currNode = *currNode.parent;
    }
    array<int, 2> coordinates = {currNode.row, currNode.col};
    path.push_back(coordinates);
    pathPoints.emplace(currNode.row * grid_width + currNode.col);

    reverse(path.begin(), path.end());

    print_map(grid, pathPoints);
    cout << "Goal Reached" << endl;

    cout << "Path: " << endl;
    for (const auto &point : path)
    {
        cout << "(" << point[0] << ", " << point[1] << ") ";
    }
    cout << endl;
};

void print_map(vector<vector<char>> &grid, unordered_set<int> &pathPoints)
{
    int grid_width = grid[0].size();
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid_width; col++)
        {
            if (pathPoints.find(row * grid_width + col) != pathPoints.end() && grid[row][col] != 'S' && grid[row][col] != 'E')
            {
                // cout << "\033[32m+\033[0m ";
                cout << "# ";
            }
            else
            {
                cout << grid[row][col] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

vector<Node *> get_neighbours(int currRow, int currCol, int E_row, int E_col, vector<vector<char>> &grid, unordered_map<int, Node> &nodes)
{
    vector<Node *> neighbours;
    int grid_width = grid[0].size();
    int grid_height = grid.size();

    //     {row, col}   Up       Down    right   left
    int moves[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    for (const auto &move : moves)
    {
        int row = currRow + move[0];
        int col = currCol + move[1];
        if (row >= 0 && col >= 0 && row < grid_height && col < grid_width && (grid[row][col] == '1' || grid[row][col] == 'E'))
        {
            int neighbourNodeKey = nodeID_hash(row, col, grid_width);
            if (nodes.find(neighbourNodeKey) == nodes.end())
            {
                Node neigbhourNode = Node(row, col);
                neigbhourNode.heuristic = calc_heuristic(neigbhourNode, E_row, E_col);
                nodes[neighbourNodeKey] = neigbhourNode;
            }
            neighbours.push_back(&nodes[neighbourNodeKey]);
        }
    }
    return neighbours;
}