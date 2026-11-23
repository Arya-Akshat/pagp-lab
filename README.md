# Parallel Programming Lab Programs

A reference guide for compiling, running, and testing all lab programs with input/output examples.

---

## 1a) Monte Carlo Estimation of Pi (OpenMP)

### How to Run:
- **Windows (MinGW / GCC):**
  ```cmd
  g++ -fopenmp 1a\main.cpp -o 1a\main.exe
  1a\main.exe
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 1a/main.cpp -o 1a/main
  ./1a/main
  ```
- **macOS (Homebrew GCC):**
  ```bash
  g++-14 -fopenmp 1a/main.cpp -o 1a/main
  ./1a/main
  ```

### Sample Input & Output:
- **Input:** (via standard input prompt)
  ```text
  Enter threads: 4
  ```
- **Output:**
  ```text
  Time: 0.001245
  Pi: 3.1416
  ```

---

## 1b) Monte Carlo Estimation of Pi (MPI)

### How to Run:
- **Windows (MS-MPI):**
  ```cmd
  mpicxx 1b\main.cpp -o 1b\main.exe
  mpiexec -n 4 1b\main.exe
  ```
- **Linux / WSL (OpenMPI / MPICH):**
  ```bash
  mpicxx 1b/main.cpp -o 1b/main
  mpirun -np 4 ./1b/main
  ```

### Sample Input & Output:
- **Input:** Number of processes specified via command-line flag `-n 4` or `-np 4`.
- **Output:**
  ```text
  Time: 0.000812
  Processes: 4
  Trials: 10000
  Est Pi: 3.148
  ```

---

## 2a) Matrix Multiplication (OpenMP)

### How to Run:
- **Windows (MinGW / GCC):**
  ```cmd
  g++ -fopenmp 2a\main.cpp -o 2a\main.exe
  2a\main.exe
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 2a/main.cpp -o 2a/main
  ./2a/main
  ```

### Sample Input & Output:
- **Input:** None (multiplies two hardcoded $1000 \times 1000$ matrices initialized to $1$).
- **Output:**
  ```text
  Time: 1.8423
  C[0]: 1000 (Expected: 1000)
  ```

---

## 2b) OpenMP Variable Scoping & Race Conditions (private vs shared)

### How to Run:
- **Windows (MinGW / GCC):**
  ```cmd
  g++ -fopenmp 2b\main.cpp -o 2b\main.exe
  2b\main.exe
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 2b/main.cpp -o 2b/main
  ./2b/main
  ```

### Sample Input & Output:
- **Input:** None (runs 500x500 matrix multiplication twice: with scoping and without scoping).
- **Output:**
  ```text
  Errors without scope: 247190 / 250000
  ```

---

## 3) Sieve of Eratosthenes (Unfriendly vs Cache-Friendly vs Parallel)

### How to Run:
- **Windows (MinGW / GCC):**
  ```cmd
  g++ -fopenmp 3\main.cpp -o 3\main.exe
  3\main.exe
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 3/main.cpp -o 3/main
  ./3/main
  ```

### Sample Input & Output:
- **Input:** None (evaluates total primes up to $N = 10^8$).
- **Output:**
  ```text
  Time: 1.4521
  Unfriendly: 5761455
  Time: 0.8123
  Friendly:   5761455
  Time: 0.2314
  Parallel:   5761455
  ```

---

## 4a) Image Processing / Grayscale Filter (OpenMP + libgd)

### Prerequisites:
- Requires **GD library** (`libgd`).
  - Ubuntu/Debian: `sudo apt-get install libgd-dev`
  - macOS: `brew install gd`
  - Windows (MSYS2): `pacman -S mingw-w64-x86_64-libgd`

### How to Run:
- **Windows (MSYS2 MinGW 64-bit):**
  ```cmd
  g++ -fopenmp 4a\main.cpp -lgd -o 4a\main.exe
  4a\main.exe input.png output.png
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 4a/main.cpp -lgd -o 4a/main
  ./4a/main input.png output.png
  ```

### Sample Input & Output:
- **Input:** Command-line arguments: `<input.png> <output.png>`.
  ```cmd
  4a\main.exe photo.png photo_gray.png
  ```
- **Output:**
  ```text
  Time: 0.08421
  ```
  *(Creates `photo_gray.png` with all pixels converted to grayscale).*

---

## 4b) Image Generation by Threads (OpenMP + libgd)

### How to Run:
- **Windows (MSYS2 MinGW 64-bit):**
  ```cmd
  g++ -fopenmp 4b\main.cpp -lgd -o 4b\main.exe
  4b\main.exe bands.png
  ```
- **Linux / WSL:**
  ```bash
  g++ -fopenmp 4b/main.cpp -lgd -o 4b/main
  ./4b/main bands.png
  ```

### Sample Input & Output:
- **Input:** Output filename passed via command-line argument:
  ```cmd
  4b\main.exe bands.png
  ```
- **Output:**
  ```text
  Time: 0.04218
  ```
  *(Creates a $1000 \times 1000$ image `bands.png` where threads paint 4 distinct colored strips).*

---

## 5) Point-to-Point Message Passing (MPI)

### How to Run:
- **Windows (MS-MPI):**
  ```cmd
  mpicc 5\main.c -o 5\main.exe
  mpiexec -n 4 5\main.exe
  ```
- **Linux / WSL:**
  ```bash
  mpicc 5/main.c -o 5/main
  mpirun -np 4 ./5/main
  ```

### Sample Input & Output:
- **Input:** Ran with 4 processes: `mpiexec -n 4`
- **Output:**
  ```text
  Received Hello in process 0 from process 1
  Received RVCE in process 0 from process 2
  Received CSE in process 0 from process 3
  ```

---

## 6) Word Search / Frequency Count in File (OpenMP)

### How to Run:
- Make sure `words.txt` is in the folder:
- **Windows (MinGW / GCC):**
  ```cmd
  cd 6
  g++ -fopenmp main.cpp -o main.exe
  main.exe
  ```
- **Linux / WSL:**
  ```bash
  cd 6
  g++ -fopenmp main.cpp -o main
  ./main
  ```

### Sample Input & Output:
- **Input:** File `words.txt` containing:
  ```text
  lorem ipsum dolo sit amet lorem lorem ipsum sit amet amet
  ```
- **Output:**
  ```text
  Time: 0.000412
  lorem: 3
  ipsum: 2
  dolo: 1
  sit: 2
  amet: 3
  ```

---

## 7) 2D Jacobi Iteration Solver (OpenACC GPU Offloading)

### Prerequisites:
- NVIDIA HPC SDK (`nvc`) OR GCC with OpenACC (`gcc -fopenacc -lm`).

### How to Run:
- **Linux / WSL (NVIDIA HPC SDK):**
  ```bash
  nvc -acc -fast 7/main.c -o 7/main
  ./7/main 1000
  ```
- **Linux / WSL (GCC):**
  ```bash
  gcc -fopenacc 7/main.c -lm -o 7/main
  ./7/main 1000
  ```
- **Windows (Standard GCC CPU fallback):**
  ```cmd
  gcc 7\main.c -o 7\main.exe
  7\main.exe 500
  ```

### Sample Input & Output:
- **Input:** Optional command-line argument for grid size $N$ (e.g. `1000` for a $1000 \times 1000$ grid).
  ```bash
  ./7/main 1000
  ```
- **Output:**
  ```text
  Grid: 1000 x 1000
  Serial:  2.845120 s (1000 iter)
  OpenACC: 0.142100 s (1000 iter)
  Speedup: 20.02x | Result: CORRECT
  ```
