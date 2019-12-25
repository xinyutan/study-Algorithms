# python3
from collections import deque

class MaxMatching:
    def read_data(self):
        n, m = map(int, input().split())
        adj_matrix = [list(map(int, input().split())) for i in range(n)]
        return adj_matrix

    def write_response(self, matching):
        line = [str(-1 if x == -1 else x + 1) for x in matching]
        print(' '.join(line))

    def construct_graph(self, adj_matrix, n, m):
        graph_size = m + n + 2
        graph = {i: set([]) for i in range(graph_size)}
        for i in range(n):
            for j in range(m):
                if adj_matrix[i][j]:
                    u, v = i + 1, j + n + 1
                    graph[u].add(v)
                    graph[0].add(u)
                    graph[v].add(graph_size - 1)
        return graph

    def find_path_bfs(self, graph, from_, to):
        q = deque([from_])
        trace, visited = {}, set([])
        path = []
        while q:
            u = q.pop()
            visited.add(u)
            if u == to:
                path.append(u)
                while u != from_:
                    u = trace[u]
                    path.append(u)
                return path[::-1]
            for v in graph[u]:
                if v not in visited:
                    trace[v] = u
                    q.appendleft(v)

        return path

    def find_path_dfs(self, graph, from_, to):
        trace = []
        visited = [False] * len(graph)

        def dfs(u, trace):
            if u == to:
                trace.append(u)
                return True
            visited[u] = True
            trace.append(u)
            for v in graph[u]:
                if not visited[v]:
                    if dfs(v, trace): return True
            visited[u] = False
            trace.pop()
            return False

        dfs(from_, trace)
        return trace

    def update(self, graph, adj_matrix, path, n, m, matching):
        for i in range(0, len(path)-1):
            u, v = path[i], path[i+1]
            if u == 0 or v == n + m + 1:
                graph[u].remove(v)
                graph[v].add(u)
                continue

            if 1 <= u <= n and n+1 <= v <= n+m:
                u_, v_ = u - 1, v - n - 1
                matching[u_] = v_
            else:
                u_, v_ = u - n - 1, v - 1
                matching[v_] = -1
            graph[u].remove(v)
            graph[v].add(u)

    def find_matching(self, adj_matrix):
        # Replace this code with an algorithm that finds the maximum
        # matching correctly in all cases.
        n = len(adj_matrix)
        m = len(adj_matrix[0])
        matching = [-1] * n
        graph = self.construct_graph(adj_matrix, n, m)
        while True:
            path = self.find_path_bfs(graph, 0, len(graph)-1)
            if not path:
                break
            self.update(graph, adj_matrix, path, n, m, matching)
        return matching

    def solve(self):
        adj_matrix = self.read_data()
        matching = self.find_matching(adj_matrix)
        self.write_response(matching)


if __name__ == '__main__':
    max_matching = MaxMatching()
    max_matching.solve()

