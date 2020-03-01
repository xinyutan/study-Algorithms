# python3
import sys


class Node:
    def __init__(self):
        self.next = {letter: None for letter in 'ACTG$'}


def build_suffix_tree(text):
    """
    Build a suffix tree of the string text and return a list
    with all of the labels of its edges (the corresponding
    substrings of the text) in any order.
    """
    result = []
    # Implement this function yourself
    root = Node()
    edges = {}
    for i in range(len(text)):
        curr_node = root
        walk_down = i
        to_split = False
        while curr_node.next[text[walk_down]] is not None:
            next_node = curr_node.next[text[walk_down]]
            start, length = edges[(curr_node, next_node)]
            for j in range(start, start + length):
                if text[j] == text[walk_down]:
                    walk_down += 1
                else:
                    to_split = True
                    break
            if to_split:
                break
            else:
                curr_node = next_node
        if not to_split:
            curr_node.next[text[walk_down]] = Node()
            edges[(curr_node, curr_node.next[text[walk_down]])] = [walk_down, len(text)-walk_down]
        else:
            split_node = Node()
            new_node = Node()
            del edges[(curr_node, next_node)]
            curr_node.next[text[start]] = split_node
            split_node.next[text[j]] = next_node
            split_node.next[text[walk_down]] = new_node
            edges[(curr_node, split_node)] = [start, j - start]
            edges[(split_node, next_node)] = [j, length - (j-start)]
            edges[(split_node, new_node)] = [walk_down, len(text) - walk_down]

    for edge in edges:
        start, length = edges[edge]
        result.append(text[start:start+length])

    return result


if __name__ == '__main__':
    text = sys.stdin.readline().strip()
    result = build_suffix_tree(text)
    print("\n".join(result))


