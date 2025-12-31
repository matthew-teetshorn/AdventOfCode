import re


def main():
    with open("ex9.input", "r") as f:
        input = f.read().strip()

    new_string = ""
    working_list = re.split(r"\(([0-9]+)x([0-9]+)\)", input, maxsplit=1)
    while len(working_list) != 1:
        pretext = working_list[0]
        num_chars = int(working_list[1])
        num_times = int(working_list[2])
        posttext = working_list[3]
        new_string += pretext
        for i in range(num_times):
            new_string += posttext[:num_chars]

        working_list = re.split(
            r"\(([0-9]+)x([0-9]+)\)", posttext[num_chars:], maxsplit=1
        )
    new_string += working_list[0]

    print(len(new_string))


if __name__ == "__main__":
    main()
