/**
    This is my solution to the challenge "Breadth First Search - Shortest Reach" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/bfsshortreach/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);


/**
    This class describes a Node in the problem's graph.
*/
class Node
{
    public :
        /**
            Node constructor.

            @param[in] value Node's value.
            @param[in] move_cost Cost to travel any edge from this node to a direct child.
        */
        Node(int value, int move_cost) : m_value(value), m_move_cost(move_cost), m_children() {} 
        Node() = delete;

        /**
            Inserts a direct child to the current node.

            @param[inout] node Node to register as a direct child. 
        */
        void insertChild(Node* node)
        {
            m_children[node->m_value] = node;
        }

        /**
            This method contains the actual implementation of the BFS algorithm.
            It looks for the minimal cost to travel from the current node to
            another node.

            @param[inout] node Node to reach.

            @returns The minimal possible cost to reach the node, or -1 if the node
                     is unreachable.
        */
        int getMinimalCostToNode(Node* node)
        {
            // Stores nodes that have already been travelled through the search.
            // (we don't travel twice through the same node).
            unordered_set<int> walked_values;

            // BFS makes a search layer after layer (breadth search orientation).
            // So this map stored the content of the current layer.
            unordered_map<int, Node*> m_current_nodes_layer;

            // This map is filled by the current layer to be the next layer in 
            //the algorithm. 
            unordered_map<int, Node*> m_next_nodes_layer;

            // The first layer to start from is the current node.
            walked_values.insert(m_value);
            m_current_nodes_layer[m_value] = this;

            // Result value that is increment at each layer iteration till
            // we reach the wanted node OR the end of the graph (I.e no more layer 
            // to iterate on)
            int global_cost = 0;

            // Loop stop conditions.
            bool found_node = false;
            bool reached_graph_end = false;

            // Core of the BFS algorithm.
            while (!reached_graph_end && !found_node)
            {
                // First we fill the next layer from the current one.
                for (auto p : m_current_nodes_layer)
                {
                    Node::insertChildrenNodes(p.second, walked_values, m_next_nodes_layer);
                }

                // The next layer is empty, we won't go any further than where we are now.
                if (m_next_nodes_layer.size() == 0)
                {
                    reached_graph_end = true;
                }
                else // We can go to the next layer, it costs 1 edge
                {
                    global_cost += m_move_cost;
                }

                // If we reached the node we are looking for during this iteration, 
                // we can stop.
                if (walked_values.find(node->m_value) != walked_values.end())
                {
                    found_node = true;
                }
                
                // The next layer will become the current one in the next iteration.
                m_current_nodes_layer = m_next_nodes_layer;

                // The next layer will need to restart from scratch at the next iteration.
                m_next_nodes_layer.clear();
            }

            if (found_node)
            {
                return global_cost;
            }
            else
            {
                return -1;
            }
        }

    private :

        /**
            Submethod for the BFS algorithm.
            
            It inserts a node's children to the next layer, if it has some, and if they
            don't have already been walked through by another iteration.

            @param[in] node Node to process.
            @param[inout] walked_values Nodes that have already been walked through in 
                                        the BFS algorithm.
            @param[inout] m_layer_nodes Layer to which add the children of the node.
        */
        static void insertChildrenNodes(const Node* node, unordered_set<int> &walked_values, unordered_map<int, Node*> &m_layer_nodes)
        {
            for (auto p : node->m_children)
            {
                if (walked_values.find(p.first) == walked_values.end())
                {
                    m_layer_nodes[p.first] = p.second;
                    walked_values.insert(p.first);
                }
            }
        }

        int m_value;
        int m_move_cost;
        unordered_map<int, Node*> m_children;
};



/**
    This class describes a graph for the problem.
    This is an optimized version to avoid registering non path nodes (i.e isolated nodes 
    that are not linked to any other). Therfore, this grpah can only be constructed 
    through edges.
*/
class Graph
{
    public :
        /**
            Graph constructor.

            @param[in] max_node_value The highest possible node value to be considered 
                                      when calling getCostsArray() on the graph. 
                                      It doesn't matter if this value will 
                                      be inserted through an edge or not. It is only
                                      usefull to know the size of the costs array.
            @param[in] edges_cost Cost by edge inserted in the graph. All edges have
                                  the same cost in this graph.
        **/
        Graph(int max_node_value, int edges_cost) : m_all_nodes(), m_max_node_value(max_node_value), m_edges_cost(edges_cost) {}

        virtual ~Graph() 
        {
            for(auto p : m_all_nodes)
            {
                delete p.second;
            }
        }

        /**
            Method to construct nodes through edges in the graph.
            It insert nodes if they don't already exist, and then make a 
            bidirectional link between the two. Therefore, order
            doesn't matter between arguments.

            @param[in] first_node The first node of the edge.
            @param[in] second_node The second node of the edge.
        **/
        void insertEdge(int first_node, int second_node)
        {
            if (m_all_nodes.find(first_node) == m_all_nodes.end())
            {
                m_all_nodes[first_node] = new Node(first_node, m_edges_cost);
            }

            if (m_all_nodes.find(second_node) == m_all_nodes.end())
            {
                m_all_nodes[second_node] = new Node(second_node, m_edges_cost);
            }

            // Edges are bidirectional in this graph.
            m_all_nodes[first_node]->insertChild(m_all_nodes[second_node]);
            m_all_nodes[second_node]->insertChild(m_all_nodes[first_node]);
        }

        /**
            Computes costs from a start node to every other node to be considered
            in the graph. Non reachable or non existing nodes in the graph will
            be tagged -1 in the result array. The result array size is equal to 
            max_node_value-1 and nodes costs are ordered by ascending node value
            order.

            @param[in] start_node_val The node from which compute the costs.
            
            @returns Costs array as decribed before.
        */
        vector<int> getCostsArray(int start_node_val)
        {
            vector<int> result(m_max_node_value-1, -1);
            unordered_map<int, Node*>::iterator map_search_res;

            // If asked node doesn't exists, the array stays filled with -1;
            if ((map_search_res = m_all_nodes.find(start_node_val)) != m_all_nodes.end())
            {
                int i = 0;
                int current_index = 0;
                Node* start_node = map_search_res->second;

                // For all possible values till m_max_node_value that are not the starting 
                // node, we compute the reach cost.
                for (i = 1; i <= m_max_node_value; i++)
                {
                    if (i != start_node_val)
                    {
                        if ( (map_search_res = m_all_nodes.find(i)) != m_all_nodes.end())
                        {
                            result[current_index] = start_node->getMinimalCostToNode(map_search_res->second);
                        }

                        current_index++;
                    }
                }
            }

            return move(result);
        }

    private :
        unordered_map<int, Node*> m_all_nodes;
        int m_max_node_value;
        int m_edges_cost;
};



/**
    Main function that solves the problem with exponential complexity through a BFS algorithm.
    It passes all test cases.
    
    @param[in] n Max node value.
    @param[in] m Number of edges.
    @param[in] edges Edges in the graph.
    @param[in] s Start node for the search.

    @returns Costs list from start node to every other node in the graph, ordered by node value
             order. If a node is unreachable, -1 is set for it.
*/
vector<int> bfs(int n, int m, const vector<vector<int>> &edges, int s) 
{
    int cost_by_edge = 6;
    Graph graph(n, cost_by_edge);

    for (auto e : edges)
    {
        graph.insertEdge(e[0], e[1]);
    }

    return move(graph.getCostsArray(s));
}


// Auto-generated main function.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string nm_temp;
        getline(cin, nm_temp);

        vector<string> nm = split_string(nm_temp);

        int n = stoi(nm[0]);

        int m = stoi(nm[1]);

        vector<vector<int>> edges(m);

        for (int i = 0; i < m; i++) 
        {
            edges[i].resize(2);

            for (int j = 0; j < 2; j++) 
            {
                cin >> edges[i][j];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int s;
        cin >> s;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<int> result = bfs(n, m, edges, s);

        for (int i = 0; i < result.size(); i++) {
            fout << result[i];

            if (i != result.size() - 1) {
                fout << " ";
            }
        }

        fout << "\n";
    }

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
