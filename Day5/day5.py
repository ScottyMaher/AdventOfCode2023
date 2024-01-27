import re
import numpy as np

with open("./Day5/problem_5_puzzle_input.txt", "r") as f:
    puzzle_input = f.read()


def part1(puzzle_input):
    segments = puzzle_input.split("\n\n")
    seeds = re.findall(r"\d+", segments[0])

    min_location = float("inf")
    for x in map(int, seeds):
        for seg in segments[1:]:
            for conversion in re.findall(r"(\d+) (\d+) (\d+)", seg):
                destination, start, delta = map(int, conversion)
                start_range = start + delta
                if x in range(start, start + delta):
                    x += destination - start
                    break

        min_location = min(x, min_location)

    return min_location


def determineOverlap(interval1, interval2):
    output1, input1, delta1 = interval1
    output2, input2, delta2 = interval2

    input_range_start, input_range_end = input1, input1 + delta1
    output_range_start, output_range_end = output2, output2 + delta2

    intervals = []
    if input_range_end <= output_range_start or input_range_start >= output_range_end:
        return None
    if input_range_start < output_range_start:
        input_range_start = output_range_start
        intervals.append((output_range_start - delta1, input_range_end - delta1))
    intervals.append((output_range_start - delta2, output_range_end - delta2))


def generateNewRangesForNextLayer():
    pass








# Function for Standard preorder traversal
# root needs to be a list of ranges [(1-20), (30-50), etc]
def preorder(test_interval, maps, level):
    # if the list of not empty
    test_interval = list(test_interval)
    if level >= 1:
        return
    if test_interval:
        intervals = []
        for mapping in maps[level]:
            # subRanges = generateNewRangesForNextLayer()
            input_range_start, output_range_start, range = mapping
            input_range_end = input_range_start + range
            difference = output_range_start - input_range_start
            z, y1, dy = mapping
            y2 = y1 + dy
            diff = z - y1

            if test_interval[1] <= input_range_start or input_range_end < test_interval[0]:  # no overlap
                continue
            if test_interval[0] < input_range_start:  # split original interval at y1
                intervals.append((input_range_start, test_interval[1], "=>", input_range_start + difference, test_interval[1] + difference, level))
                # test_interval[0] = input_range_start
            if input_range_end < test_interval[1]:  # split original interval at y2
                intervals.append((test_interval[0], input_range_end, "=>", test_interval[0] + difference, input_range_end + difference, level))
                # test_interval[1] = input_range_end

            # if x2 <= y1 or y2 <= x1:  # no overlap
            #     continue
            # if x1 < y1:  # split original interval at y1
            #     intervals.append((x1, y1, level))
            #     x1 = y1
            # if y2 < x2:  # split original interval at y2
            #     intervals.append((y2, x2, level))
            #     x2 = y2
            # intervals.append(
            #     (x1 + diff, x2 + diff, level + 1)
            # )  # perfect overlap -> make conversion and let pass to next nevel
            # break

        print(intervals, end=" ")
        preorder(intervals, maps, level + 1)


def part2(puzzle_input):
    segments = puzzle_input.split("\n\n")
    intervals = []
    iterations = 0

    for seed in re.findall(r"(\d+) (\d+)", segments[0]):
        x1, dx = map(int, seed)
        x2 = x1 + dx
        intervals.append((x1, x2))

    min_location = float("inf")

    segments.reverse()
    conversionMaps = []
    # all the segments not including the last one
    for seg in segments[:-1]:
        temp_map = []
        for conversion in re.findall(r"(\d+) (\d+) (\d+)", seg):
            destination, start, delta = map(int, conversion)
            temp_map.append([destination, start, delta])
        conversionMaps.append(np.array(temp_map))

    # map1 = []
    # map1 = np.array(map1)

    # # sort the rows in the matrix by the first column
    conversionMaps[0] = conversionMaps[0][conversionMaps[0][:, 0].argsort()]

    # print(conversionMaps)

    # Step 1: Sort the first layer, as this determines the order of exploration of the tree, this only needs to be done once
    # Step 2: Within a for loop, pop the first element and pass it to the reccursive function
    # Step 3: Within the reccursive function, go through next layer (column 0) mappings and find the one that is the lowest (below) and closest to the converted function input start
    # Step 4: Split the input at that point and pass the new interval to the reccursive function
    # Step 5: Repeat until the input interval is empty

    testRange = [0, 12]
    def testReccursiveFunction(prev_input, current_layer):

        if current_layer == 2:
            print("got to the end with: ", prev_input, current_layer)
            if prev_input == [12, 20]:
                print("found match")
                return prev_input
            else:
                print("no match found")
                return False
            
        else:
            while prev_input != []:
                if prev_input[0] < testRange[1]:
                    new_input = [prev_input[0], testRange[1]]
                    prev_input[0] = testRange[1]
                    matchResult = testReccursiveFunction(new_input, current_layer + 1)
                    if matchResult:
                        return matchResult
                elif prev_input[0] >= testRange[1]:
                    new_input = prev_input.copy()
                    prev_input = []
                    matchResult = testReccursiveFunction(new_input, current_layer + 1)
                    if matchResult:
                        return matchResult


    # print("answer is: ", testReccursiveFunction([1, 20], 1))

    def reccursiveFunction(prev_input, current_layer):
        if current_layer == 7:
            print("got to the end with: ", prev_input)
            for interval in intervals:
                if (interval[0] <= prev_input[1] and interval[1] >= prev_input[1]) or (interval[0] <= prev_input[1] + prev_input[2] and interval[1] >= prev_input[1] + prev_input[2]):
                    print("found match", interval)
                    return interval
            print("no match found")
            return
        
        mappings = conversionMaps[current_layer]

        closest_lower_match = [0, 0, 0]
        for test in mappings:
            # print("test: ", test)

            if prev_input[1] == test[0]:
                print("found match")
                break
            elif test[0] < prev_input[1] and test[0] >= closest_lower_match[0]:
                # dont need to check if the (test[0] + test[2]) is less than prev_input[1] since all mappings are connected, therefore we would find a closer one if it existed
                closest_lower_match = test
                continue

        # now need to do some splitting based on the range of the prev_input and the closest_lower_match
        diff = closest_lower_match[1] - closest_lower_match[0]
        if prev_input[1] + prev_input[2] > closest_lower_match[0] + closest_lower_match[2]:
            new_input = np.array([0, prev_input[1] + diff, (closest_lower_match[0] + closest_lower_match[2]) - prev_input[1]])
            # prev_input[2] = (prev_input[1] + prev_input[2]) - (closest_lower_match[0] + closest_lower_match[2])
            # prev_input[1] = closest_lower_match[0] + closest_lower_match[2]
            print("\nnew input1: ", new_input, current_layer)
            # print("\nprev input: ", prev_input)
            return reccursiveFunction(new_input, current_layer + 1)
        elif prev_input[1] + prev_input[2] <= closest_lower_match[0] + closest_lower_match[2]:
            new_input = np.array([0, prev_input[1] + diff, prev_input[2]])
            # prev_input[2] = (closest_lower_match[0] + closest_lower_match[2]) - (prev_input[1] + prev_input[2])
            # prev_input[1] = prev_input[1] + prev_input[2]
            print("\nnew input2: ", new_input, current_layer)
            # print("\nprev input: ", prev_input)
            return reccursiveFunction(new_input, current_layer + 1)

        # print("\nnew input: ", prev_input[1], prev_input[1] + prev_input[2], closest_lower_match[0] + closest_lower_match[2], "\nmin match: ", closest_lower_match)

    
    # reccursiveFunction(conversionMaps[0][0], 1)

    intervals = np.array(intervals)
    intervals = intervals[intervals[:, 0].argsort()]
    # print("\nintervals: \n", intervals, "\n")

    # print(conversionMaps)

    def testMappingDown(test_input, mappingIndexRange=range(6, -1, -1)):
        for i in mappingIndexRange:
            for conversion in conversionMaps[i]:
                destination, start, delta = conversion
                if test_input >= start and test_input < (start + delta):
                    # print("X before conversion: ", test_input)
                    test_input += destination - start
                    # print("X after conversion: ", test_input)
                    break
        print("Final bottom output: ", test_input)
    
    def testMappingUp(test_input, mappingIndexRange=range(0, 7)):
        for i in mappingIndexRange:
            for conversion in conversionMaps[i]:
                destination, start, delta = conversion
                if test_input >= destination and test_input < (destination + delta):
                    # print("X before conversion: ", test_input)
                    test_input += start - destination
                    # print("X after conversion: ", test_input)
                    break
        print("Final top output: ", test_input)

    # print(1108003799-1105083897)

    # print(conversionMaps[0], "\n\n")
    # 2919902

    x = 1108003799
    # testMappingDown(920707452, range(6, -1, -1))

    # testMappingUp(60568880, range(0, 7))

    # print(1184826596 + 60568879)

    map1Ranges = []
    for mapping in conversionMaps[0]:
        map1Ranges.append([mapping[1], mapping[1] + mapping[2] - 1])
    map1Ranges = np.array(map1Ranges)

    # print(map1Ranges, "\n")

    map2Ranges = []
    for mapping in conversionMaps[1]:
        map2Ranges.append([mapping[0], mapping[0] + mapping[2] - 1])
    map2Ranges = np.array(map2Ranges)

    intervals = []
    for i in range(len(map2Ranges)):
        if map2Ranges[i, 1] < map1Ranges[0, 0] or map2Ranges[i, 0] > map1Ranges[0, 1]:
            continue
        elif map2Ranges[i, 1] >= map1Ranges[0, 0]:
            intervals.append(map2Ranges[i])
        elif map2Ranges[i, 0] <= map1Ranges[0, 1]:
            intervals.append(map2Ranges[i])

    # print(np.array(intervals))
    print("\n")



def part3(puzzle_input):
    segments = puzzle_input.split("\n\n")
    intervals = []
    iterations = 0

    for seed in re.findall(r"(\d+) (\d+)", segments[0]):
        x1, dx = map(int, seed)
        x2 = x1 + dx
        intervals.append((x1, x2, 1))

    min_location = float("inf")

    segments.reverse()
    conversionMaps = []
    # all the segments not including the last one
    for seg in segments[:-1]:
        temp_map = []
        for conversion in re.findall(r"(\d+) (\d+) (\d+)", seg):
            destination, start, delta = map(int, conversion)
            temp_map.append([destination, start, delta])
        conversionMaps.append(np.array(temp_map))

    while intervals:
        x1, x2, level = intervals.pop()
        if level == 8:
            min_location = min(x1, min_location)
            continue

        for conversion in re.findall(r'(\d+) (\d+) (\d+)', segments[level]):
            iterations += 1
            z, y1, dy = map(int, conversion)
            y2 = y1 + dy
            diff = z - y1
            if x2 <= y1 or y2 <= x1:    # no overlap
                continue
            if x1 < y1:                 # split original interval at y1
                intervals.append((x1, y1, level))
                x1 = y1
            if y2 < x2:                 # split original interval at y2
                intervals.append((y2, x2, level))
                x2 = y2
            intervals.append((x1 + diff, x2 + diff, level + 1)) # perfect overlap -> make conversion and let pass to next nevel
            break

        else:
            intervals.append((x1, x2, level + 1))
    # print('iterations', iterations)
    return min_location


# print("Part 1:", part1(puzzle_input))
# print("Part 2:", part2(puzzle_input))
part2(puzzle_input)
# print(
#     "\nPart 2:",
#     preorder((30, 47), [[(0, 40, 35), (35, 0, 25), (60, 35, 15)], [(0, 20), (21, 60), (61, 90)]], 0),
# )
