from sys import argv, exit
import csv
import time

if len(argv) != 3:
    print("You should pass two arguments.")
    exit(1)

csv_file = open(argv[1])
database = list(csv.reader(csv_file))

txt_file = open(argv[2])
dna_sequence = txt_file.read()

strs = database[0][1:len(database)]

pattern_count = {}

print("DNA sequence: ", dna_sequence)
time.sleep(1)
print("STRs to search:", end=" ")
for s in strs:
    print(s, end=", ")
print()
print()
time.sleep(1)

for pattern in strs:
    print("DNA pattern", pattern, end=" ")
    # time.sleep(1)
    pattern_count[pattern] = 0
    sequence_counter = 0
    index = 0
    while index < len(dna_sequence):
        amount = 0
        while dna_sequence[index:len(pattern) + index] == pattern:
            amount += 1
            index += len(pattern)
        if amount > pattern_count[pattern]:
            pattern_count[pattern] = amount
        index += 1
    print("Appears", pattern_count[pattern], "times in the sequence.")
    time.sleep(1)
print()
# print(pattern_count)

for person in database[1:len(database)]:
    # print(person)
    equal_strs = 0
    person_nums = person[1:len(person)]

    i = 0

    while i < len(person_nums):
        print(f"{person[0]} has the {strs[i]} pattern {person_nums[i]} times against {pattern_count[strs[i]]} found.")
        time.sleep(1)
        if(pattern_count[strs[i]] == int(person_nums[i])):
            equal_strs += 1
        i += 1
    if equal_strs == (len(database[0]) - 1):
        print()
        print("The DNA sequence belongs to: ", person[0])
        exit(0)
    print()

print("No match")
