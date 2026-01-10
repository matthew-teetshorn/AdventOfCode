def cell_eqn(x: int, y: int) -> int:
    MAGIC_NUMBER = 1352
    res = (
        bin((x * x + 3 * x + 2 * x * y + y + y * y) + MAGIC_NUMBER)
        .lstrip("0b")
        .count("1")
    ) % 2

    return res


def print_maze(maze: list[list[int]], path: list[tuple[int, int]]):
    for r in range(len(maze)):
        for c in range(len(maze[0])):
            if (c, r) in path:
                print("O", end="")
            else:
                print("#" if maze[r][c] else ".", end="")
        print()


def is_open(maze: list[list[int]], point: tuple[int, int]) -> bool:
    x = point[0]
    y = point[1]
    if maze[y][x] == 0:
        return True
    else:
        return False


def in_maze(maze: list[list[int]], point: tuple[int, int]) -> bool:
    return (
        True
        if point[0] >= 0
        and point[0] < len(maze[0])
        and point[1] >= 0
        and point[1] < len(maze)
        else False
    )


# BFS Traversal
def traverse(
    maze: list[list[int]],
    parent: list[list[tuple[int, int] | None]],
    visited: list[list[bool]],
    start: tuple[int, int],
    end: tuple[int, int],
) -> bool:
    queue = [start]
    directions = [(-1, 0), (1, 0), (0, 1), (0, -1)]

    while len(queue) != 0:
        curr = queue.pop(0)
        cx, cy = curr[0], curr[1]
        visited[cy][cx] = True
        if (curr) == end:
            return True
        for d in directions:
            dx, dy = d[0], d[1]
            nx, ny = (cx + dx, cy + dy)
            next = (nx, ny)
            if in_maze(maze, next) and is_open(maze, next) and not visited[ny][nx]:
                parent[ny][nx] = curr
                queue.append(next)

    return False


def walk(
    parent: list[list[tuple[int, int] | None]], end: tuple[int, int]
) -> list[tuple[int, int]]:
    ret = [end]
    next = parent[end[1]][end[0]]
    while next is not None:
        ret.append(next)
        next = parent[next[1]][next[0]]

    ret.reverse()
    return ret


def main():
    DIM = 50

    maze = [[cell_eqn(x, y) for x in range(DIM)] for y in range(DIM)]
    visited = [[False for _ in range(DIM)] for _ in range(DIM)]
    parent: list[list[tuple[int, int] | None]] = [
        [None for _ in range(DIM)] for _ in range(DIM)
    ]
    parent[1][1] = None
    start = (1, 1)
    end = (31, 39)

    traverse(maze, parent, visited, start, end)
    path = walk(parent, end)
    print_maze(maze, path)
    print(f"Path found in {len(path) - 1} steps")


if __name__ == "__main__":
    main()
