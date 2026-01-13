def calc_checksum(string: str) -> str:
    ret_str = ""
    for i in range(0, len(string), 2):
        if string[i] == string[i + 1]:
            ret_str += "1"
        else:
            ret_str += "0"
    return ret_str


def main():
    with open("ex16.input", "r") as f:
        bin_string = f.read().strip()

    # Choose which part to calculate:
    PART1 = False
    if PART1:
        TARGET = 272
    else:
        TARGET = 35651584

    while len(bin_string) < TARGET:
        end_string = bin_string.replace("1", "a")
        end_string = end_string.replace("0", "1")
        end_string = end_string.replace("a", "0")
        bin_string = bin_string + "0" + end_string[::-1]

    bin_string = bin_string[:TARGET]

    checksum = calc_checksum(bin_string)
    while len(checksum) % 2 == 0:
        checksum = calc_checksum(checksum)

    print(checksum)


if __name__ == "__main__":
    main()
