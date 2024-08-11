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
void exploreNeighbours(Node &currNode, unordered_set<int> &openSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes);
vector<int> construct_path(unordered_map<int, Node> &nodes);

vector<int> AStarAlgorithm(vector<vector<string>> edges_csv, vector<vector<string>> nodes_csv)
{
    vector<int> path;

    /*
        Storing the nodes in a hashMap (unordered_map) where the key is the Node ID to access
        any node using its ID in constant time.
    */
    unordered_map<int, Node> nodes = nodes_from_csv(nodes_csv);

    /*
        Created and adjacency list From the edges csv file to access the neighbours of a node using its ID
     */
    unordered_map<int, vector<pair<int, float>>> adjList = adjList_from_edges(edges_csv);

    /*
        openSet has the IDs of the nodes we need to test.
        I used a set because I'm going to insert, erase, and find elements in those sets, and those operation in
        the unordered_set has an amortized time complexity of O(1) which makes them a perfect fit.
        Plus, Sets avoid duplicates, and that ensures we don't add the same nodeID twice to the openSet.
    */
    unordered_set<int> openSet = {startNodeID};

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

        openSet.erase(top_node_ID);
        exploreNeighbours(currNode, openSet, adjList[currNode.id], nodes);
    }

    return path;
}

/*
    A function to store the nodes in a hashMap (unordered_map) where the key is the Node ID to access
    any node using its ID in constant time.
*/
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

/*
    A function to create and adjacency list from the edges csv file to know what are the neighbours of each node
*/
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

/*
    A function to explore the neighbours of the node we're currently testing, and modifying the cost_to_reach
    and the f_score of the neighbour nodes
*/
void exploreNeighbours(Node &currNode, unordered_set<int> &openSet, vector<pair<int, float>> &neighbours, unordered_map<int, Node> &nodes)
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

            openSet.emplace(neighbourID); // since openSet is an unorderd_set it won't allow duplicates,
                                          //  meaning if neighbourID is already in openSet it won't be added again.

            /*
                Decided to add the neighbourID to openSet if tentaive_cost < neigbhourNode.cost_to_reach
                because if a node is reached by one path, removed from openSet, and subsequently reached
                by a cheaper path, it will be added to openSet again. And that is essential to guarantee
                that the path returned is optimal if the heuristic function is admissible but not consistent.
            */
        }
    }
}

/*
    A function to construct the output path by tracking the parents of the each node starting from the goal node.
*/
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
