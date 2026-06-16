#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct GraphNode {
    std::string id;
    std::string name;
    std::string type;
};

struct GraphEdge {
    std::string to;
    double      weight;
};

class DeliveryGraph {
private:
    std::unordered_map<std::string, GraphNode>              nodes;
    std::unordered_map<std::string, std::vector<GraphEdge>> adj;

    bool dfsHelper(const std::string& cur, const std::string& dst,
                   std::unordered_set<std::string>& visited,
                   std::vector<std::string>& path);

public:
    void addNode(const std::string& id, const std::string& name,
                 const std::string& type);
    void addEdge(const std::string& from, const std::string& to, double weight);

    std::pair<std::unordered_map<std::string, double>,
              std::unordered_map<std::string, std::string>>
    dijkstra(const std::string& src);

    std::vector<std::string> getPath(
        const std::unordered_map<std::string, std::string>& prev,
        const std::string& src, const std::string& dst);

    std::vector<std::string> bfs(const std::string& src,
                                 const std::string& dst);
    std::vector<std::string> dfs(const std::string& src,
                                 const std::string& dst);

    double pathDistance(const std::vector<std::string>& path);

    void printGraph();
    void printPath(const std::vector<std::string>& path, double totalDist = -1);

    std::unordered_map<std::string, GraphNode>& getNodes();
};
