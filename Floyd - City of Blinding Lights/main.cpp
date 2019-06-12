/**
    This is my solution to the challenge "Floyd : City of Blinding Lights" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/floyd-city-of-blinding-lights/problem
*/

#include <bits/stdc++.h>

using namespace std;

#include <bits/stdc++.h>

using namespace std;

/**
    This class defines a Node for the graph.
**/
class Node
{
    public :
        // A node must have an ID.
        Node() = delete; 

        /**
            Constructor of a node.

            @param[in] id Node's id.
        **/
        Node(int id) :  m_children(), m_node_id(id) {}

        /** 
            Destructor of a node. Nothing special because children pointers are destructed by the graph.
        **/
        virtual ~Node() = default;

        /**
            Adds a connection to another node.

            @param[in] node Child node to make a connection with.
            @param[in] cost Edge cost to child.
        **/
        void addChild(const Node* node, int cost)
        {
            unordered_map<int, pair<const Node*,int>>::iterator it;

            // If an edge already exists, we keep only the one with the best cost
            // since we are in an optimization problem.
            if ((it = m_children.find(node->getID())) != m_children.end())
            {
                
                it->second.second = cost;
                
            }
            else
            {
                m_children[node->getID()] = make_pair(node, cost);
            }
        }

        /**
            Solves the best path in cost from the current node to another one, and returns the cost.
            It uses a dp-optimized Dijkstra algorithm.
          
            @param[in] node_id The node to reach.

            @returns The best cost to reach the node, or -1 if the node is not reachable.
        **/
        int getBestCostToNode(int node_id)
        {
            unordered_map<int, int>::iterator node_search;
            unordered_map<int, unordered_map<int, int>>::iterator opt_search;

            // First DP optimization : if we already did this path search, we return the result we ended up with.
            if ((opt_search = m_nodes_optimized_costs.find(m_node_id)) != m_nodes_optimized_costs.end())
            {
                return ((node_search = opt_search->second.find(node_id)) != opt_search->second.end() ? node_search->second : -1);
            }

            // Dijkstra algorithm uses concentric layers progression around start node.
            unordered_map<int, const Node*> current_layer;
            unordered_map<int, const Node*> next_layer;
            unordered_map<int, int> costs;

            // The first layer is the start node.
            current_layer[m_node_id] = this;

            // We continue till we have all optimal costs and no more nodes 
            // to walk through.
            while (current_layer.size() != 0)
            {
                // filling next layer.
                for (auto p : current_layer)
                {
                    p.second->addChildrenToDijkstraLayer(next_layer, costs);
                }

                // The next layer is the current layer for the next iteration.
                swap(current_layer, next_layer);
                next_layer.clear();
            }

            // In the end, the result is whether the cost for the requested node id in the costs array, or -1 if it is not present (non reachable node).
            int result = ((node_search = costs.find(node_id)) != costs.end() ? node_search->second : -1);

            // We register the computed costs to avoid having to redo all of that over again for this node.
            m_nodes_optimized_costs[m_node_id] = move(costs);

            return result;
        }

        /**
            @returns Node's id.
        **/
        int getID() const
        {
            return m_node_id;
        }

    private :

        /**
            Part of the dijkstra algorithm that fills the next layer with current 
            node children. 

            @param[inout] next_layer The next layer to fill.
            @param[inout] costs Current best costs for all known nodes.
        **/
        void addChildrenToDijkstraLayer(unordered_map<int, const Node*> &next_layer, unordered_map<int, int> &costs) const
        {
            int current_cost = costs[m_node_id];
            unordered_map<int, unordered_map<int, int>>::iterator opt_search;

            // 1 - If we already know the most optimized costs to all other nodes from the current one,
            // we can avoid filling the next layer with children and fill in the costs array with
            // already computed optimized costs.
            if ((opt_search = m_nodes_optimized_costs.find(m_node_id)) != m_nodes_optimized_costs.end())
            {
                for (auto p : opt_search->second)
                {
                    int new_cost = current_cost + p.second;
                    unordered_map<int, int>::iterator it;

                    // already walked node => we consider it only if the new cost is inferior to the previously known one
                    if ((it = costs.find(p.first)) != costs.end()) 
                    {
                        if (new_cost < it->second)
                        {
                            costs[p.first] = new_cost;
                        }
                    }
                    else // new node
                    {
                        costs[p.first] = new_cost;
                    }
                }
            }
            // 2 - Else, we still need to iterate through children to find optimized costs, so 
            // we add them to the next dijkstra layer 
            else
            {
                for (auto p : m_children)
                {
                    int new_cost = current_cost + p.second.second;
                    unordered_map<int, int>::iterator it;

                    // already walked node => we consider it only if the new cost is inferior to the previously known one
                    if ((it = costs.find(p.first)) != costs.end()) 
                    {
                        if (new_cost < it->second)
                        {
                            next_layer[p.first] = p.second.first;
                            costs[p.first] = new_cost;
                        }
                    }
                    else  // new node
                    {
                        next_layer[p.first] = p.second.first;
                        costs[p.first] = new_cost;
                    }
                }
            }
        }

        int m_node_id;

        // DP array that stores optimized paths from one node to all other reachable ones. 
        // It associates the node id to a cost array associating other's node id to their optimized cost.
        static unordered_map<int, unordered_map<int, int>> m_nodes_optimized_costs;

        // Associative map that associates a children node ID to it's object and cost.
        unordered_map<int, pair<const Node*, int>> m_children; 
};

// Static member initialization.
unordered_map<int, unordered_map<int, int>> Node::m_nodes_optimized_costs = unordered_map<int, unordered_map<int, int>>();


/**
    This class defines a graph.
**/
class Graph
{
    public :

        /**
            Default graph constructor.
        **/
        Graph() : m_active_nodes(), m_total_nodes_count(0) {}

        /**
            Main graph constructor.

            @param[in] nodes_count The number of nodes in the graph.
        **/
        Graph(int nodes_count) : m_active_nodes(), m_total_nodes_count(nodes_count) {}

        /** 
            Destructor. Destroys nodes.
        **/
        virtual ~Graph()
        {
            for (auto p : m_active_nodes)
            {
                delete p.second;
            }
        }

        /**
            Adds a new edge in the graph.

            @param[in] node_1 First node.
            @param[in] node_2 Second node.
            @param[in] cost Edge cost.
        **/
        void addEdge(int node_1, int node_2, int cost)
        {
            unordered_map<int, Node*>::iterator it;

            // We add nodes to the active map only once.
            if ((it = m_active_nodes.find(node_1)) == m_active_nodes.end())
            {
                m_active_nodes[node_1] = new Node(node_1);
            }

            if ((it = m_active_nodes.find(node_2)) == m_active_nodes.end())
            {
                m_active_nodes[node_2] = new Node(node_2);
            }

            // Edges are unidirectional.
            m_active_nodes[node_1]->addChild(m_active_nodes[node_2], cost);
        }

        /**
            Computes the best cost to go from one node to another one.

            @param[in] node_1 First node_id.
            @param[in] node_2 Arrival node_id.

            @returns The best possible cost to go from node_1 to node_2.
        **/
        int getBestPathCost(int node_1, int node_2)
        {
            if (node_1 == node_2)
            {
                return 0;
            }
            
            unordered_map<int, Node*>::iterator ita;
            
            Node* start_node;
            
            // If the start node is not isolated (i.e it is in the actives array).
            if ((ita = m_active_nodes.find(node_1)) != m_active_nodes.end())
            {
                start_node = ita->second;
            }
            else
            {
                return -1;
            }


            if ((ita = m_active_nodes.find(node_2)) == m_active_nodes.end())
            {
                return -1;
            }

            return start_node->getBestCostToNode(node_2);
        }

        

    private :
        int m_total_nodes_count;

        // Associates nodes ids to their object representation.
        // This map only contains "active" nodes, i.e the ones that are
        // connected by at least one edge.
        unordered_map<int, Node*> m_active_nodes;
};


// Main function, rewritten with C scanf function to speed up IO time.
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int road_nodes;
    int road_edges;

    Graph graph(road_nodes);

    scanf("%d %d\n", &road_nodes,  &road_edges);

    int a = 0, b = 0, c = 0;
       
    for (int i = 0; i < road_edges; i++) 
    {
        scanf("%d %d %d\n", &a, &b, &c);
        graph.addEdge(a,b,c);
    }

    int q, cost;

    scanf("%d\n", &q);

    for (int i = 0; i < q; i++) 
    {
        scanf("%d %d\n", &a, &b);

        fout << graph.getBestPathCost(a,b) << "\n";
    }

    fout.close();

    return 0;
}
