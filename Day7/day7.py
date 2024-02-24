import re

with open("./Day7/day7.txt", 'r') as file:
    puzzle_input = file.read()

hands = puzzle_input.split("\n")
# hands_bets = re.findall(r"\d+", hands)
print(hands)

ranked_hands = {'five_of_a_kind': [], 'four_of_a_kind': [], 'full_house': [], 'three_of_a_kind': [], 'two_pair': [], 'one_pair': [], 'high_card': []}
for hand in hands:
    hand = hand.split(" ")
    set(hand)
    if hand[1]:
        print(hand)


answer = sum(ranked_hands)
print("Total winnings: ", answer)