# PADP Lab - Compile / Run / Input Guide (Exp 1-7)

All commands tested on Ubuntu 24.04, GCC 13.3, OpenMPI. Run from the folder containing the .c files.

## One-time setup (lab machine)

```
sudo apt install build-essential libgd-dev openmpi-bin libopenmpi-dev
```
Check: `gcc --version`, `mpicc --version`, `ls /usr/include/gd.h`.
If no sudo access: ask the lab admin; Exp 4 needs libgd, Exp 1(MPI)/5 need MPI.

Core count of the lab machine: `nproc`. Use it to choose thread counts.

---

## Exp 1 - Pi by Monte Carlo

### OpenMP - exp1_pi_omp.c
```
gcc -fopenmp exp1_pi_omp.c -o exp1
export OMP_NUM_THREADS=4
./exp1
```
Input (prompted): `100000000`
Output format:
```
Enter the number of iterations used to estimate pi: 100000000
# of trials = 100000000, threads = 4, estimate of pi = 3.14160x
Time taken = x.xxxxxx s
```
Fill the table (threads 1,2,4,8 x sizes 1e6, 1e7, 5e7, 1e8):
```
for n in 1000000 10000000 50000000 100000000; do
  for t in 1 2 4 8; do
    echo "n=$n t=$t: $(echo $n | OMP_NUM_THREADS=$t ./exp1 | grep Time)"
  done
done
```

### MPI - exp1_pi_mpi.c
```
mpicc exp1_pi_mpi.c -o exp1_mpi
mpirun -np 4 ./exp1_mpi 10000000
```
Input: iteration count as the argument (default 10000000 if omitted).
If "not enough slots": `mpirun --oversubscribe -np 4 ./exp1_mpi 10000000`
Output:
```
# of trials = 10000000, processes = 4, pi = 3.14xxxx, time = 0.xxxxxx s
```

---

## Exp 2 - Matrix multiplication (OpenMP)

exp2_matmul.c (compile WITHOUT -O flags)
```
gcc -fopenmp exp2_matmul.c -o exp2
./exp2
```
Input (prompted): `N threads`
- Part (a), small correctness check: `3 4`
- Part (b) / table: `400 4`

Output for `3 4` (A, B are random; A x B must match hand calculation):
```
A:
   3   7   3
...
A x B:
   59   40   82
...
N = 3, threads = 4, time (with private) = 0.0001xx s
With private(i,j,k): CORRECT
Without scope : x of 9 elements wrong
```
Output for `400 4`:
```
N = 400, threads = 4, time (with private) = 0.xxxxxx s
With private(i,j,k): CORRECT
Without scope : <large number> of 160000 elements wrong
```
The "Without scope" count is a race: it differs every run. Needs >= 2 threads and a multi-core CPU to be large. With 1 thread it is 0. For N=3 it may also be 0; use N=400 to show the error.

Table:
```
for n in 200 400 600 800; do
  for t in 1 2 4 8; do
    echo "N=$n t=$t: $(echo "$n $t" | ./exp2 | grep 'time (with')"
  done
done
```

---

## Exp 3 - Cache-unfriendly vs cache-friendly sieve

exp3_sieve.c (needs -lm)
```
gcc -O2 -fopenmp exp3_sieve.c -o exp3 -lm
export OMP_NUM_THREADS=4
./exp3 100000000
```
Input: N as argument (default 100000000).
Expected primes: N=10^6 -> 78498, 10^7 -> 664579, 10^8 -> 5761455.
Output:
```
N = 100000000
Cache unfriendly : primes = 5761455 time = x.xxxxxx s
Cache friendly : primes = 5761455 time = x.xxxxxx s
Parallel (4 thr) : primes = 5761455 time = x.xxxxxx s
Correctness : all counts match
```
Table:
```
for n in 1000000 10000000 100000000 500000000; do ./exp3 $n; done
```
Memory: N=5e8 allocates 500 MB per sieve; check `free -h` first.

---

## Exp 4 - Colour to grey image, OpenMP scheduling

exp4_bw.c (needs libgd, PNG input only)
```
gcc -fopenmp exp4_bw.c -o exp4 -lgd
./exp4 in.png out.png <static|dynamic|guided|default> <chunk> <threads> [mark]
```
Examples:
```
./exp4 img1024.png bw.png static 10 4
./exp4 img4096.png out.png dynamic 50 4
./exp4 img1024.png mark.png static 50 4 1
```
- chunk is ignored for `default`; pass any number (e.g. 0).
- 6th argument `1` = part (b): each thread paints its pixels in its own colour; open mark.png to see the pattern.
Output:
```
1024x1024 schedule=static chunk=10 threads=4 time = 0.0xxxxx s
```
Need a PNG? Use your own, or generate one (any size):
```
python3 mkpng.py 1024 img1024.png
```
mkpng.py:
```
import zlib, struct, random, sys
w = h = int(sys.argv[1]); out = sys.argv[2]
raw = b''.join(b'\x00' + bytes(random.randrange(256) for _ in range(w*3)) for _ in range(h))
def ch(t, d):
    c = struct.pack('>I', len(d)) + t + d
    return c + struct.pack('>I', zlib.crc32(t + d) & 0xffffffff)
open(out, 'wb').write(b'\x89PNG\r\n\x1a\n' + ch(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) + ch(b'IDAT', zlib.compress(raw)) + ch(b'IEND', b''))
```
Generated images are random noise (fine for timing). For a recognisable image in part (b), use a real PNG photo. JPEG: convert first (`convert in.jpg in.png`).
Tables:
```
for s in default static dynamic guided; do
  for n in 512 1024 2048 4096; do ./exp4 img$n.png out.png $s 10 4; done
done
for s in static dynamic guided; do
  for c in 1 10 50 100; do ./exp4 img4096.png out.png $s $c 4; done
done
```

---

## Exp 5 - MPI message passing

exp5_mpi_msg.c
```
mpicc exp5_mpi_msg.c -o exp5
mpirun -np 4 ./exp5
```
Input: none. Use exactly 4 processes (`-np` > 4 reads out of bounds). Fewer than 4 cores: add `--oversubscribe`.
Output:
```
Rank 0 received "HELLO" from rank 1
Rank 0 received "CSE" from rank 2
Rank 0 received "RVCE" from rank 3
```

---

## Exp 6 - Parallel word search

exp6_wordsearch.c
```
gcc -fopenmp exp6_wordsearch.c -o exp6
./exp6 words.txt 4
```
Input: file path (default words.txt in current folder), then thread count (default 4).
Any text file works. Keywords searched: The, around, graphics, from, by, be, any, which, various, mount.
Output:
```
The        : 6561
around     : 6474
...
mount      : 6357
Threads = 4 Time taken = 0.0xxxxx s
```
Counts must be identical for every thread count.
Make files of 1/5/10/20 MB:
```
python3 mkwords.py 1 w1.txt
python3 mkwords.py 5 w5.txt
python3 mkwords.py 10 w10.txt
python3 mkwords.py 20 w20.txt
```
mkwords.py:
```
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
```
(Or use any real text file and `cat a.txt a.txt a.txt > big.txt` to grow it.)
Table:
```
for f in w1.txt w5.txt w10.txt w20.txt; do
  for t in 1 2 4 8; do echo "$f t=$t: $(./exp6 $f $t | grep Time)"; done
done
```

---

## Exp 7 - Matrix multiplication (OpenACC)

exp7_openacc.c

GPU machine with NVIDIA HPC SDK:
```
nvc -acc -Minfo=accel exp7_openacc.c -o exp7
./exp7 1000
```
(Paste the -Minfo=accel output into the record as proof of offloading.)

No GPU / no nvc (GCC, CPU fallback, no speedup expected):
```
gcc -O2 -fopenacc exp7_openacc.c -o exp7
./exp7 1000
```
Input: N as argument (default 1000).
Output:
```
N = 1000
Serial CPU : 1.xxxxxx s
OpenACC : x.xxxxxx s
OpenACC matrix multiplication test was successful!
```
Table:
```
for n in 256 512 1000 2000; do ./exp7 $n; done
```

---

## Common failures

| Symptom | Cause / fix |
|---|---|
| `fatal error: gd.h` | `sudo apt install libgd-dev` |
| `fatal error: mpi.h` / `mpicc: not found` | install openmpi (see setup) |
| `There are not enough slots available` | add `--oversubscribe` to mpirun |
| `undefined reference to sqrt` | add `-lm` at the END of the gcc command |
| `undefined reference to gdImage...` | add `-lgd` at the END of the gcc command |
| Segfault in exp4 | input is not a PNG (guard prints "not a valid PNG") |
| `cannot open words.txt` | file not in current folder, or pass its path |
| Threads > 1 shows no speedup | `nproc` is the limit; threads beyond cores do not help |
