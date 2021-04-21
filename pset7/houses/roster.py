from sys import argv, exit
import csv
import cs50

if len(argv) != 2:
    print("You should pass an argument.")
    exit(1)

open(f"students.db", "r").close()
db = cs50.SQL("sqlite:///students.db")

query = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

for row in query:
    name = row["first"]

    if row["middle"] != None:
        name += " " + row["middle"] + " " + row["last"]
    else:
        name += " " + row["last"]

    print(name + ",", "born", row["birth"])