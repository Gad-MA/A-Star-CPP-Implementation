#include <limits>
#include <unordered_map>
#include <algorithm>
class Node
{
public:
    int id;
    float x;
    float y;
    float cost_to_reach;
    float heuristic;
    float f_score; // f_score = cost_to_reach + heuristic
    Node *parent;
    Node(int id, float x, float y, float heuristic)
    {
        this->id = id;
        this->x = x;
        this->y = y;
        this->cost_to_reach = numeric_limits<float>::max();
        this->heuristic = heuristic;
        this->f_score = numeric_limits<float>::max();
        ; // f_score = cost_to_reach + heuristic
        this->parent = nullptr;
    }
    Node() : id(0), x(0.0f), y(0.0f),cost_to_reach(numeric_limits<float>::max()) ,heuristic(0.0f), f_score(numeric_limits<float>::max()), parent(nullptr) {}
};

int startNodeID = 1;
int goalNodeID = 12;

// Declaration of functions
unordered_map<int, Node> nodes_from_csv(vector<vector<string>> nodes_csv);
unordered_map<int, vector<pair<int, float>>> adjList_from_edges(vector<vector<string>> edges_csv);
void exploreNeighbours(Node &currNode, vector<int> &openSet, vector<int> &closedSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes);
vector<int> construct_path(unordered_map<int, Node> &nodes);

vector<int> AStarAlgorithm(vector<vector<string>> edges_csv, vector<vector<string>> nodes_csv)
{
    vector<int> path;

    // populating the nodes and the adjList
    unordered_map<int, Node> nodes = nodes_from_csv(nodes_csv);
    unordered_map<int, vector<pair<int, float>>> adjList = adjList_from_edges(edges_csv);

    // initializng the openSet and the closedSet
    vector<int> openSet = {startNodeID}; // a vector that has the IDs of the nodes that are still to be tested
    vector<int> closedSet = {}; // a vector that has the IDs of the nodes that we've tested

    // the main loop implementating the algorithm
    while (!openSet.empty())
    {
        int top_node_ID_index = 0; // A variable to keep track of the index of the ID of the node we need to process next in the OpenSet

        // A loop to get the index of the node to be processed next in the openSet
        for (int i = 0, openSet_size = openSet.size(); i < openSet_size; i++)
        {
            if (nodes[openSet[i]].f_score < nodes[openSet[top_node_ID_index]].f_score)
            {
                top_node_ID_index = i;
            }
        }

        Node &currNode = nodes[openSet[top_node_ID_index]];

        exploreNeighbours(currNode, openSet, closedSet, adjList[currNode.id], nodes);

        closedSet.push_back(openSet[top_node_ID_index]);
        openSet.erase(openSet.begin() + top_node_ID_index);
    }

    path = construct_path(nodes);

    return path;
}

unordered_map<int, Node> nodes_from_csv(vector<vector<string>> nodes_csv)
{
    unordered_map<int, Node> nodes;
    for (int i = 0, vector_size = nodes_csv.size(); i < vector_size; i++)
    {
        int id = stoi(nodes_csv[i][0]);
        float x = stof(nodes_csv[i][1]);
        float y = stof(nodes_csv[i][2]);
        float heuristic = stof(nodes_csv[i][3]);
        Node tmpNode = Node(id, x, y, heuristic);
        if (id == startNodeID)
        {
            tmpNode.cost_to_reach = 0;
            tmpNode.f_score = heuristic;
        }
        nodes.emplace(id, tmpNode);
    }
    return nodes;
}

unordered_map<int, vector<pair<int, float>>> adjList_from_edges(vector<vector<string>> edges_csv)
{
    unordered_map<int, vector<pair<int, float>>> adjList;
    for (int i = 0, edges_csv_size = edges_csv.size(); i < edges_csv_size; i++)
    {
        int ID1 = stoi(edges_csv[i][0]);
        int ID2 = stoi(edges_csv[i][1]);
        float cost = stof(edges_csv[i][2]);
        adjList[ID1].push_back(make_pair(ID2, cost));
        adjList[ID2].push_back(make_pair(ID1, cost));
    }
    return adjList;
}

void exploreNeighbours(Node &currNode, vector<int> &openSet, vector<int> &closedSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes)
{
    for (const auto &[neighbourID, edge_weight] : neighbours)
    {
        Node &neighbourNode = nodes[neighbourID];
        float tentative_cost = currNode.cost_to_reach  + edge_weight;
        if (tentative_cost < neighbourNode.cost_to_reach)
        {
            neighbourNode.parent = &currNode;
            neighbourNode.cost_to_reach = tentative_cost;
            neighbourNode.f_score = tentative_cost + neighbourNode.heuristic;
        }

        bool neighbour_not_in_closedSet = (find(closedSet.begin(), closedSet.end(), neighbourID) == closedSet.end());
        bool neighbour_not_in_openSet = (find(openSet.begin(), openSet.end(), neighbourID) == openSet.end());
        bool neighbour_not_goalNode = (neighbourID != goalNodeID);

        if (neighbour_not_in_closedSet && neighbour_not_in_openSet && neighbour_not_goalNode)
        {
            openSet.push_back(neighbourID);
        }
    }
}


vector<int> construct_path(unordered_map<int, Node> &nodes)
{
    vector<int> path;
    Node goalNode = nodes[goalNodeID];
    Node currNode = goalNode;
    while (currNode.parent != nullptr)
    {
        path.push_back(currNode.id);
        currNode = *currNode.parent;
    }
    path.push_back(currNode.id);
    reverse(path.begin(), path.end());

    cout << "path: ";
    for (const auto &n : path)
    {
        cout << n << " ";
    }
    cout << " " << endl;

    return path;
}