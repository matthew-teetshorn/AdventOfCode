import re


def main():
    with open("ex7.input", "r") as f:
        # Supernets are strings outside of brackets, Hypernets are strings inside of brackets
        input = [re.split(r"\[([^\]]+)\]", line.strip()) for line in f]

    # Join supernets and hypernets into their own string groups
    net_groups = [(" ".join(line[::2]), " ".join(line[1::2])) for line in input]
    # Count how many meet the requirements (i.e. 'xyx' ... '[yxy]')
    total = sum(
        any(
            c1 == c3 and c2 != c1 and c2 + c1 + c2 in hypernets
            for c1, c2, c3 in zip(supernets, supernets[1:], supernets[2:])
        )
        for supernets, hypernets in net_groups
    )

    print(total)


if __name__ == "__main__":
    main()
