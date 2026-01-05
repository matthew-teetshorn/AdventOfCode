def map_register(value: str | None) -> tuple[int, bool]:
    match value:
        case "a":
            return 0, True
        case "b":
            return 1, True
        case "c":
            return 2, True
        case "d":
            return 3, True
        case _:
            return -1, False


def main():
    with open("ex12.input", "r") as f:
        cmds = [line.strip().split() for line in f]

    # Change this to eval Day12 part 2
    part = 2
    cmd_index = 0
    registers = [0, 0, 0, 0]
    if part == 2:
        registers[2] = 1

    while cmd_index < len(cmds):
        instr = cmds[cmd_index][0]
        lvalue = cmds[cmd_index][1]
        rvalue = cmds[cmd_index][2] if len(cmds[cmd_index]) == 3 else None

        lindex, lvalid = map_register(lvalue)
        # In given architecture rindex is always valid register addr
        rindex, _ = map_register(rvalue)
        match instr:
            case "cpy":
                if lvalid:
                    registers[rindex] = registers[lindex]
                else:
                    registers[rindex] = int(lvalue)
            case "inc":
                registers[lindex] += 1
            case "dec":
                registers[lindex] -= 1
            case "jnz":
                if registers[lindex] != 0:
                    cmd_index += int(rvalue) - 1

        cmd_index += 1

    print(registers[0])


if __name__ == "__main__":
    main()
