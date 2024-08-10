#include <limits>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
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
    Node() : id(0), x(0.0f), y(0.0f), cost_to_reach(numeric_limits<float>::max()), heuristic(0.0f), f_score(numeric_limits<float>::max()), parent(nullptr) {}
};

int startNodeID = 1;
int goalNodeID = 12;

// Declaration of functions
unordered_map<int, Node> nodes_from_csv(vector<vector<string>> nodes_csv);
unordered_map<int, vector<pair<int, float>>> adjList_from_edges(vector<vector<string>> edges_csv);
void exploreNeighbours(Node &currNode, unordered_set<int> &openSet, unordered_set<int> &closedSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes);
vector<int> construct_path(unordered_map<int, Node> &nodes);

vector<int> AStarAlgorithm(vector<vector<string>> edges_csv, vector<vector<string>> nodes_csv)
{
    vector<int> path;

    /*
        Storing the nodes in a hashMap (unordered_map) where the key is the Node ID to access
        any node using its ID in constant time.
        Created and adjacency list From the edges csv file to access the neighbours of a node using its ID
    */
    unordered_map<int, Node> nodes = nodes_from_csv(nodes_csv);
    unordered_map<int, vector<pair<int, float>>> adjList = adjList_from_edges(edges_csv);

    /*
        To keep track of the nodes we need to examine and the nodes we've already examined I used two sets.
        I used sets because I'm going to insert, erase, and find elements in those sets, and those operation in
        the unordered_set has an amortized time complexity of O(1) which makes them a perfect fit.
        I have two sets that contains the IDs of the nodes, where openSet has the IDs of the nodes we need to test
        and closedSet has the IDs of the nodes we've tested and we don't need to check them again.
    */
    unordered_set<int> openSet = {startNodeID};
    unordered_set<int> closedSet = {};

    // the main loop implementating the algorithm
    while (!openSet.empty())
    {
        /*
            From the openSet we need to find the ID of the node that has the least f_score to examine next.
            I used a variable (top_node_ID) to store the ID of my top node which is the node with the least f_score
            from the openSet, and used a loop to go through my set and search for the ID of the node with the least
            f_score and assigns its ID to top_node_ID
        */
        int top_node_ID = *openSet.begin(); // used *openSet.begin() to initialize top_node_ID to make sure that the
                                            // initial value of top_node_ID is a valid node ID from our openSet
        for (const auto &nodeID : openSet)
        {
            if (nodes[nodeID].f_score < nodes[top_node_ID].f_score)
            {
                top_node_ID = nodeID;
            }
        }

        Node &currNode = nodes[top_node_ID];

        if (currNode.id == goalNodeID)
        {
            return construct_path(nodes);
        }

        exploreNeighbours(currNode, openSet, closedSet, adjList[currNode.id], nodes);

        closedSet.emplace(top_node_ID);
        openSet.erase(top_node_ID);
    }

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

void exploreNeighbours(Node &currNode, unordered_set<int> &openSet, unordered_set<int> &closedSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes)
{
    for (const auto &[neighbourID, edge_weight] : neighbours)
    {
        Node &neighbourNode = nodes[neighbourID];
        float tentative_cost = currNode.cost_to_reach + edge_weight;
        if (tentative_cost < neighbourNode.cost_to_reach)
        {
            neighbourNode.parent = &currNode;
            neighbourNode.cost_to_reach = tentative_cost;
            neighbourNode.f_score = tentative_cost + neighbourNode.heuristic;
        }

        bool neighbour_not_in_closedSet = (closedSet.find(neighbourID) == closedSet.end());

        if (neighbour_not_in_closedSet)
        {
            openSet.emplace(neighbourID);
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