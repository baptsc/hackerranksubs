/**
    This is my solution to the challenge "Minimum Penalty Path" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/beautiful-path/problem
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/**
    This class defines a Node in the problem's graph.
**/
class Node 
{
    public:
        //Node constructors (requires a val).
        Node() = delete;
        Node(int val) : m_value(val), m_children(), m_costs() {}

        // Node destructor. Nothing special, children pointers values will be deleted by graph destruction.
        virtual ~Node() = default;
         
        /**
            Adds a directed edge between this node and another one.
            If an edge with the new child already exists, the less costy edge is kept.

            @param[in] child Child to add.
            @param[in] cost Cost of the edge between this node and the new child. 
        **/
        void addChild(const Node* child, int cost)
        {
            
            if (m_children.find(child->getValue()) != m_children.end())
            {
                // We change the existing edge only if it is less costy
                if (cost >= m_costs[child->getValue()])
                {
                    return;
                }
            }

            m_children[child->getValue()] = child;
            m_costs[child->getValue()] = cost;
        }

        /**
            Returns this node's value.

            @returns The node value.
        **/
        int getValue() const
        {
            return m_value;
        }

        /**
            Main method that solves the optimisation problem to reach another node at 
            minimal cost from this node.

            @param[in] Value of the targeted node (ID).
            
            @returns The smallest possible cost to reach the target from this node, or -1
                     if the target is not reachable.
        **/
        int getOptimalPathCost(int target_node) const
        {
            // 1 - First we perform a BFS search in the graph to get quickly one of the less 
            // costy paths (at least one possible path, but most likely among the less 
            // costy ones). It is used later on to optimize the DFS search to cut 
            // the most costy branches of the recursion tree.
            int bfs_cost = getBfsPathCost(target_node);

            // If we couldn't find any path through BFS algorithm, there is no chance to 
            // find one with DFS.
            if (bfs_cost == -1)
            {
                return -1;
            }
            else
            {
                unordered_set<int> walked_nodes;
                unordered_map<string, int> dynamic_dfs_results;

                // 2 - Secondly we perform a DFS search to refine the result from the BFS
                // search in order to find the less costy path to the target.
                // This part also uses Dynamic Programming to optimize the recursion tree
                // by avoiding computing the same results twice. 
                int result = getDfsBestCostToTarget(m_value, 0, &bfs_cost, dynamic_dfs_results, walked_nodes, target_node);

                // If the DFS couldn't find any better paths costs, we return the one found
                // by the BFS.
                return (result != -1 ? result : bfs_cost);
            }
        }
    private :

        /**
            This method performs an optimized DFS search in the graph to find the less 
            costy path to the target.
            This is a recursive method with Dynamic Programming optimization.

            @param[in] parent_val Value (ID) of the parent node that led the search to 
                                  the current node.
            @param[in] current_cost Cost of the path that led to the current node.
            @param[inout] best_cost Best path cost so far to go from the search start 
                                    node to the targeted node (initially, this should be set
                                    to the result of a BFS search to optimize the recursion).
            @param[inout] dynamic_dfs_results Associative Dynamic Programming array to
                                              store already known results of the recursion
                                              tree, in order to avoid computing the same 
                                              results twice.
            @param[inout] walked_nodes Nodes values (IDs) that are part of the current path
                                       that led to the current node. This avoids iterating
                                       twice on the same node in the same path (because 
                                       loops can exist in the graph).
            @param[in] target_val Value (ID) of the targeted node.

            @returns The smallest possible cost of the total path from the start node to the 
                     target node, passing by walked_nodes + this node. If no path exists, 
                     the method returns -1.
        **/
        int getDfsBestCostToTarget(int parent_val, int current_cost, int* best_cost, unordered_map<string, int> &dynamic_dfs_results, unordered_set<int> &walked_nodes, int target_val) const
        {
            // Dynamic Programming key. Here we can consider quite unique the key
            // "parent_node | current_node | current_cost" to identify distinct
            // results in the associative array.
            string key(to_string(parent_val)+"|"+to_string(m_value)+"|"+to_string(current_cost));
            
            unordered_map<string, int>::iterator map_it;

            // If the result of the current call is already known thanks to Dynamic Programming,
            // we just need to return it.
            if ((map_it = dynamic_dfs_results.find(key)) != dynamic_dfs_results.end())
            {
                return map_it->second;
            }

            // If the current node is already walked in the path or if the current cost
            // is already higher than the best known one, no need to go any further
            // we won't be able to improve the best known score.
            if (walked_nodes.find(m_value) != walked_nodes.end() || current_cost >= *best_cost)
            {
                return -1;
            }

            // If we reached the target with the current node,
            // we must stop the recursion here.
            if (target_val == m_value)
            {
                return current_cost;
            }

            // ELSE : let's go deeper in the recursion tree.

            // The current node is now part of the path.            
            walked_nodes.insert(m_value);

            int optimal_path_cost = -1;
            int child_cost = 0;
            int child_edge_cost = 0;
            int next_path_cost = 0;

            // We look for the best path among children. 
            for (auto c : m_children)
            {
                child_edge_cost = (m_costs.find(c.first))->second;

                child_cost = current_cost | child_edge_cost;

                // Recursive call on child.
                next_path_cost = c.second->getDfsBestCostToTarget(c.first, child_cost, best_cost, dynamic_dfs_results, walked_nodes, target_val);

                // If the current child can give us a better result than the previous kept one,
                // it becomes our bet.
                if (next_path_cost != -1 && (optimal_path_cost == -1 || next_path_cost < optimal_path_cost) )
                {                    
                    optimal_path_cost = next_path_cost;
                }
            }

            // Before returning, we must erase this node from the locked ones
            // to enable the fact that it could be iterated again but through
            // a different edge, coming from another parent path.
            walked_nodes.erase(m_value);

            // Now, we know the best possible cost from this node
            // with this specific call parameters. So we write it to the
            // dynamic programming array to avoid computing the result 
            // again.
            dynamic_dfs_results[key] = optimal_path_cost;

            // We return the best child cost only if it is better 
            // than the best known cost so far, otherwise we can consider
            // it as a dead path since it is worst than the best known one. 
            if (optimal_path_cost != -1 && optimal_path_cost < *best_cost)
            {
                *best_cost = optimal_path_cost;
                return optimal_path_cost;
            }
            else
            {
                return -1;
            }
        }

        /**
            This method performs a BFS search to find the shortest path in terms of nodes
            (not cost) from this node to a targeted node.

            @param[in] target_val Value (ID) of the targeted node.

            @returns The cost of the found path, or -1 if this node is not connected
                     to the targeted node. 
        **/
        int getBfsPathCost(int target_val) const
        {
            unordered_set<int> walked_nodes;

            unordered_map<int, const Node*> current_layer;
            // To be able to retreive the cost of the chosen path,
            // We need to keep track of costs layer after layer
            unordered_map<int, int> current_layer_costs;

            unordered_map<int, const Node*> next_layer;
            unordered_map<int, int> next_layer_costs;

            // The first layer of the BFS search is the current node.
            current_layer[m_value] = this;
            current_layer_costs[m_value] = 0;
            walked_nodes.insert(m_value);

            bool found_target = false;
            bool reached_end = false;

            int result_cost = -1;
            int loop_result = 0; 

            // The BFS search ends either when we found the targeted node,
            // or when we have no more nodes to iterate on.
            // We iterate layer after layer. One layer consisting
            // in all distinct children of the previous layer.
            while (!found_target && !reached_end)
            {
                for (auto p : current_layer)
                {
                    // We create the next layer from the current one.
                    // If this method returns something else than -1, we found the target
                    // and we can stop. 
                    loop_result = Node::insertBfsChildren(p.second, current_layer_costs[p.first], target_val, walked_nodes, next_layer, next_layer_costs);
                    
                    // The next layer contains the targeted node,
                    // we can stop here, we have the result to return.
                    if (loop_result != -1)
                    {
                        result_cost = loop_result;
                        found_target = true;
                        break;
                    }
                }

                if (!found_target)
                {
                    if (next_layer.size() > 0) // We have more nodes to iterate on and we haven't reached the target yet
                    {
                        // The next layer will be the curent layer for the next iteration
                        // And the next layer must be emptied before the next iteration.
                        current_layer = next_layer;
                        current_layer_costs = next_layer_costs;
                        next_layer.clear();
                        next_layer_costs.clear();
                    }
                    else // The next layer is empty, we reached graph's end, we can't iterate any further.
                    {
                        reached_end = true;
                    }
                }
            }

            return result_cost;
        }

        /**
            Inserts children of a node in the BFS layer for the method getBfsPathCost().
            It doesn't inserts children that are already referenced in the walked nodes.

            @param[in] node Node to extract children from.
            @param[in] parent_cost Cost of parent path till node.
            @param[in] target Target node value.
            @param[inout] walked_nodes Already walked nodes by the BFS (we don't walk any
                                       node twice in a BFS). All inserted children will
                                       be added to this list.
            @param[inout] next_layer Next BFS layer to fill with children nodes.
            @param[inout] next_layer_costs Next layer costs to update with children edge costs.

            @returns If the targeted node is among direct children, the total cost to the
                     target is returned. Otherwise, it returns -1. This return is a bit weird
                     but it helps optimizing the BFS by avoiding the need to iterate through
                     every new layer to see if we reached the target or not.
        **/
        static int insertBfsChildren(const Node* node, int parent_cost, int target, unordered_set<int> &walked_nodes, unordered_map<int, const Node*> &next_layer, unordered_map<int, int> &next_layer_costs)
        {
            int result = -1;
            int child_edge_cost = 0;

            for (auto p : node->m_children)
            {
                // We insert only if the child doesn't exist yet in the walked nodes.
                if (walked_nodes.find(p.first) == walked_nodes.end()) 
                {
                    next_layer[p.first] = p.second;

                    child_edge_cost = node->m_costs.find(p.first)->second;

                    next_layer_costs[p.first] = parent_cost | child_edge_cost;
                    walked_nodes.insert(p.first);

                    // If we found the target among children, we can stop and return the
                    // cost, the BFS won't go any further.
                    if (target == p.first)
                    {
                        result = next_layer_costs[p.first];
                        break;
                    }
                }
            }

            return result;
        }

        unordered_map<int, const Node*> m_children;
        unordered_map<int, int> m_costs;
        int m_value;
};

/**
    This class defines the graph for the problem.
**/
class Graph
{
    public :
        Graph() = default;

        /**
        	Graph destructor. Need to destruct dynamically allocated nodes.
        **/
        virtual ~Graph()
        {
        	for(auto p : m_nodes)
            {
                delete p.second;
            }
        }
        
        /**
            Adds an edge to the graph, inserting the required nodes if they don't exist yet.
            An edge is inserted on both nodes because the graph is undirected.

            @param[in] node_1_val Value of the first edge node.
            @param[in] node_2_val Value of the second edge node.
            @param[in] cost Cost of the edge.

        **/
        void addEdge(int node_1_val, int node_2_val, int cost)
        {
            if (m_nodes.find(node_1_val) == m_nodes.end())
            {
                m_nodes[node_1_val] = new Node(node_1_val);
            }

            if (m_nodes.find(node_2_val) == m_nodes.end())
            {
                m_nodes[node_2_val] = new Node(node_2_val);
            }

            m_nodes[node_1_val]->addChild(m_nodes[node_2_val], cost);
            m_nodes[node_2_val]->addChild(m_nodes[node_1_val], cost);
        }

        /**
            Performs the computing of the minimum possible cost to go from one node
            to another.

            @param[in] start_node Node we need to start path from.
            @param[in] end_node Node we need to reach with the path.

            @returns The minimal possible cost to go from start_node to end_node, or -1
                     if nodes are not connected.
        **/
        int getMinimumPathCost(int start_node, int end_node)
        {
            if (m_nodes.find(start_node) != m_nodes.end() && m_nodes.find(end_node) != m_nodes.end())
            {
                return m_nodes[start_node]->getOptimalPathCost(end_node);
            }
            else
            {
                return -1;
            }
        }

    private :
        unordered_map<int, Node*> m_nodes;
};


/**
    Main method that solves the global problem.

    @param[in] edges Graph's edges.
    @param[in] A First path node.
    @param[in] B Last path node.

    @returns Minimum possible cost between A and B, or -1 if nodes are not connected.
**/
int beautifulPath(vector<vector<int>> edges, int A, int B) 
{
    Graph graph;

    for (auto v : edges)
    {
        graph.addEdge(v[0], v[1], v[2]);
    }

    return graph.getMinimumPathCost(A,B);
}

// Auto-generated main.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<vector<int>> edges(m);
    for (int i = 0; i < m; i++) {
        edges[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> edges[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string AB_temp;
    getline(cin, AB_temp);

    vector<string> AB = split_string(AB_temp);

    int A = stoi(AB[0]);

    int B = stoi(AB[1]);

    int result = beautifulPath(edges, A, B);

    fout << result << "\n";

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
