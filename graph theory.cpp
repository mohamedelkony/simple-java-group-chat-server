#include<iostream>
#include <limits>
#include<vector>
#include<string>
#include<array>
#include<new>
#include<cassert>
#include<stdexcept>
#include<memory>
#include<sstream>
#include<iomanip>
#include<list>
#include<algorithm>
#include<fstream>
#include<map>
#include<set>
#include<bitset>
#include<regex>
#include<random>
#include<unordered_set>
#include<unordered_map>
#include<stack>
#include<queue>
#include<cstdlib>
#include<functional>
//#include"indexed_priority_queue.h"
#include<cstdlib>
#define pnt_xy pair<int,int>

using namespace std;

class graph
{
	struct edge
	{
		int to; int cost;
		edge(int _to, int _cost = 0) :to{ _to }, cost{ _cost }{}
		edge();
		edge(initializer_list<int> l) :to{ *l.begin() }, cost{ *(l.begin() + 1) }{}
	};

	int vertices_count;
	vector<vector<edge>> edges;
	bool directed;
	
	void DFS_util(int, vector<bool>&)const;

	void toplogical_sort_utill(int, vector<int>&, stack<int>&)const;
	void bridges_dfs(int at, int parent, vector<bool>& visited, vector<int>& ids, vector<int>& low, vector<pair<int, int>>& ans)const;
	void ap_dfs(int root,int at, int parent, vector<bool>& visited, vector<int>& ids, vector<int>& low, vector<int>& ans,int& out_edges_of_root)const;

public:
	
	graph(int n, bool _directed ) :vertices_count{ n }{
		edges = vector<vector<edge>>(vertices_count, vector<edge>{});
		directed = _directed;
	}

	/*
	//copy constructor
	graph(const graph& ref) : vertices_count{ ref.vertices_count }
	{
		this->directed = ref.directed;
		this->edges = vector<vector<edge>>(vertices_count);
		for (int i = 0; i < vertices_count; i++)
		{
			//memcpy(edges + i, ref.edges + i, sizeof(ref.edges + i));
			for (edge e : ref.edges[i])
			{
				this->edges[i].push_back(e);
			}
		}
	}

	//copy assignment
	graph& operator=(const graph& ref) 
	{
		delete[] this->edges;

		this->directed = ref.directed;
		this->vertices_count = ref.vertices_count;

		this->edges = new vector<edge>[vertices_count]{};
		for (int i = 0; i < vertices_count; i++)
		{
			//memcpy(edges + i, ref.edges + i, sizeof(ref.edges + i));
			for (edge e : ref.edges[i])
			{
				this->edges[i].push_back(e);
			}
		}
		return *this;
	}
	//move constructor
	graph(graph&& ref) :vertices_count{ref.vertices_count}
	{
		this->directed = ref.directed;
		this->edges = ref.edges;
		ref.edges = nullptr;
	}
	//move assignment
	graph& operator=(graph&& ref)
	{
		this->directed = ref.directed;
		vertices_count = ref.vertices_count;
		this->edges = ref.edges;
		ref.edges = nullptr;
		
		return *this;
	}
	int size()const
	{
		return vertices_count;
	}
	vector<edge>& operator[](int i)
	{
		return edges[i];
	}
	~graph()
	{
		if (edges)
			delete[] edges;
	}
	*/

	void add_edge(int, int, int);
	void print()const;
	
	void DFS(int)const;
	void DFS_iterative(int)const;
	vector<vector<int>> breadth_first_search(int, vector<int>&)const;
	vector<int> shortest_unweighted_path(int, int)const;
	static vector <pair<int,int>>maze_solver(vector < vector<char>> maze, pair<int, int> start, pair<int, int>end);
	vector<int> toplogical_sort()const;
	vector<int> lazy_dijkstra_algorithm(int source, int target, vector<int>& path );
	vector<int> dijkestra_algorithm(int source, int target ) const;
	vector<pair<int, int>> bridges()const;
	vector<int> articulation_points()const;
	
};

//pair.first : x value of vertex
//pair.second : y value of vertex
vector<pair<int, int>>graph::maze_solver(vector < vector<char>> maze, pair<int, int> start, pair<int, int>end)
{
	int xd[4]{0, 0, -1, 1}; int yd[4]{1, -1, 0, 0};
	int maze_width = maze[0].size(); int maze_hieght = maze.size();
	
	vector<vector<pnt_xy>> parents;

	//verteices that were reached before via other vertices
	//i.e. it's shortest path has been known 
	//but their nieghbours still hasn't been added yet 
	vector < vector<bool>>visited;

	for (int i = 0; i < maze_hieght; i++)
	{
		visited.push_back(vector<bool>(maze_width, false));
		parents.push_back(vector<pnt_xy>(maze_width, { -1, -1 }));
	}

	queue<int> x_que, y_que;
	visited[start.second][start.first] = true;
	x_que.push(start.first); y_que.push(start.second);
	
	while (!x_que.empty())
	{
		int crnt_x = x_que.front(); int crnt_y = y_que.front();
		if (crnt_x == end.first && crnt_y == end.second)break;
		y_que.pop(); x_que.pop();
		for (int i = 0; i < 4; i++)
		{
			int adj_x = crnt_x + xd[i]; int adj_y = crnt_y+yd[i];
			
			if (adj_x < 0 || adj_y < 0)continue;
			if (adj_x >= maze_width || adj_y >= maze_hieght)continue;

			if (maze[adj_y][adj_x] == '#')continue;

			if (!visited[adj_y][adj_x])
			{
				x_que.push(adj_x); y_que.push(adj_y);
				parents[adj_y][adj_x] = { crnt_x, crnt_y };
				visited[adj_y][adj_x] = true;
			}
		}
	}
	if (parents[end.second][end.first].first == -1)
		return vector<pair<int, int>>{};

	vector<pnt_xy> path;

	//iterate until reach node has no parent i.e.{-1.-1}
	//which may be the start point or dead end point
	for (pnt_xy crnt = end; crnt.first != -1; crnt = parents[crnt.second][crnt.first])
		path.push_back(crnt);

	std::reverse(path.begin(), path.end());

	if (path[0] == start)
		return path;
	return vector<pnt_xy>{};
}

vector<vector<int>> graph::breadth_first_search(int source, vector<int>& parents )const
{
	vector<int> ans; ans.reserve(vertices_count);
	
	vector<vector<int>> layers;
	
	vector<bool> visited(vertices_count, false);
	
	visited[source] = true;
	queue<int> q;
	int vertices_left_in_layer = 1;
	int next_layer_count = 0;
	q.push(source);
	vector<int> layer;
	while (!q.empty())
	{
		int vertex = q.front(); q.pop();
		ans.push_back(vertex);
		
		if (vertices_left_in_layer == 0)
		{vertices_left_in_layer = next_layer_count; next_layer_count = 0;
		layers.push_back(layer);
		layer.clear();
		}	
		vertices_left_in_layer--;
		
		for (auto edg : this->edges[vertex])
		{
			if (!visited[edg.to])
			{
				visited[edg.to] = true;
				q.push(edg.to);
				next_layer_count++;
				layer.push_back(edg.to);
				if (!parents.empty())
					parents[edg.to] = vertex;
			}
			
		}
	}
	return layers;
}

vector<int> graph::shortest_unweighted_path(int from, int to)const
 {
	vector<int> parnets(vertices_count, -1);
	this->breadth_first_search(from, parnets);
	vector<int> ans;
	//traget =-1 if it was the source or it has no parent (not reachable)
	for (int target = to; target != -1; target = parnets[target])
		ans.push_back(target);
	
	std::reverse(ans.begin(), ans.end());
	if (ans[0] == from)//when (ans[0]!= from) it's not reachable
		return ans;
	return vector<int>{};
}

void graph::add_edge(int from, int to, int cost=0)
{
	edges[from].push_back({ to, cost });
	if (!directed)
		edges[to].push_back({ from, cost });
}

void graph::print()const
{
	for (int i = 0; i < vertices_count; i++)
	{
		cout << i << " -> [";
		for (int k = 0; k < edges[i].size(); k++)
			cout << edges[i][k].to <<", ";
		cout <<  "]"<<endl;
	}
}

void graph::DFS_util(int from, vector<bool>& visited) const
{
	if (visited[from])return;
	visited[from] = true;
	cout << from<<" -> ";
	for (auto edg : edges[from])
	{
		DFS_util(edg.to, visited);
	}
}
void graph::DFS(int from)const
{
	vector<bool>visited(this->vertices_count);
	DFS_util(from, visited);
}
void graph::DFS_iterative(int source) const
{
	stack<int> s;
	s.push(source);
	vector<bool>visited(vertices_count,false);
	while (!s.empty())
	{
		int vertex = s.top(); s.pop();
		if (visited[vertex])continue;
		visited[vertex] = true;
		cout << vertex << " -> ";
		auto node_edegs = edges[vertex];
		for (auto edg : node_edegs)
		{
			s.push(edg.to);
		}
	}
}


struct min_priority_queue_comper
{
	bool operator()(pair<int, int> p1, pair<int, int> p2)
	{
		return p1.second > p2.second;//min heap
	}
};

//O(E*log(V))
//suits non-negative weighted grahps
vector<int> graph::lazy_dijkstra_algorithm(int source, int target, vector<int>& path )
{
	vector<bool> visited(this->vertices_count, false);
	vector<int> best_cost(this->vertices_count, numeric_limits<int>().max());
	priority_queue < pair<int, int>, vector<pair<int, int>>, min_priority_queue_comper> q;
	
	q.push({ source, 0 });
	best_cost[source] = 0;
	vector<int>parents = vector<int>(this->vertices_count, -1);
	while (!q.empty())
	{
		int crnt_node = q.top().first;
		int crnt_cost = q.top().second;
		q.pop();

		if (best_cost[crnt_node] < crnt_cost)continue;//skip duplicated outdated pairs (due to lazy implementation)
		visited[crnt_node] = true;//visited nodes are nodes that their final best_cost have been known
		if (crnt_node == target)break;

		for (auto edg : edges[crnt_node])
		{
			if (visited[edg.to])continue;
			int this_cost = best_cost[crnt_node] + edg.cost;
			if (this_cost < best_cost[edg.to])
			{
				parents[edg.to] = crnt_node;//crnt_node is a candidate best parent
				best_cost[edg.to] = this_cost;
				q.push({ edg.to, this_cost });
			}
		}
	}
	path.clear();
	if (target != -1)
	{
		if (best_cost[target] != numeric_limits<int>().max())
		{
			
			for (int prnt = target; prnt != -1; prnt = parents[prnt])
				path.push_back(prnt);
		}
		std::reverse(path.begin(), path.end());
	}
	return best_cost;
}

//eager implemantation using indexed priority queue
// time complextiiy(E*log(V))
vector<int> graph::dijkestra_algorithm(int source,int target=-1)const
{
	return vector<int>{};
	/*
	vector<bool> visited(vertices_count, false);
	vector<int> best_cost(vertices_count, numeric_limits<int>().max());
	indexed_priority_queue ipq;

	best_cost[source] = 0;
	ipq.insert(source, 0);
	
	while (!ipq.empty())
	{
		int crnt_vertx = ipq.get_min(); 
		int crnt_cost = ipq.get_key_priority(crnt_vertx);
		ipq.extract_min();
		
		if (crnt_vertx == target)break;
		
		visited[crnt_vertx] = true;//visited nodes are nodes that their final best_cost have been known
				
		for (auto edg : this->edges[crnt_vertx])
		{
			if (visited[edg.to])continue;
			int this_cost = crnt_cost + edg.cost;
			if (this_cost < best_cost[edg.to])
			{
				best_cost[edg.to] = this_cost;
				//contains_key O(1)
				if (ipq.contains_key(edg.to))
				{
					//decrease_key O(log[ipq.count])
					ipq.decrease_key(edg.to, this_cost);
				}
				else
				{
					//insert O(log[ipq.count])
					ipq.insert(edg.to, this_cost);
				}

			}
		}
	}
	return best_cost;
	*/
	}

void graph::toplogical_sort_utill(int vertex, vector<int>& visited, stack<int>& explored_stack)const
{
	//base conditon (i.e. add vertex to stack and exit)
	//all of this vertex neighbours had been visited
	//or this vertex has no neighbours.
	
	visited[vertex] = 1;
	for (auto child:edges[vertex])
	{
		if (visited[child.to]==2)continue;
		if (visited[child.to] == 1){
		//child & child.to edge forms cycle
		//toplogical sort dose't exist
			continue;
		}
		toplogical_sort_utill(child.to, visited, explored_stack);
	}
	explored_stack.push(vertex);
	visited[vertex] = 2;
}

/*for DAG 
foreach directed edge u to v : u should come before v.
 no unique solution
*/
vector<int> graph::toplogical_sort()const
{
	stack<int> ans;//or use array and reverse it in the end;

	//0 -> not vistewd at all
	//1 -> visited it but haven't visited all vertices in its subtree
	//2 -> visited it and visited all vertices in its subtree and left it
	//ref http://codeforces.com/blog/entry/4907

	vector<int>visited(vertices_count, 0 );
	for (int i = 0; i < vertices_count; i++)
	{
		if (!visited[i])
		{
			toplogical_sort_utill(i, visited, ans);
		}
	}
	vector<int> v;
	while (!ans.empty())
	{
		v.push_back(ans.top());
		ans.pop();
	}
	return v;
}


void solvemaze(string s)
{
char ch;
	vector<vector<char>> maze; pair<int, int> start, end;
	fstream fin(s, fstream::in);
	int row = 0;
	bool fistchar = true;
	while (fin >> noskipws >> ch) {
		if (ch == '\n')
			{ row++; fistchar = true; }
		else if (fistchar)
			{
			maze.push_back(vector<char>{ ch }); fistchar = false;
			}
		else
		{
			if (ch != EOF)
			{
				maze[row].push_back(ch); fistchar = false;
				if (ch == 'S')
				{
					start.first = maze[row].size()-1;
					start.second = row;
				}
				else if (ch == 'E')
				{
					end.first = maze[row].size() - 1;
					end.second = row;
				}
			}
		}
	}
	
	for (auto d : maze)
	{
		for (char c : d)
			cout << c;
		cout << endl;
	}
	cout << endl;

	auto ans = graph::maze_solver(maze, start,end );
	for (auto x : ans)
	{
		if (x == start || x == end)continue;
		maze[x.second][x.first] = '@';
	}
	for (auto d : maze)
	{
		for (char c : d)
			cout << c;
		cout << endl;
	}
	cout << endl;
}

void graph::bridges_dfs(int at, int parent, vector<bool>& visited, vector<int>& ids, vector<int>& low,vector<pair<int,int>>& ans)const
{
	/*
	*low:lowest id that node could reach from another path exculding the path it was discoverd from
	*initialze visited and low with time of discover
	*Don't skip visited so,we indicate reaching them from another parent(way)
	*so we update the lowest id that parent can reach.
	*/

	static int id=0;
	visited[at] = true; ids[at] = low[at]=id++;
	
	for (auto i : edges[at])
	{
		int to = i.to;
		//don't dicover the path it came from 
		if (to == parent)continue;

		if (visited[to])
		{
			//node to was reached second time via another path
			low[at] = std::min(low[at], ids[to]);
		}
	    else
		{
			graph::bridges_dfs(to, at, visited, ids, low,ans);
			//after finishing [to] has a its final low value
			//so update value of parent e.g. at 
			low[at] = std::min(low[at], low[to]);
			if (low[to] >ids[at])
				ans.push_back({ at, to });
		}
	}
}
//source https://www.youtube.com/watch?v=aZXi1unBdJA

//O(V+E)
//for undirected & directed graphs
vector<pair<int,int>> graph::bridges()const
{
	vector<bool> visited(this->vertices_count);
	vector<pair<int, int>> ans;
	vector<int> low(this->vertices_count),id(this->vertices_count);
	
	for (int i = 0; i < vertices_count; i++)
	{
		if (!visited[i])
		{
			bridges_dfs(i, -1, visited, id, low,ans);			
		}
	}

	return ans;
}

void graph::ap_dfs(int root,int at, int parent, vector<bool>& visited, vector<int>& ids, vector<int>& low, vector<int>& ans,int& out_edges_of_root)const
{
	/*
	*low:lowest id node can reach from another path exculding the path it was discoverd from
	*initialze visited and low with nnumber of discover
	*Don't skip vivsted so,we indicate reahing them from another parent(way)
	*so we updated lowest id parent can reach.
	*/	static int id = 0;
	visited[at] = true; ids[at] = low[at] = id++;
	if (root == parent)out_edges_of_root++;
	for (auto i : edges[at])
	{
		int to = i.to;
		//don't dicover the path it came from 
		if (to == parent)continue;

		if (visited[to])
		{
			//to was reached second time via onther path
			low[at] = std::min(low[at], ids[to]);
		}
		else
		{
			graph::ap_dfs(root,to, at, visited, ids, low, ans,out_edges_of_root);
			//after finishing [to] has a its final low value
			//so update value of parent e.g. at 
			low[at] = std::min(low[at], low[to]);
			//== in case of cycle (and not part of bridge)
			//< in case of part of bridge(it will be part of cycle also)
			if (ids[at] <= low[to])
				ans[at] = true;
		}
	}
}

//O(V+E)
//for undirected & directed graphs
vector<int> graph::articulation_points()const
{
	vector<bool> visited(this->vertices_count);
	vector<int> ans(this->vertices_count);
	vector<int> low(this->vertices_count), id(this->vertices_count);
	
	for (int i = 0; i < vertices_count; i++)
	{
		int out_edges_of_root = 0;
		if (!visited[i])
		{
			ap_dfs(i,i, -1, visited, id, low, ans,out_edges_of_root);
			ans[i] = (out_edges_of_root>1);
		}
	}

	return ans;
}

//for all graphs
//warning: fuction dosent check if all vetrices are connected(no isolated graphs)! but
//when used in finding euler path this condtion is eaiser to be checked there.
bool has_euler_path(vector<int>& in,vector<int>& out)
{
	int odd_count = 0;
	for (int i = 0; i < in.size(); i++)
	{
		int diff = abs(in[i] - out[i]);
		if (diff == 1)
			odd_count++;
		else if(diff>1)
			return false;
	}
	if (odd_count ==0 ||odd_count==2)
		return true;
	return false;
}
void dfs_euler(int at, vector<vector<int>>& edges, vector<int> in, vector<int>&out, list<int>&path)
{
	while (out[at] != 0)
	{
		dfs_euler(edges[at][--out[at]],edges,in,out,path);
	}
	path.push_front(at);
}

//for all graphs
list<int> euler_path(vector<vector<int>>& edges)
{
	int n = edges.size();
	vector<int> in(n, 0); vector<int> out(n, 0);
	list<int> path;
	int edges_count = 0;

	//fill number of in & out edges for each vertex 
	for (int at = 0; at < n; at++)
	{
		for (int to : edges[at])
		{
			edges_count++;
			out[at]++;
			in[to]++;
		}
	}

	if (!has_euler_path(in,out))
		return list<int>();

	//get first node to start from. any node is valid except if 
	//node is isolated i.e. has no incoming or outgoing edges [graph is still euler] or 
	//when graph has vertex with (in - out) edges=1 then path must start from that vertex.
	int first_vertex = -1;
	for (int i = 0; i < n; i++)
	{
		if (out[i] - in[i] == 1)
		{
			first_vertex = i; break;
		}
		if (out[i]>0)first_vertex = i;
	}
	
	dfs_euler(first_vertex, edges, in, out, path);

	if (path.size()  == edges_count+1)
		return path;
	return list<int>{};
}
void dfs_scc_grid(int w, int h, pnt_xy at, vector<vector<int>>& low, vector<vector<int>>& ids, int& scc_count, int& id, stack<pnt_xy>& stk, vector<vector<bool>>& on_stack)
{
	//dfs is guranted to be called only on unvivsted nodes
	stk.push(at);
	int x = at.first; int y = at.second;
	on_stack[y][x] = true;
	ids[y][x] = low[y][x] = id++;

	vector<pnt_xy> edgs;
	//generate edges here:
	/*
	int x1, y1;
	if (hss[at.second] == '>')
		x1 = at.first + 1;
	else
		x1 = at.first - 1;
	if (wss[at.first] == 'v')
		y1 = at.second + 1;
	else
		y1 = at.second - 1;
	if (x1 >= 0 && x1 < w)
		edgs.push_back({ x1, at.second });
	if (y1 >= 0 && y1 < h)
		edgs.push_back({ at.first, y1 });
	*/
	for (pnt_xy to : edgs)
	{
		//Case1(Tree Edge) : If node to is not visited already, then after DFS of to is complete
		//then minimum of low[at] and low[to] will be updated to low[at].
		if (ids[to.second][to.first] == -1)
		{
			dfs_scc_grid(w, h, to, low, ids, scc_count, id, stk, on_stack);
			low[at.second][at.first] = std::min(low[to.second][to.first], low[at.second][at.first]);
		}
		//Case 2 (Back Edge) : When child to is already visited
		//To make sure, we don’t consider cross edges, when we reach a node which is already visited
		//we should process the visited node only if it is present in stack
		else if (on_stack[to.second][to.first])
		{
			low[at.second][at.first] = std::min(ids[to.second][to.first], low[at.second][at.first]);
		}
	}

	if (ids[y][x] == low[y][x])
	{
		pnt_xy c;
		//contents of stack are nodes of a strong connected component
		while (stk.top() != at)
		{
			c = stk.top();
			///	cout << c.first << "," << c.second << " ";
			on_stack[c.second][c.first] = false;
			stk.pop();
		}
		c = stk.top();
		//	cout << c.first << ", " << c.second << "\n";
		on_stack[c.second][c.first] = false;
		stk.pop();
		scc_count++;
	}
}

void dfs_scc(int n, vector<vector<int>>& edgs, int at, vector<int>& low, vector<int>& ids, int& scc_count, int& id, stack<int>& stk, vector<bool>& on_stack)
{
	//dfs is guranted to be called only on unvivsted nodes
	stk.push(at);
	on_stack[at] = true;
	ids[at] = low[at] = id++;

	for (int to : edgs[at])
	{
		if (ids[to] == -1)
		{
			dfs_scc(n, edgs, to, low, ids, scc_count, id, stk, on_stack);
			low[at] = std::min(low[to], low[at]);
		}
		else if (on_stack[to])
		{
			low[at] = std::min(ids[to], low[at]);
		}

	}

	if (ids[at] == low[at])
	{
		int top;
		while (stk.top() != at)
		{
			top = stk.top();
			on_stack[top] = false;
			stk.pop();
		}
		top = stk.top();
		on_stack[top] = false;
		stk.pop();
		scc_count++;
	}

}
//for directed graph
//O(V+E)
int strongly_connected_compnents(int count,vector<vector<int>> edgs)
{
	vector<int> ids(count, -1);
	vector<int> low(count, -1);
	vector<bool> on_stack(count, false);
	int scc_count = 0;
	int id = 0;
	stack<int> stk;

	for (int i = 0; i < count; i++)
	{
		if (ids[i] == -1/*not vivsted*/)
			dfs_scc(count, edgs, i, low, ids, scc_count, id, stk, on_stack);
	}
	return scc_count;
}


typedef pair < int, long long> wighted_edge;
struct edge{
	int from, to, cost;
	edge(int f, int t, long long c) : from{ f }, to{ t }, cost{ c }{}
};
struct mst_comper{
	bool operator()(edge a, edge b)
	{
		return a.cost > b.cost;
	}
};

//lazy prim algorithm
//for undirected graph
//O((E+V)log(V))
int minimum_spanning_tree(int nodes_count, vector < vector <wighted_edge >> edges)
{
	int min_cost = 0; int edges_count = 0; 
	int mst_edges_count = nodes_count - 1;
	vector<bool> visited(nodes_count, false);
	priority_queue<edge, vector<edge>,mst_comper> q;//min heap
	
	for (wighted_edge w : edges[0])
	{
		q.push({ 0, w.first, w.second });
	}
	visited[0] = true;
	
	while (!q.empty())
	{
		edge e = q.top(); q.pop();
		if (visited[e.to])continue;
		min_cost += e.cost;
		edges_count++;
		visited[e.to] = true;
	///	cout << "{" << e.from << "," << e.to << ":"<< e.cost << "}"<<endl;
		for (wighted_edge w : edges[e.to])
		{
			if (visited[w.first])continue;
			q.push({ e.to, w.first, w.second });
		}
	}
	if (edges_count != nodes_count - 1)return -1;//mst dosen't exist due to existence of isolated components
	return min_cost;
}

int main() {
	int v, e;
	cin >> v >> e;
	vector < vector <wighted_edge >> g(v, vector<wighted_edge>{});
	for (int i = 0; i<e; i++)
	{
		int f, t; long long c; cin >> f >> t >> c;
		g[f-1].push_back({ t-1, c });
		g[t - 1].push_back({ f - 1, c });
	}
cout <<	minimum_spanning_tree(v, g);
	//cin >> v;
	return 0;
}

graph DAG_sample()
{
	graph g(13, true);
	g.add_edge(0, 1);
	g.add_edge(1, 2);
	g.add_edge(1, 5);
	g.add_edge(2, 4);
	g.add_edge(2, 3);
	g.add_edge(5, 4);
	g.add_edge(0, 6);
	g.add_edge(6, 5);
	g.add_edge(7, 8);
	g.add_edge(7, 9);
	g.add_edge(10, 11);
	g.add_edge(10, 0);
	g.add_edge(9, 0);
	g.add_edge(10, 9);
	g.add_edge(11, 12);
	g.add_edge(11, 2);
	g.add_edge(12, 2);
	g.add_edge(8, 0);
	return g;
}
graph sample1()
{
	graph g(21,false);
	g.add_edge(0, 1);
	g.add_edge(0, 2);
	g.add_edge(0, 12);;
	g.add_edge(2, 15);
	g.add_edge(2, 3);
	g.add_edge(1, 17);
	g.add_edge(17, 3);
	g.add_edge(15, 16);
	g.add_edge(3, 18);
	g.add_edge(18, 4);
	g.add_edge(8, 14);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(4, 7);
	g.add_edge(4, 9);
	g.add_edge(7, 8);
	g.add_edge(8, 9);
	g.add_edge(9, 10);
	g.add_edge(9, 11);
	g.add_edge(12, 13);
	g.add_edge(13, 14);
	g.add_edge(20, 19);
	g.add_edge(11, 17);
	g.add_edge(13, 18);
	g.add_edge(16, 0);
	g.add_edge(1, 15);
	g.add_edge(3, 5);
	return g;
}
graph sample_fo_AP()
{
	graph g(9,false);
	g.add_edge(0, 1);
	g.add_edge(1, 2);
	g.add_edge(0, 2);

	g.add_edge(2, 5);
	g.add_edge(2, 3);
	g.add_edge(3, 4);

	g.add_edge(5, 6);
	g.add_edge(6, 7);
	g.add_edge(7, 8);
	g.add_edge(5, 8);
	return g;
}
graph directed_wighted_sample1()
{
	graph g(6, true);
	g.add_edge(0, 1, 5);
	g.add_edge(0, 2, 1);
	g.add_edge(1, 3, 3);
	g.add_edge(1, 2, 2);
	g.add_edge(2, 1, 3);
	g.add_edge(1, 4, 20);
	g.add_edge(2, 4, 12);
	g.add_edge(3, 5, 6);
	g.add_edge(3, 4, 2);
	g.add_edge(3, 2, 3);
	g.add_edge(4, 5, 1);
	return g;
}

typedef unsigned long long int big_int;
typedef enum _types{ nlog_t, nsquared_t, n_t, of_one_t, logn_t }types;
typedef struct _big_o{ int val; types type; }big_o;

int time_analysis_comper(const void* p1,const void* p2)
{
	return ((big_o*)p1)->val - ((big_o*)p2)->val;
}

string big_o_tostr(big_o b){

	string s;
	switch (b.type)
	{
	case nlog_t:
		s = "nlogn";
		break;
	case logn_t:
		s = "logn";
		break;
	case nsquared_t:
		s = "nsquare";
		break;
	case of_one_t:
		s = "1";
		break;
	case n_t:
		s = "n";
		break;
	}
	return s;
}
void analyize_time(big_int n, big_int compares_count)
{
	/*float nsquared_cof = compares_count / (long double)(n*n);
	float nlogn_cof = compares_count / (n*log2(n));
	float n_cof = compares_count / (long double)n;
	float logn_cof = compares_count / (log2(n));
	*/
	//printf(">>>Time (%llu) complexity :%0.3f logn, %0.3f n ,%0.3f nlog2n ,%0.3f n^2\n",compares_count,logn_cof, n_cof, nlogn_cof, nsquared_cof);

	big_o o1 = { abs((long)(compares_count - (n*n))), nsquared_t };
	big_o o2 = { abs(compares_count - (n*log2(n))), nlog_t };
	big_o o3 = { abs((long)(compares_count - n)), n_t };
	big_o o4 = { abs((long)(compares_count - 1)), of_one_t };
	big_o o5 = { abs(compares_count - (log2(n))), logn_t };

	big_o os[5]; os[0] = o1; os[1] = o2; os[2] = o3; os[3] = o4; os[4] = o5;
	qsort(os, 5, sizeof(big_o), time_analysis_comper);
	printf("%llu comparsions done for a queue of size %d\n", compares_count, n);
	printf("  >> its bestfit is O(%s),2nd bestfit is O(%s)\n\n", big_o_tostr(os[0]).c_str(), big_o_tostr(os[1]).c_str());
}
/*
void test1()
{
	while (1)
	{

		printf("enter items count");
		int n = 10e4;
		scanf("%d", &n);

		indexed_priority_queue q;
		for (int i = 0; i<n - 1; i++)
			q.insert( i, -i + 1);

		q.insert( 25, -1050);
		printf("\n 1 item  insertion analysis into queue\n", n);
		analyize_time(n, q.comparsions);


		printf("ipq_decrease_key(q, 3, -1000);\n");
		q.decrease_key( 3, -1000);
		analyize_time(n, q.comparsions);


		//ipq_print(q);
		printf("get_min() =%d\n", q.get_min());
		analyize_time(n, q.comparsions);

		//ipq_print(q);
		printf("ipq_extract_min(); = %d\n", q.extract_min());
		analyize_time(n, q.comparsions);

		printf(" ipq_get_key_priority(3); = %d \n", q.get_key_priority( 3));
		analyize_time(n,q.comparsions);

		printf("ipq_contains_key(20); = %d \n", q.contains_key( 20));
		analyize_time(n, q.comparsions);

		printf("queue is being heap sorted\n");
		auto v = q.get_sorted_keys();
		analyize_time(n, q.comparsions);

		printf("print queue?"); int er;
		scanf("%d", &er);
		if (er)
			q.ipq_print();
		
	}
}
void test2()
{
	indexed_priority_queue q;


	q.insert(5, 50);q.ipq_print();
	q.insert(4, 40);q.ipq_print();
	q.insert(3, 30);q.ipq_print();
	q.insert(2, 20);q.ipq_print();
	q.insert(1, 10);q.ipq_print();

	printf("decrease_key 5 to -1000\n");
	q.decrease_key(5, -1000);
	q.ipq_print();

	//ipq_print(q);
	printf("get_min() =%d\n", q.get_min());

	printf("ipq_extract_min(); = %d\n", q.extract_min());


	printf(" ipq_get_key_priority(3); = %d \n", q.get_key_priority(3));

	printf("ipq_contains_key(20); = %d \n", q.contains_key(20));
	printf("ipq_contains_key(4); = %d \n", q.contains_key(4));

	q.insert(9, 90);
	q.ipq_print();
	q.insert(7, 70);
	cout << "inserted {7,70] ,{9,90}" << endl;
	q.ipq_print();

}
*/

vector<int> dsu_ids;
vector<int> dsu_size;
int dsu_compnents;
void initilize_dsu(int n)
{
	for (int i = 0; i < n; i++)
	{
		dsu_ids.push_back(i);
		dsu_size.push_back(1);
	}
	dsu_compnents = n;
}
int dsu_find(int id)
{
	int root = id;
	while (root != dsu_ids[root])
	{
		root = dsu_ids[root];
	}
	//path comperssion
	while (root!=id)
	{
		int next = dsu_ids[id];
		dsu_ids[id] = root;
		id = next;
	}
	return root;
}
void unify(int id1,int id2)
{
	int root1 = dsu_find(id1);
	int root2 = dsu_find(id2);

	if (root1 == root2)return;
	//Union by size 
	if (dsu_size[root1] > dsu_size[root2])
	{
		dsu_size[root1] += dsu_size[root2];
		dsu_ids[root2] = root1;
	}
	else{
		dsu_size[root2] += dsu_size[root1];
		dsu_ids[root1] = root2;
	}
	dsu_compnents--;
}


int main1()
{
	if (0)cout << "A"; cout << "$";

	auto g = sample_fo_AP();

	cout << "1 ->add edge 2->print 3->DFS,5->BFS,6->shortest path,7->maze solver,8->toplogical sort,\
		9,10->lazy/eager dijkestra,11->birdges,12->articulation points,13->SCC,14->euler path";
	vector<vector<int>> edgs1(8, vector < int>{});
	edgs1[0] = { 1 };
	edgs1[1] = { 2 };
	edgs1[3] = {4,7};
	edgs1[4] = { 5 };
	edgs1[5] = {0,6};
	edgs1[6] = {0,2,4};
	edgs1[7] = {5,3};
	edgs1[2] = {  };

	vector<vector<int>> edgs2(5, vector < int>{});
	edgs2[0] = { 1 };
	edgs2[1] = { 3,2 };
	edgs2[3] = { 4 };
	edgs2[2] = { 1 };


	while (1)
	{
		
		int k; cin >> k;
		if (k == 0)
		{
			cout << "enter smaple number";
			int x; cin >> x;
			if (x == 0)
				g = sample1();
			else if (x == 1)
				g = DAG_sample();
		}
		if (k == 1)
		{
			cout << "enter vertices";
			int i, j; cin >> i >> j;
			g.add_edge(i, j);
		}
		else if (k == 2)
		{
			g.print();
		}
		else if (k == 3)
		{
			g.DFS(0);
		}
		else if (k == 4)
		{
			g.DFS_iterative(0);
		}
		else if (k == 5)
		{
			int i; cout << "enter vertex number"; cin >> i;
			vector<int> aaa;
			auto v = g.breadth_first_search(i,aaa);
			int n = 1;
			for (auto layer : v)
			{
				cout << "layer " << n << " :";
				for (auto i : layer)
					cout << i << " -> ";
				cout << endl;
				n++;
			}
			
		}
		else if (k == 6)
		{
			cout << "enter from to values"; int from,to; cin >>from>> to;
			auto v = g.shortest_unweighted_path(from, to);
			for (auto i : v)
				cout << i << " -> ";
			cout << endl;
			if (v.empty())
				cout << "NO path";
		}
		else if (k == 7)
		{
			solvemaze("maze2.txt");
		}
		else if (k == 8)
		{
			auto v = g.toplogical_sort();
			for (auto i : v)
				cout << i << " ->";
			cout << endl;
		}
		else if (k == 9)
		{
			vector<int>parents;
			auto v = g.lazy_dijkstra_algorithm(0,-1,parents);
			cout << "for source vertex 0 \n";
			for (int i = 0; i < v.size(); i++)
				cout << "{ " << i << "cost = " << v[i] << "}, ";
			cout << endl;
		}
		else if (k == 10)
		{
			cout << "enter from andd to";
			int f, t; cin >> f>>t;
			auto v = g.dijkestra_algorithm(f,t);
			cout << "for source vertex 0 \n";
			for (int i = 0; i < v.size(); i++)
				cout << "{ " << i << "cost = " << v[i] << "}, ";
			cout << endl;

		}
		else if (k == 11)
		{
			auto v = g.bridges();
			for (int i = 0; i < v.size(); i++)
				cout << "{ " << v[i].first << " ," <<v[i].second << "}, ";
			cout << endl;
		}
		else if (k == 12)
		{
			auto v = g.articulation_points();
			for (int i = 0; i < v.size(); i++)
			{
				if (v[i])
					cout << i << endl;
			}
		}
		else if (k == 13)
		{
			cout << "scc=" << strongly_connected_compnents(8, edgs1)<<endl;
		}
		else if (k == 14)
		{
			cout << "euler path ";
			auto path = euler_path(edgs2);
			for (int x : path)
				cout << " ->" << x;
		}
	}
	return 0;
}