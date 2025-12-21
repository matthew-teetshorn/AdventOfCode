def main():
    with open("ex2.input", "r") as f:
        data = f.read()

    # fmt: off
    buttons = [
        [None, None, None, None, None, None, None],
        [None, None, None,  "D", None, None, None],
        [None, None,  "A",  "B",  "C", None, None],
        [None,  "5",  "6",  "7",  "8",  "9", None],
        [None, None,  "2",  "3",  "4", None, None],
        [None, None, None,  "1", None, None, None],
        [None, None, None, None, None, None, None],
    ]
    # fmt: on

    r = 3
    c = 1
    to_press = []
    for char in data:
        if char == "\n":
            to_press.append(buttons[r][c])
            continue
        match char:
            case "U":
                r = r + 1 if buttons[r + 1][c] is not None else r
            case "D":
                r = r - 1 if buttons[r - 1][c] is not None else r
            case "L":
                c = c - 1 if buttons[r][c - 1] is not None else c
            case "R":
                c = c + 1 if buttons[r][c + 1] is not None else c

    print(to_press)


if __name__ == "__main__":
    main()
