# Unique to my input
NUM_BOTS = 210
NUM_OUTPUTS = 21
LOW_SEARCH = 17
HIGH_SEARCH = 61


class Bot:
    def __init__(self, index: int):
        self.index = index
        self.low = 0
        self.high = 0
        self.isReady = False

    def receive(self, value: int):
        if value < self.high:
            self.low = value
        else:
            self.low = self.high
            self.high = value

        self.low, self.high = sorted([self.low, self.high])

        if self.low != 0 and self.high != 0:
            self.isReady = True

    def giveLow(self) -> int:
        ret = self.low
        self.low = 0
        self.isReady = False
        return ret

    def giveHigh(self) -> int:
        ret = self.high
        self.high = 0
        self.isReady = False
        return ret

    def __repr__(self) -> str:
        return f"Bot #{self.index:03}: Holding: Low: {self.low:2}, High: {self.high:2}, isReady:{self.isReady}"


def main():
    with open("ex10.input", "r") as f:
        input = [
            line.split() for line in sorted((line.strip() for line in f), reverse=True)
        ]

    init = [item for item in input if item[0] == "value"]
    cmds = sorted(
        (item for item in input if item[0] == "bot"), key=lambda item: int(item[1])
    )

    bots = [Bot(i) for i in range(NUM_BOTS)]
    outputs = [0 for _ in range(NUM_OUTPUTS)]
    for instr in init:
        value = int(instr[1])
        dest = int(instr[5])
        bots[dest].receive(value)

    bot = next((bot for bot in bots if bot.isReady), None)
    while bot is not None:
        cmd = cmds[bot.index]
        if bot.low == LOW_SEARCH and bot.high == HIGH_SEARCH:
            print(bot)
            print(" ".join(cmd))
        # bot XX gives low to bot/output YY and high to bot/output ZZ
        low_dest_type = cmd[5]
        low_dest_index = int(cmd[6])
        high_dest_type = cmd[10]
        high_dest_index = int(cmd[11])

        if low_dest_type == "bot":
            bots[low_dest_index].receive(bot.giveLow())
        else:
            outputs[low_dest_index] = bot.giveLow()

        if high_dest_type == "bot":
            bots[high_dest_index].receive(bot.giveHigh())
        else:
            outputs[high_dest_index] = bot.giveHigh()

        bot = next((bot for bot in bots if bot.isReady), None)
    print(f"Outputs[0..2] mult: {outputs[0] * outputs[1] * outputs[2]}")


if __name__ == "__main__":
    main()
