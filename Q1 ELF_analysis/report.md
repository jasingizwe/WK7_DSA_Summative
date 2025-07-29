
FUNCTIONS IDENTIFIED IN THE BINARY:

init: Sets up the program’s runtime environment before main is called.

.plt (Procedure Linkage Table): Contains jump stubs to dynamically linked library functions.

__cxa_finalize@plt: Cleans up after atexit() handlers, part of C++/libc runtime.

strcpy@plt: Calls the standard C function strcpy() to copy strings.

puts@plt: Calls puts() to write strings to standard output with a newline.

fclose@plt: Closes an open file stream.

__stack_chk_fail@plt: Stack smashing protector function, called if a buffer overflow is detected.

printf@plt: Sends formatted output to standard output.

strcspn@plt: Searches a string for a set of characters (typically used to trim input).

fgets@plt: Reads a line of input from a file or stdin.

strcmp@plt: Compares two strings for equality.

fprintf@plt: Sends formatted output to a file stream.

fopen@plt: Opens a file and returns a pointer to it.

perror@plt: Prints an error message related to the last system error.

exit@plt: Terminates the program.

_start: Program’s real entry point before main is called; sets up the stack, arguments, etc.

deregister_tm_clones: Internal function used for cleaning up thread-local storage.

register_tm_clones: Registers thread-local data structures.

__do_global_dtors_aux: Executes destructors for global variables.

frame_dummy: Used for managing debug frame information.

main: Main function — program logic starts here.

getStudentNames: Likely reads student names from input.

writeToFile: Writes data (probably student names) to a file.

readFromFile: Reads data from a file.

sortNames: Sorts student names (probably alphabetically).

printNames: Displays student names to the user.

_fini: Called at the end of the program, runs cleanup routines.

CONTROL FLOW ANALYSIS:

🔸 Instruction:
asm
Copy
Edit
jne 0x40114e
🔸 Meaning:
jne stands for Jump if Not Equal. It checks the Zero Flag (ZF) after a comparison:

If ZF = 0 (i.e., values are not equal), the program jumps to address 0x40114e.

If ZF = 1 (values are equal), it continues to the next instruction.

🔸 Effect on Control Flow:
This conditional jump alters program execution:

If condition is met (not equal), it jumps forward, skipping some instructions.

Otherwise, it continues line-by-line.

Used typically in:

if (x != y) logic

Looping

Input validation or error checks

🔸 Context from Disassembly:
asm
Copy
Edit
40113e:	48 85 c0             	test   rax, rax
401141:	74 05                	je     401148
401143:	48 89 c7             	mov    rdi, rax
401146:	ff d0                	call   rax
401148:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
40114c:	48 85 c0             	test   rax, rax
40114f:	74 05                	je     401156
test rax, rax: checks if rax == 0

jne 0x40114e: jumps if rax is not zero

🔸 C-like Pseudocode:
c
Copy
Edit
if (rax != 0) {
    goto 0x40114e;
} else {
    // continue with the next line
}

STRACE SYSTEM CALL SUMMARY:

🔸 Observed System Calls & Explanation
Syscall	Description
write(1, ...): Output to terminal (stdout), used to prompt for user input and print messages
read(0, ...): Read from terminal (stdin), used to capture names from the user
openat(...): Open or create files (students.txt, sorted_students.txt)
fstat(...): Retrieve metadata about the opened file (e.g., size, type)
close(...): Close an opened file descriptor
read(3, ...): Read contents from the file (likely to load stored student names)
write(3, ...): Write to files: first to save the names, then to store the sorted names
exit_group(0): Exit the program successfully

🔸 Program Behavior (Inferred)
Based on the syscalls and arguments:

User Input Phase

The program prompts the user to input student names using write(1, ...).

The names are read using read(0, ...), and this continues sequentially for 5 students (Student 6 to Student 10).

File Write Phase (Raw Input Storage)

The program opens students.txt with O_WRONLY|O_CREAT|O_TRUNC, meaning:

If the file doesn't exist, it is created.

If it exists, it is cleared (truncated) before writing.

It writes all the collected names to this file and closes it.

File Read Phase (Load for Sorting)

The same file (students.txt) is opened again and its contents read — possibly to prepare for sorting.

File Write Phase (Sorted Output)

It creates sorted_students.txt, then writes the sorted list of names into it.

Afterward, it prints a success message to the terminal.

Program Termination

The program exits cleanly with exit_group(0).

🔸 Final Output Seen in Terminal
yaml
Copy
Edit
Student 6:
Student 7:
Student 8:
Student 9:
Student 10:

Sorted names have been written to sorted_students.txt
🔸 Inferred C-like Pseudocode Logic
c
Copy
Edit
for (int i = 6; i <= 10; i++) {
    printf("Student %d: ", i);
    fgets(name[i], 100, stdin);
}

write_names_to_file("students.txt", names);
sorted = sort_names(names);
write_names_to_file("sorted_students.txt", sorted);
printf("\nSorted names have been written to sorted_students.txt\n");

GDB ANALYSIS

Breakpoints Set
Breakpoint 1: Set at the entry of the main() function at address 0x00005555555552b1. This allowed inspection of the initial program state.

Breakpoint 2: Set at instruction address 0x5555555552e0 inside the main() function, triggered after the prompt "Enter 10 full names of students" appeared.

Watchpoint 3: Set on the memory location *(int*)($rbp - 0x10), to observe write activity to a local integer variable.

Initial Register State at Entry of main
rdi = 1: Indicates the argument count (argc = 1).

rsi = 0x7fffffffdda8: Pointer to the argv array.

rsp = rbp = 0x7fffffffdc80: Stack frame initialized for main, showing the start of a new function call frame.

Attempting to inspect argv[0] at $rsi yielded non-readable characters, suggesting the argument may not be a valid C string or memory corruption may be present at that moment.

Instruction Trace (x/5i $rip)
The first five instructions of main were:

asm
Copy
Edit
0x5555555552b1 <main+8>:    sub    $0x3f0,%rsp
0x5555555552b8 <main+15>:   mov    %fs:0x28,%rax
0x5555555552c1 <main+24>:   mov    %rax,-0x8(%rbp)
0x5555555552c5 <main+28>:   xor    %eax,%eax
0x5555555552c7 <main+30>:   mov    $0xa,%esi
Explanation:

sub $0x3f0, %rsp: Allocates 1008 bytes on the stack (likely for local variables or buffers).

mov %fs:0x28, %rax and subsequent store: Sets up thread-local storage or stack canary.

xor %eax, %eax: Sets eax = 0 (common C idiom to set return value to 0).

mov $0xa, %esi: Loads 10 into esi, possibly preparing for a loop or function call.

Function Transition and Runtime Behavior
Continued execution led to a prompt: Enter 10 full names of students:

At this point, control reached the second breakpoint at 0x5555555552e0, still within main, suggesting user input is handled next.

This confirms that the program interacts with standard I/O, likely reading names and storing them in memory.

Stack Inspection
Using x/10gx $rsp, the stack contents revealed several 64-bit values. Notably:

0x00007fffffffd9d0, 0x00007ffff7fd4ae3: Likely return addresses or saved register values.

Presence of 0x0000000000000009 suggests integer values are being used or returned within the stack.

Watchpoint Observation
A watchpoint was set on *(int*)($rbp - 0x10), a likely local variable.

This will allow detection of when the program writes to this specific memory address, revealing how and when variables are manipulated within main.