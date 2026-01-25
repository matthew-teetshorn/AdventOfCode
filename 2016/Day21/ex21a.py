class Op:
    def __init__(self, op: str, left: str, right: str):
        self.op = op
        self.left = left
        self.right = right

    def __repr__(self):
        return f"{self.op} {self.left} {self.right}"


def swap(string: str, index_l: int, index_r: int) -> str:
    temp = list(string)
    temp[index_l], temp[index_r] = temp[index_r], temp[index_l]
    return "".join(temp)


def rotate(string: str, index: int) -> str:
    return string[index:] + string[:index]


def reverse(string: str, index_l: int, index_r: int) -> str:
    return (
        string[:index_l]
        + "".join(reversed(string[index_l : index_r + 1]))
        + string[index_r + 1 :]
    )


def move(string: str, index_l: int, index_r: int) -> str:
    letter = string[index_l]
    temp = list(string)
    temp.remove(letter)
    temp.insert(index_r, letter)
    return "".join(temp)


def process_string(ops: list[Op], string: str, unscramble=False) -> str:
    umap = {0: 1, 1: 1, 2: 6, 3: 2, 4: 7, 5: 3, 6: 0, 7: 4}
    map = {0: 1, 1: 2, 2: 3, 3: 4, 4: 6, 5: 7, 6: 0, 7: 1}
    for op in ops:
        match op.op:
            case "rotate":
                index = 0
                if op.left == "index":
                    if unscramble:
                        map = umap
                    index = -map[string.find(op.right)]
                elif op.left == "left":
                    index = int(op.right)
                elif op.left == "right":
                    index = -int(op.right)
                if unscramble:
                    index = -index
                string = rotate(string, index)
            case "swap_letter":
                string = swap(string, string.find(op.left), string.find(op.right))
            case "swap_pos":
                string = swap(string, int(op.left), int(op.right))
            case "reverse":
                string = reverse(string, int(op.left), int(op.right))
            case "move":
                if unscramble:
                    string = move(string, int(op.right), int(op.left))
                else:
                    string = move(string, int(op.left), int(op.right))

    return string


def main():
    ops = []
    with open("ex21.input", "r") as f:
        for line in f:
            op, left, right = line.strip().split()
            ops.append(Op(op, left, right))

    print(f"Part 1: {process_string(ops, 'abcdefgh')}")
    print(f"Part 2: {process_string(ops[::-1], 'fbgdceah', unscramble=True)}")


if __name__ == "__main__":
    main()
