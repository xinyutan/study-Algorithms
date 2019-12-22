# python3
from collections import deque
import glob

class Edge:

    def __init__(self, u, v, capacity):
        self.u = u
        self.v = v
        self.capacity = capacity
        self.flow = 0

# This class implements a bit unusual scheme for storing edges of the graph,
# in order to retrieve the backward edge for a given edge quickly.
class FlowGraph:

    def __init__(self, n):
        # List of all - forward and backward - edges
        self.edges = []
        # These adjacency lists store only indices of edges in the edges list
        self.graph = [[] for _ in range(n)]

    def add_edge(self, from_, to, capacity):
        # Note that we first append a forward edge and then a backward edge,
        # so all forward edges are stored at even indices (starting from 0),
        # whereas backward edges are stored at odd indices.
        forward_edge = Edge(from_, to, capacity)
        backward_edge = Edge(to, from_, 0)
        self.graph[from_].append(len(self.edges))
        self.edges.append(forward_edge)
        self.graph[to].append(len(self.edges))
        self.edges.append(backward_edge)

    def size(self):
        return len(self.graph)

    def get_ids(self, from_):
        return self.graph[from_]

    def get_edge(self, id):
        return self.edges[id]

    def add_flow(self, id, flow):
        # To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
        # due to the described above scheme. On the other hand, when we have to get a "backward"
        # edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
        # should be taken.
        #
        # It turns out that id ^ 1 works for both cases. Think this through!
        self.edges[id].flow += flow
        self.edges[id ^ 1].flow -= flow


def read_file_data(filename):
    with open(filename, 'r') as f:
        vertex_cnt, edge_cnt = map(int, f.readline().split())
        graph = FlowGraph(vertex_cnt)
        for line in f:
            u, v, capacity = map(int, line.split())
            graph.add_edge(u - 1, v - 1, capacity)
    return graph



def read_data():
    vertex_count, edge_count = map(int, input().split())
    graph = FlowGraph(vertex_count)
    for _ in range(edge_count):
        u, v, capacity = map(int, input().split())
        graph.add_edge(u - 1, v - 1, capacity)
    return graph


def get_path(graph, from_, to):
    """Return an array of edge ids representing the path"""
    q = deque([from_])
    trace, visited = {}, set([])
    while q:
        u = q.pop()
        visited.add(u)
        for edge_id in graph.get_ids(u):
            e = graph.get_edge(edge_id)
            if e.v not in visited and e.flow < e.capacity:
                if e.v == to:
                    path = [edge_id]
                    p = u
                    while p != from_:
                        p, eid = trace[p]
                        path.append(eid)
                    return path[::-1]

                q.appendleft(e.v)
                trace[e.v] = (u, edge_id)
    return None


def get_min_flow_on_path(graph, path):
    """Return the minimum flow on the path"""
    edges = [graph.get_edge(edge_id) for edge_id in path]
    return min([e.capacity - e.flow for e in edges])


def update_flow(graph, edge_id, flow):
    """add flow to edge edge_id"""
    graph.add_flow(edge_id, flow)
    if edge_id & 1 == 0:
        back_edge = graph.get_edge(edge_id^1)
        back_edge.capacity = graph.get_edge(edge_id).flow

def calculate_flow(graph, from_):
    return sum([graph.get_edge(eid).flow for eid in graph.get_ids(from_)])

def max_flow(graph, from_, to):
    flow = 0
    # your code goes here
    while True:
        path = get_path(graph, from_, to)
        if not path:
            break
        min_flow = get_min_flow_on_path(graph, path)
        for e in path:
            update_flow(graph, e, min_flow)
    flow = calculate_flow(graph, from_)
    return flow


if __name__ == '__main__':
    graph = read_data()
    print(max_flow(graph, 0, graph.size() - 1))
