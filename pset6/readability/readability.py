from cs50 import get_string
from sys import exit
import re

text = get_string("Text: ")

letters = len(re.sub(r"[\".,;!?' ]", '', text))
words = len(text.split())
sentences = len(re.split("[.!?].{1}", text))

averageL = (100 / words) * letters
averageS = (100 / words) * sentences

index = 0.0588 * averageL - 0.296 * averageS - 15.8

if (index >= 16):
    print("Grade 16+")
    exit(0)

if (index < 1):
    print("Before Grade 1")
    exit(0)

else:
    print(f"Grade {round(index)}")
    exit(0)

# print("letters", letters)
# print("words", words)
# print("sentences", sentences)