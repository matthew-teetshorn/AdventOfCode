def manhattan_distance(a: tuple[int, int], b: tuple[int, int]) -> int:
    distance = 0
    for p1, p2 in zip(a, b):
        distance = abs(p1 - p2)
    return distance


def main():
    DIM_Y = 25
    DIM_X = 35
    WALL_VAL = 500

    grid = [["." for _ in range(DIM_X)] for _ in range(DIM_Y)]
    grid[0][DIM_X - 1] = "G"
    grid[0][0] = "D"

    walls: list[tuple[int, int]] = []
    hole: tuple[int, int] = (0, 0)
    with open("ex22.input", "r") as f:
        for line in f:
            x, y, s, u, _, _ = map(int, line.strip().split())
            if s >= WALL_VAL:
                grid[y][x] = "#"
                walls.append((x, y))
            if u == 0:
                grid[y][x] = "E"
                hole = (x, y)

    for row in grid:
        print("".join(row))

    moves = 0
    goal: tuple[int, int] = (DIM_X - 1, 0)
    end: tuple[int, int] = (0, 0)
    queue: list[tuple[tuple[int, int], tuple[int, int], int]] = [(hole, goal, moves)]
    seen: set[tuple[tuple[int, int], tuple[int, int]]] = set()
    seen.add((hole, goal))

    while len(queue) > 0:
        hole, goal, moves = queue.pop(0)
        distance_to_end = manhattan_distance(goal, end)

        if goal == end:
            break

        # Move hole
        x, y = hole
        dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        for dx, dy in dirs:
            nx = x + dx
            ny = y + dy
            # Next point not on grid or is wall
            if nx < 0 or nx == DIM_X or ny < 0 or ny == DIM_Y or (nx, ny) in walls:
                continue
            # Move the hole
            next_hole = (nx, ny)
            # Move the goal if hole hits it
            next_goal = goal
            if (nx, ny) == goal:
                next_goal = hole
            # Only explore unique configurations of the grid which don't move goal farther away
            if (next_hole, next_goal) not in seen and manhattan_distance(
                next_goal, end
            ) <= distance_to_end:
                queue.append((next_hole, next_goal, moves + 1))
                seen.add((next_hole, next_goal))

    print(moves)


if __name__ == "__main__":
    main()
