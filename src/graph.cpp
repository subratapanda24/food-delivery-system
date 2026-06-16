#include "../include/graph.h"
#include "../include/ui.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

void DeliveryGraph::addNode(const string& id, const string& name, const string& type) {
    nodes[id] = {id, name, type};
}

void DeliveryGraph::addEdge(const string& from, const string& to, double weight) {
    adj[from].push_back({to,   weight});
    adj[to].push_back({from, weight});
}

// Dijkstra
pair<unordered_map<string,double>, unordered_map<string,string>>
DeliveryGraph::dijkstra(const string& src) {
    unordered_map<string,double> dist;
    unordered_map<string,string> prev;

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
        dist[it->first] = numeric_limits<double>::infinity();
    dist[src] = 0.0;

    priority_queue<pair<double,string>,
                   vector<pair<double,string>>,
                   greater<pair<double,string>>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        double d = pq.top().first;
        string u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        for (size_t i = 0; i < adj[u].size(); i++) {
            const GraphEdge& e = adj[u][i];
            double alt = dist[u] + e.weight;
            if (alt < dist[e.to]) {
                dist[e.to] = alt;
                prev[e.to] = u;
                pq.push({alt, e.to});
            }
        }
    }
    return {dist, prev};
}

vector<string> DeliveryGraph::getPath(
    const unordered_map<string,string>& prev,
    const string& src, const string& dst)
{
    vector<string> path;
    string cur = dst;
    while (!cur.empty()) {
        path.push_back(cur);
        auto it = prev.find(cur);
        cur = (it != prev.end()) ? it->second : "";
    }
    reverse(path.begin(), path.end());
    if (path.empty() || path[0] != src) return {};
    return path;
}

// BFS
vector<string> DeliveryGraph::bfs(const string& src, const string& dst) {
    unordered_map<string,string> parent;
    unordered_set<string>        visited;
    queue<string>                q;

    q.push(src);
    visited.insert(src);
    parent[src] = "";

    while (!q.empty()) {
        string cur = q.front(); q.pop();
        if (cur == dst) break;
        for (size_t i = 0; i < adj[cur].size(); i++) {
            const string& nb = adj[cur][i].to;
            if (!visited.count(nb)) {
                visited.insert(nb);
                parent[nb] = cur;
                q.push(nb);
            }
        }
    }

    vector<string> path;
    string cur = dst;
    while (!cur.empty()) {
        path.push_back(cur);
        auto it = parent.find(cur);
        cur = (it != parent.end()) ? it->second : "";
    }
    reverse(path.begin(), path.end());
    if (path.empty() || path[0] != src) return {};
    return path;
}

// DFS
bool DeliveryGraph::dfsHelper(const string& cur, const string& dst,
                               unordered_set<string>& visited,
                               vector<string>& path)
{
    visited.insert(cur);
    path.push_back(cur);
    if (cur == dst) return true;
    for (size_t i = 0; i < adj[cur].size(); i++) {
        const string& nb = adj[cur][i].to;
        if (!visited.count(nb))
            if (dfsHelper(nb, dst, visited, path)) return true;
    }
    path.pop_back();
    return false;
}

vector<string> DeliveryGraph::dfs(const string& src, const string& dst) {
    unordered_set<string> visited;
    vector<string>        path;
    dfsHelper(src, dst, visited, path);
    return path;
}

double DeliveryGraph::pathDistance(const vector<string>& path) {
    double total = 0;
    for (int i = 0; i < (int)path.size() - 1; i++) {
        for (size_t j = 0; j < adj[path[i]].size(); j++) {
            if (adj[path[i]][j].to == path[i+1]) {
                total += adj[path[i]][j].weight;
                break;
            }
        }
    }
    return total;
}

void DeliveryGraph::printGraph() {
    cout << "\n";
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const string&    id   = it->first;
        const GraphNode& node = it->second;
        string col = (node.type == "restaurant") ? CYAN :
                     (node.type == "hub")        ? WHITE : GREEN;
        cout << "  " << col << BOLD << "[" << id << "] " << node.name << RESET;
        cout << DIM << "  (" << node.type << ")" << RESET << "\n";
        for (size_t i = 0; i < adj[id].size(); i++) {
            cout << "      --> " << adj[id][i].to
                 << "  (" << fixed << setprecision(1) << adj[id][i].weight << " km)\n";
        }
    }
}

void DeliveryGraph::printPath(const vector<string>& path, double totalDist) {
    if (path.empty()) { printError("No path found."); return; }
    for (int i = 0; i < (int)path.size(); i++) {
        auto it = nodes.find(path[i]);
        if (it == nodes.end()) continue;
        const GraphNode& n = it->second;
        string col = (n.type == "restaurant") ? CYAN :
                     (n.type == "hub")        ? WHITE : GREEN;
        cout << col << BOLD << n.name << RESET;
        if (i < (int)path.size() - 1) cout << " --> ";
    }
    cout << "\n";
    if (totalDist >= 0)
        cout << "  " << "Total distance: " << fixed << setprecision(2)
             << totalDist << " km\n";
}

unordered_map<string,GraphNode>& DeliveryGraph::getNodes() { return nodes; }
