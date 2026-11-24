import random, sys
mb = float(sys.argv[1]); out = sys.argv[2]
words = "the around graphics from by be any which various mount lorem ipsum dolor sit amet".split()
size = 0
with open(out, "w") as f:
    while size < mb * 1_000_000:
        w = random.choice(words)
        f.write(w.capitalize() if random.random() < .1 else w)
        f.write(random.choice([" ", ", ", ". ", "\n"]))
        size += len(w) + 1
