def main():
    with open("ex2.input", "r") as f:
        data = f.read()

    buttons = [[7, 8, 9], [4, 5, 6], [1, 2, 3]]
    r = c = 1
    to_press = []
    for char in data:
        if char == "\n":
            to_press.append(buttons[r][c])
            continue
        match char:
            case "U":
                r = r + 1 if r < 2 else 2
            case "D":
                r = r - 1 if r > 0 else 0
            case "L":
                c = c - 1 if c > 0 else 0
            case "R":
                c = c + 1 if c < 2 else 2

    print(to_press)


if __name__ == "__main__":
    main()
