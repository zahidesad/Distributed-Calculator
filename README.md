# Distributed Calculator Project

## Project Topic
The **Distributed Calculator Project** is designed to demonstrate process creation, inter-process communication (IPC), and basic operating system concepts such as pipes, `fork`, and `exec` system calls. This project implements a simple distributed calculator system, where each arithmetic operation (addition, subtraction, multiplication, and division) is handled by a separate process. The main program (`Calculator`) interacts with these processes using dedicated pipes for data transfer. Additionally, a separate saver program is utilized to log results into a file (`results.txt`).

### Key Objectives
- Utilizing Linux system calls to manage processes and communication.
- Implementing modular design, where each operation is handled by its own program.
- Ensuring the parent process (`Calculator`) manages the lifecycle of child processes.
- Avoiding direct output operations (e.g., `printf`) within the arithmetic operation processes, with the main program handling user interactions.

## Tasks Completed During the Project

### Main Program (Calculator)
- Designed the main program (`calculator.c`) to:
  - Create pipes for communication with each child process.
  - Use `fork` to create separate processes for each operation (addition, subtraction, multiplication, division).
  - Send input operands to the corresponding child process through pipes.
  - Receive results from child processes and display them to the user.
  - Invoke the saver program to log results in a file.
  - Terminate all child processes gracefully when the user selects the "exit" option.

### Inter-Process Communication with Pipes
- Facilitated communication by setting up multiple pipes, where each subprocess has a dedicated pair of pipes for two-way communication.
- The main process writes the user’s chosen operation parameters (operands) into the write-end of the appropriate pipe. The subprocess reads these values from its read-end, performs the calculation, and returns the result to the main process through another pipe.
- This design ensures clarity in the data flow and prevents interference between different operations.

### Subprocess Computation and Result Retrieval
- Each arithmetic subprocess:
  - Receives operands from the main process.
  - Performs the required calculation.
  - For division operations, checks for division-by-zero errors and responds with a designated error message rather than a numeric result.
  - Writes the final answer (or error message) back to the main process via its dedicated output pipe.

### Integration of the Saver Process
- After computing the result, each arithmetic subprocess invokes the saver program using `fork` and `exec`.
- The computed result is passed as a command-line argument to the saver, which appends the result to `results.txt`.
- This modular design separates the calculation logic from the file-writing logic, making the code more maintainable and flexible.

### User Interaction and Control Flow
- The main calculator program presents a simple text-based menu, allowing the user to choose between addition, subtraction, multiplication, division, or exiting the application.
- User inputs are directed to the relevant subprocess for computation.
- The program continues until the user selects "exit," at which point it signals each subprocess to shut down.

### Synchronization and Graceful Termination
- The main program uses `wait` system calls to ensure all subprocesses terminate correctly before the main program exits.
- This prevents orphaned processes and ensures a controlled shutdown sequence.

### Validation and Error Handling
- Basic input validation ensures:
  - Operands are valid numbers.
  - Division does not attempt to divide by zero.
- Informative error messages are sent back and logged, maintaining robustness and preventing crashes.

## Additional Notes

### Environment and Prerequisites
- Designed for a Linux-based environment.
- Requires familiarity with:
  - Terminal commands
  - The C programming language
  - Standard Unix/Linux system calls (`fork`, `exec`, `pipe`, `wait`)

### Compilation and Execution
- A `Makefile` is included to streamline compilation.
- Run `make` to compile all source files into their respective binaries.
- Start the main program using `./calculator`.
- Tested using the GNU Compiler Collection (GCC) and standard libraries.

### Maintainability and Extensibility
- The architecture is designed for easy extension:
  - Adding another mathematical operation involves creating a new subprocess and adding its pipes.
  - Each component (operation) is isolated in its own program, enabling independent modifications.

### Future Improvements
- Potential enhancements:
  - Advanced error handling
  - Improved user interface
  - Logging computation steps
  - Supporting complex input operations
  - Demonstrating additional IPC paradigms such as shared memory or message queues.

### Comparison with Other Approaches
- Focuses on simplicity and modularity.
- Adheres to the principle of single responsibility by separating each operation into its own executable.
- Alternatives such as shared memory or sockets could add complexity but provide additional learning opportunities.

## Conclusion
This project achieved its goals of demonstrating process-based modular design, inter-process communication, and basic operating system concepts. By following a structured and modular approach, it highlights the power of process separation and efficient resource management. Future enhancements can build upon this foundation to create a more advanced distributed calculator system.

## References

- Kerrisk, Michael. *The Linux Programming Interface*. No Starch Press, 2010.
- Stevens, W. Richard, and Stephen A. Rago. *Advanced Programming in the UNIX Environment*. Addison-Wesley, 2013.
- [GNU Make Manual](https://www.gnu.org/software/make/manual/make.html)
- [ISO C Standard](https://www.iso.org/standard/74528.html)
