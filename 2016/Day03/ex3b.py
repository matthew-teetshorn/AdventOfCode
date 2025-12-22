def main():
    with open("ex3.input", "r") as f:
        input = f.read()

    col_sides = list(
        int(item)
        for tup in zip(*(line.strip().split() for line in input.strip().split("\n")))
        for item in tup
    )

    sides_per_tri = 3
    valid_tri_count = 0
    for i in range(0, len(col_sides), sides_per_tri):
        sides = sorted(col_sides[i : i + sides_per_tri])
        if sides[0] + sides[1] > sides[2]:
            valid_tri_count += 1

    print(valid_tri_count)


if __name__ == "__main__":
    main()
