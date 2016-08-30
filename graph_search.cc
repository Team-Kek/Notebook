constexpr Point adjacencies[] = {{1,0},{-1,0},{0,-1},{0,1}}; // CHANGEME: based on needs

bool GraphSearch(Map m, Point src, Point dest)
{
  std::map<Point, Point> predecessor;
  Queue<Node> q; // CHANGEME: Queue type, for e.g. bfs vs dijkstras.
  q.push({src, NullPoint, 0}); // CHANGEME: based on node impl
  while (!q.empty()) {
    Node n = pop(q); // CHANGEME based on queue type
    Point curr = n.curr; // CHANGEME: based on node impl
    if (map[curr.x][curr.y] == 'X' || predecessor.count(curr)) continue; // CHANGEME: based on map details
    predecessor[curr] = n.pre;
  if (curr == dest) break;
    for (const auto &adj : adjacencies) { // CHANGEME: based on adjacencies.
      Point next = curr + adj;
      // CHANGEME: optionally store this distance somewhere.
      double dist = std::max(1, map[next.x][next.y] - map[curr.x][curr.y]);
      q.push({next, curr, n.cost + dist, n.cost + dist + h(next), ts++});
    }
  }
  // CHANGEME: based on goals
  if (!predecessor.count(dest)) return false;
  for (Point p = dest; p != NullPoint; p = predecessor[p]) {
    // Step back through predecessors.
  }
  return true;
}
