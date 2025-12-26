def main():
    input = []
    with open("ex6.input", "r") as f:
        for line in f:
            input.append(line.strip())

    letters = [{}, {}, {}, {}, {}, {}, {}, {}]
    for line in input:
        for i in range(len(line)):
            curr_char = line[i]
            letters[i][curr_char] = letters[i].get(curr_char, 0) + 1

    print(":::Most frequent:::")
    for i in range(len(letters)):
        print(sorted(letters[i].items(), key=lambda item: -item[1])[0][0], end="")
    print("\n")
    print(":::Least frequent:::")
    for i in range(len(letters)):
        print(sorted(letters[i].items(), key=lambda item: item[1])[0][0], end="")
    print("\n")


if __name__ == "__main__":
    main()
