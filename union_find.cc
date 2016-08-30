#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <vector>

std::map<int, std::set<int>>
FindConnectedComponents(const std::vector<std::set<int>> &edges)
{
  std::vector<int> parent(edges.size());
  std::iota(parent.begin(), parent.end(), 0);
  std::function<int(int)> GetParent = [&GetParent, &parent](int i) -> int {
    if (parent[i] == i) return i;
    return parent[i] = GetParent(parent[i]);
  };
  std::function<void(int,int)> UnionEdge = [&GetParent, &parent](int i, int j) {
    parent[GetParent(j)] = GetParent(i);
  };
  for (unsigned i = 0; i < parent.size(); i++) {
    for (auto j : edges[i]) {
      UnionEdge(i, j);
    }
  }
  std::map<int, std::set<int>> connected_components;
  for (unsigned i = 0; i < parent.size(); i++) {
    connected_components[GetParent(i)].insert(i);
  }
  return connected_components;
}
