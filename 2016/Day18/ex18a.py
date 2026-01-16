def main():
    # Decide on Part 1 or Part 2
    PART1 = False
    if PART1:
        NUM_ROWS = 40
    else:
        NUM_ROWS = 400000

    rows = [[0]]  # Left and right walls are always 0
    with open("ex18.input", "r") as f:
        rows[0].extend([0 if c == "." else 1 for c in f.read().strip()])
        rows[0].append(0)  # Left and right walls are always 0

    trap_rules = (
        (1, 1, 0),
        (0, 1, 1),
        (1, 0, 0),
        (0, 0, 1),
    )

    row_index = 1
    for i in range(NUM_ROWS - 1):
        rows.append([0])  # place left wall
        p_row = rows[row_index - 1]
        for j in range(1, len(rows[0]) - 1):  # skip left and right walls
            rule = (p_row[j - 1], p_row[j], p_row[j + 1])
            if rule in trap_rules:
                rows[row_index].append(1)
            else:
                rows[row_index].append(0)
        rows[row_index].append(0)  # place right wall
        row_index += 1

    safe_tiles = 0
    for row in rows:
        safe_tiles += row.count(0) - 2

    print(f"Number of safe tiles in room: {safe_tiles}")


if __name__ == "__main__":
    main()
