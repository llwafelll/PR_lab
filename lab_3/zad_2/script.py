v = []
with open("log", "r") as f:
    for c, line in enumerate(f):
        if c % 3 == 1:
            v.append(int(line.split()[-1]))

print(*v, end="", sep=", ")
print()

print(f"len of list: {len(v)}")
print(f"len of set : {len(set(v))}")

