# Process the string going forward, increasing weights of follow-on
# characters as we reach each marker
def process_linear(string: str) -> int:
    weights = [1] * len(string)
    length = 0

    i = 0
    while i < len(string):
        if string[i] == "(":
            start = i + 1
            end = string[start:].find(")") + start
            num, mult = map(int, string[start:end].split("x"))
            next = end + 1
            for j in range(next, next + num):
                weights[j] *= mult
            i = next
        else:
            length += weights[i]
            i += 1

    return length


def main():
    with open("ex9.input", "r") as f:
        input = f.read().strip()

    total_length = process_linear(input)
    print(f"Total length: {total_length}")


if __name__ == "__main__":
    main()
