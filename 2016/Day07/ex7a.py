import re


def main():
    must_have = r"(.)(?!\1)(.)\2\1"
    cant_have = r"\[[^\]]*(.)(?!\1)(.)\2\1[^\]]*\]"

    input = []
    with open("ex7.input", "r") as f:
        for line in f.readlines():
            input.append(line.strip())

    count = 0
    for line in input:
        if (
            re.search(must_have, line) is not None
            and re.search(cant_have, line) is None
        ):
            count += 1

    print(count)


if __name__ == "__main__":
    main()
