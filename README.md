# Pipex

## Overview

**Pipex** is a project developed as part of the curriculum at [Hive Helsinki](https://www.hive.fi/). The goal of this project is to recreate the functionality of the Unix pipe `|` operator in C. The program executes commands similarly to how they are handled in the shell, allowing the output of one command to be the input of another.

## Objectives

- **Understanding Unix Processes**: Learn how to create and manage processes using `fork()`.
- **Inter-Process Communication**: Implement pipes for communication between processes.

- **Command Execution**: Execute shell commands from within a C program using `execve()`.

## Getting Started


### Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/yourusername/pipex.git
2. **Navigate to the project directory:**:

   ```bash
   cd pipex
3. **Compile the program:**:
 ```bash
   make
```
## Usage
The syntax for running the program is as follows:

```bash
./pipex infile "cmd1" "cmd2" "cmd3" outfile
```
This command is equivalent to the shell command:
```
< infile cmd1 | cmd2 | cmd3 > outfile
```
### Parameters

- **infile**: The input file from which data is read.
- **cmd1**: The first command to execute.
- **cmd2**: The second command to execute.
- **cmd3**: The third command to execute.
- **outfile**: The output file where the result is written.

### Examples

**Basic Example:**

```bash
./pipex input.txt "grep 'hello'" "wc -l" output.txt
```
This will search for the word "hello" in input.txt, count the number of matching lines, and write the result to output.txt.

**Another Example with Multiple Flags:**

```bash
./pipex input.txt "cat" "cat" "cat" output.txt
```
This example will simply pass the contents of input.txt through three consecutive cat commands, ultimately writing the unchanged content to output.txt.

## Bonus Part

**Handling Multiple Pipes**:  
  The program should support multiple commands linked by pipes, allowing for more complex command sequences.

  Example:

  ```bash
  ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```
Should behave like:
```
< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
```
**`here_doc` Support (`<<` and `>>`)**:  
When `"here_doc"` is the first parameter, the program should:

```
./pipex here_doc LIMITER cmd cmd1 file
```
This should behave like:
```
cmd << LIMITER | cmd1 >> file
```
LIMITER ends the here_doc input, and output is appended to file.
