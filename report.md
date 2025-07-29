
Functions Identified in the Binary:
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