import sys
import time

NUM_ROWS = 6
NUM_COLS = 50


def fill_rect(x: int, y: int, screen: list[list[int]]) -> list[list[int]]:
    matrix = screen.copy()
    for r in range(y):
        for c in range(x):
            matrix[r][c] = 1

    return matrix


def transpose(matrix: list[list]) -> list[list]:
    new_matrix = [list(row) for row in zip(*matrix)]
    return new_matrix


def rotate(
    rot_type: str, index: int, amount: int, screen: list[list[int]]
) -> list[list[int]]:
    if rot_type == "column":
        matrix = transpose(screen)
    else:
        matrix = screen.copy()

    length = len(matrix[0])
    temp = matrix[index].copy()
    for i in range(length):
        temp[(i + amount) % length] = matrix[index][i]

    matrix[index] = temp.copy()
    if rot_type == "column":
        matrix = transpose(matrix)

    return matrix


def clear(matrix: list[list[int]]):
    for r in range(len(matrix)):
        for c in range(len(matrix[0])):
            matrix[r][c] = 0


def main():
    with open("ex8.input", "r") as f:
        input = [line.strip() for line in f]

    screen: list[list[int]] = [[0 for _ in range(NUM_COLS)] for _ in range(NUM_ROWS)]

    have_written = False
    for line in input:
        cmd = line.split(" ")
        if len(cmd) == 2:
            x, y = map(int, cmd[1].split("x"))
            fill_rect(x, y, screen)
        else:
            rot_type = cmd[1]
            index = int(cmd[2].split("=")[1])
            amount = int(cmd[4])
            screen = rotate(rot_type, index, amount, screen)

        # Move cursor to redraw screen
        if have_written:
            sys.stdout.write(f"\033[{NUM_ROWS}F")

        for r in range(NUM_ROWS):
            for c in range(NUM_COLS):
                char = "#" if screen[r][c] else "."
                print(char, end="")
            print()

        sys.stdout.flush()
        time.sleep(0.05)
        have_written = True

    total = sum(cell for row in screen for cell in row if cell == 1)
    print(total)


if __name__ == "__main__":
    main()
