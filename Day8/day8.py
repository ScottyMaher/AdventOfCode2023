import re

with open("./Day8/day8.txt", 'r') as file:
    puzzle_input = file.read()

instructions, directions = puzzle_input.split("\n\n")

directionsMap = {}
starting_nodes = []
for direction in directions.split("\n"):
    key = direction[0:3]
    left = direction[7:10]
    right = direction[12:15]
    directionsMap[key] = (left, right)

    if key[2] == 'A':
        starting_nodes.append(key)


def part1():
    current_node = 'AAA'
    steps = 0
    iterations = 0
    while iterations < 100:
        for instruction in instructions:
            if instruction == "L":
                current_node = directionsMap[current_node][0]
            elif instruction == "R":
                current_node = directionsMap[current_node][1]
            steps += 1
            if current_node == 'ZZZ':
                print("You're at the end!", steps)
                iterations = 100
                break

        iterations += 1

def part2():
    current_nodes = starting_nodes
    steps = 0
    iterations = 0
    nodes_length = len(current_nodes)
    while iterations < 1000000:
        for instruction in instructions:
            accumulator = 0
            for i in range(nodes_length):
                if instruction == "L":
                    current_nodes[i] = directionsMap[current_nodes[i]][0]
                elif instruction == "R":
                    current_nodes[i] = directionsMap[current_nodes[i]][1]
                if current_nodes[i][2] == 'Z':
                    accumulator += 1
            # print("current nodes: ", current_nodes)
            if accumulator == 4:
                print("accumulator: ", accumulator)

            if accumulator == nodes_length:
                print("You're at the end!", steps)
                return

            steps += 1

        iterations += 1
    


part2()