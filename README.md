
---

## ✅ Question 1: ELF Binary Reverse Engineering 

### 📄 File: `report.md`

### 🔧 Tools Used:
- `objdump -d ./elf_binary`
- `strace ./elf_binary`
- `gdb ./elf_binary`

### 📋 Report Includes:
- List of functions from `objdump`
- Sample conditional/unconditional jump and control flow effect
- System call analysis from `strace`
- Function calls and memory behavior using `gdb` breakpoints
- Execution trace and function transitions

### 🧠 Key Skills:
- Control flow reconstruction
- System-level call tracing
- Memory access classification (heap, stack, globals)

---

## ✅ Question 2: Assembly Program to Count Names 

### 📄 File: `main.asm`

### 🧰 Tools:
- NASM assembler: `nasm -f elf32 -o line_counter.o line_counter.asm`
- Linker: `ld -m elf_i386 -o line_counter line_counter.o`

### ▶️ Run:
```bash
./line_counter

📥 Input:
input.txt in the same directory with each line containing a name.

Lines may be empty and should be counted.

🖥️ Output:
Prints the total number of lines (non-empty + empty) to console.

💬 Comments in code explain:
File reading with syscalls

Line-by-line traversal logic

Loop and condition flow control

✅ Question 3: C Extension Module for Python 
📄 Files:
stats_module.c — C source for extension

setup.py — Build script

test_stats.py — Sample Python usage

⚙️ Build:
bash
Copy
Edit
python3 setup.py build
python3 setup.py install --user
🧪 Test:
bash
Copy
Edit
python3 test_stats.py
🧮 Functions Exposed:
sum_values(): Sum of float array

mean_values(): Mean

std_dev(): Standard deviation

mode_value(): Most frequent number

count_values(): Number of elements

📑 Code explains:
How each function works

Time complexity (e.g., O(n), O(n^2) for mode)

Memory efficiency and usage

✅ Question 4: Producer-Consumer Simulation 
📄 File: main.c
🧵 Features:
Producer adds to a shared queue every 2 seconds

Consumer removes from the queue every 3 seconds

Queue max size: 10

Full queue: producer waits

Empty queue: consumer waits

Thread-safe synchronization using pthread_mutex and pthread_cond

🛠️ Compile & Run:
bash
Copy
Edit
gcc producer_consumer.c -o simulation -pthread
./simulation
💬 Console Output Example:
arduino
Copy
Edit
[Producer] Added item 1. Queue size: 1
[Consumer] Packed item 1. Queue size: 0
...
💬 Comments explain:
Condition variable usage

Producer blocking when queue is full

Consumer blocking when queue is empty

✅ Question 5: Real-Time Chat System 
📄 Files:
server.c — Central chat server

client.c — Interactive chat client

⚙️ Compile:
bash
Copy
Edit
gcc server.c -o server -pthread
gcc client.c -o client -pthread
▶️ Run:
Start server:

bash
Copy
Edit
./server
Then, in multiple terminals:

bash
Copy
Edit
./client
🧪 Functionality:
Each client enters a username for authentication

Server shares online user list

Clients can choose whom to message

Messages delivered as:

css
Copy
Edit
From alice: Hello!
🔐 Features:
TCP sockets for reliable delivery

Authenticated users only

Thread-per-client model on server

Input loop + receiver thread on client

💬 Comments explain:
Authentication logic

Online user tracking

Message routing and delivery

Thread management
