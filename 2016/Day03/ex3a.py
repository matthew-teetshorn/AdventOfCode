def main():
    with open("ex3.input", "r") as f:
        input = f.read()

    num_tris = list(
        map(
            lambda nums: nums[0] + nums[1] > nums[2],
            list(
                sorted(int(number) for number in line.strip().split())
                for line in input.strip().split("\n")
            ),
        )
    ).count(True)
    print(num_tris)


if __name__ == "__main__":
    main()
