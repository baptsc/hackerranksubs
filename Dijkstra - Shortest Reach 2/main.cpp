/**
    This is my solution to the challenge "Dijkstra: Shortest Reach 2" on Hackerrank.

    Challenge link : https://www.hackerrank.com/challenges/dijkstrashortreach/problem
*/

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
                if (it->second.second > cost)
                {
                    it->second.second = cost;
                }
            }
            else
            {
                m_children[node->getID()] = make_pair(node, cost);
            }
        }

        /**
            Finds the most optimal cost to all other reachable nodes with
            a Dijkstra implementation, applied to all other nodes at the same time.

            @returns An associative array that associates all reachable nodes
                     ids from the current node, to their optimal cost.
        **/
        unordered_map<int, int> getBestCostsToAllNodes()
        {
            unordered_map<int, int> result;
            
            // Dijkstra algorithm uses concentric layers progression around start node.
            unordered_map<int, const Node*> current_layer;
            unordered_map<int, const Node*> next_layer;

            // The first layer is the start node.
            current_layer[m_node_id] = this;

            int cost = 0;

            // We continue till we have all optimal costs and no more nodes 
            // to walk through.
            while (current_layer.size() != 0)
            {
                // filling next layer.
                for (auto p : current_layer)
                {
                    cost = p.first == m_node_id ? 0 : result[p.second->getID()];
                    p.second->addChildrenToDijkstraLayer(m_node_id, next_layer, result, cost);
                }

                // The next layer is the current layer for the next iteration.
                swap(current_layer, next_layer);
                next_layer.clear();
            }

            return move(result);
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

            @param[in] start_node Id of the starting node.
            @param[inout] next_layer The next layer to fill.
            @param[inout] costs Current best costs for all known nodes.
            @param[in] current_cost Current best cost to current node.
        **/
        void addChildrenToDijkstraLayer(int start_node, unordered_map<int, const Node*> &next_layer, unordered_map<int, int> &costs, int current_cost) const
        {
            for (auto p : m_children)
            {
                // we never add the starting node to the next layer.
                if (p.first != start_node) 
                {
                    int new_cost = current_cost + p.second.second;
                    unordered_map<int, int>::iterator it;

                    if ((it = costs.find(p.first)) != costs.end()) // already walked node
                    {
                        // We take it into consideration only if we have a better cost
                        // than the already registrered one.
                        if (new_cost < it->second) 
                        {
                            next_layer[p.first] = p.second.first;
                            it->second = new_cost;
                        }
                    }
                    else // new node
                    {
                        next_layer[p.first] = p.second.first;
                        costs[p.first] = new_cost;
                    }
                }
            }
        }


        int m_node_id;

        // Associative map that associates a children node ID to it's object and cost.
        unordered_map<int, pair<const Node*, int>> m_children; 
};

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

            // Edges are bidirectional.
            m_active_nodes[node_1]->addChild(m_active_nodes[node_2], cost);
            m_active_nodes[node_2]->addChild(m_active_nodes[node_1], cost);
        }

        /**
            Gives the most optimal costs from a node to all others in the graph.
            
            @param[in] start_node_id The start node.

            @retruns Optimal costs to all other nodes from the starting one, ordered 
                     by ascending node id number. Non reachable nodes have a cost 
                     equal to -1.
        **/
        vector<int> getBestCostsFromNode(int start_node_id)
        {
            vector<int> results_array(m_total_nodes_count-1);

            unordered_map<int, int>::iterator itr;
            unordered_map<int, Node*>::iterator ita;

            int i = 0;
            int id = 0;
            Node* start_node;
            
            // If the start node is not isolated (i.e it is in the actives array).
            if ((ita = m_active_nodes.find(start_node_id)) != m_active_nodes.end())
            {
                start_node = ita->second;

                // We get best costs to all reachable nodes from the starting one. 
                unordered_map<int, int> linked_nodes_result = start_node->getBestCostsToAllNodes();

                // We complete the result with isolated nodes.
                for (id = 1; id <= m_total_nodes_count; id++)
                {
                    // Starting node must not be part of the result.
                    if (id != start_node_id) 
                    {
                        // If the node is in the reachable nodes returned by the 
                        // getBestCostsToAllNodes() method, we use its computed cost.
                        if ((itr = linked_nodes_result.find(id)) != linked_nodes_result.end()) 
                        {
                            results_array[i] = itr->second;
                        }
                        else // else the node is not reachable, its cost is -1.
                        {
                            results_array[i] = -1;
                        }

                        i++;
                    }
                }
            }
            else // else if the start node is isolated, all costs are -1.
            {
                for (i = 0; i < m_total_nodes_count-1; i++)
                {
                    results_array[i] = -1;
                }
            }

            return move(results_array);
        }

    private :
        int m_total_nodes_count;

        // Associates nodes ids to their object representation.
        // This map only contains "active" nodes, i.e the ones that are
        // connected by at least one edge.
        unordered_map<int, Node*> m_active_nodes;
};

/**
    Main function that solves the problem.
    It passes all test cases.

    @param[in] n Number of nodes.
    @param[in] edges Graph edges.
    @param[in] s Starting node.

    @returns Best costs to all other nodes, ordered by ascending nodes 
             number. Unreachable nodes from start have a cost equal to -1.
**/
vector<int> shortestReach(int n, const vector<vector<int>> &edges, int s) 
{
    // 1 - Constructing graph.
    Graph graph(n);

    for (auto v : edges)
    {
        graph.addEdge(v[0], v[1], v[2]);
    }

    // 2 - Solving costs.
    vector<int> results_array;

    results_array = graph.getBestCostsFromNode(s);

    return move(results_array);
}

/**
    Main function based on starting code.
    I had to rewrite I/O in this main with C scanf method because std::cin was too
    slow for test case #7 (which contains a lot of data to read to contruct a big
    graph).
**/  
int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int t,n,m;
    scanf("%d\n", &t);

    for (int t_itr = 0; t_itr < t; t_itr++) 
    {
        scanf("%d %d\n", &n, &m);

        vector<vector<int>> edges(m);
        
        for (int i = 0; i < m; i++) 
        {
            edges[i].resize(3);

            scanf("%d %d %d\n", &edges[i][0], &edges[i][1], &edges[i][2]);
        }

        int s;

        scanf("%d\n", &s);

        vector<int> result = shortestReach(n, edges, s);

        for (int i = 0; i < result.size(); i++) 
        {
            fout << result[i];

            if (i != result.size() - 1) 
            {
                fout << " ";
            }
        }

        fout << "\n";
    }

    fout.close();

    return 0;
}
