# Pipex Project - Detailed Code Explanation

This document provides a comprehensive explanation of every component in the pipex project, designed to help you understand the code for your 42 evaluation.

## Table of Contents
1. [Project Overview](#project-overview)
2. [String Utility Functions](#string-utility-functions)
3. [Path Utilities](#path-utilities)
4. [Pipe Utilities](#pipe-utilities)
5. [Execution Utilities](#execution-utilities)
6. [Error Handling](#error-handling)
7. [Main Logic Flow](#main-logic-flow)
8. [Bonus Features](#bonus-features)

---

## Project Overview

**pipex** recreates the shell pipe functionality. When you run:
```bash
./pipex file1 cmd1 cmd2 file2
```

It behaves like: `cmd1 < file1 | cmd2 > file2`

The program:
1. Opens `file1` for reading
2. Executes `cmd1` with `file1` as stdin
3. Pipes `cmd1` output to `cmd2`
4. Executes `cmd2` with pipe as stdin
5. Writes `cmd2` output to `file2`

---

## String Utility Functions

### ft_strlen.c
**Purpose**: Calculate the length of a string.

```c
size_t ft_strlen(const char *s)
```
- **Line-by-line**:
  - `len = 0`: Initialize counter
  - `while (s[len] != '\0')`: Loop until null terminator
  - `len++`: Increment for each character
  - `return (len)`: Return total count

**Why needed**: Used throughout the project for string operations (memory allocation, comparisons, etc.)

---

### ft_strncmp.c
**Purpose**: Compare two strings up to n characters.

```c
int ft_strncmp(const char *s1, const char *s2, size_t n)
```
- **Logic**:
  - Returns 0 if strings match up to n characters
  - Returns positive if s1 > s2 (ASCII difference)
  - Returns negative if s1 < s2
- **Used for**: Comparing PATH variable names, command names

---

### ft_strchr.c
**Purpose**: Find first occurrence of a character in a string.

```c
char *ft_strchr(const char *s, int c)
```
- **Returns**: Pointer to character or NULL if not found
- **Used for**: Checking if a command path contains '/' (absolute path check)

---

### ft_strdup.c
**Purpose**: Duplicate a string with malloc.

```c
char *ft_strdup(const char *s1)
```
- **Process**:
  1. Calculate length with `ft_strlen`
  2. Allocate memory: `(len + 1)` bytes (for null terminator)
  3. Copy characters one by one
  4. Add null terminator
  5. Return pointer
- **Memory**: Caller must free the returned pointer

---

### ft_substr.c
**Purpose**: Extract a substring from a string.

```c
char *ft_substr(char const *s, unsigned int start, size_t len)
```
- **Edge cases handled**:
  - NULL input → returns NULL
  - start beyond string length → returns empty string
  - len exceeds available characters → adjusts to available length
- **Used for**: Extracting parts of strings (like command arguments)

---

### ft_strjoin.c
**Purpose**: Concatenate two strings.

```c
char *ft_strjoin(char const *s1, char const *s2)
```
- **Process**:
  1. Calculate lengths of both strings
  2. Allocate: `(len1 + len2 + 1)` bytes
  3. Copy s1 first, then s2
  4. Add null terminator
- **Used for**: Building full command paths (e.g., "/usr/bin" + "/" + "ls" = "/usr/bin/ls")

---

### ft_split.c
**Purpose**: Split a string by a delimiter character.

```c
char **ft_split(char const *s, char c)
```
- **Helper functions**:
  - `count_words()`: Counts how many substrings will be created
  - `get_word()`: Extracts one word at a time
  - `free_array()`: Frees all allocated memory if error occurs
- **Process**:
  1. Count words (sequences of non-delimiter characters)
  2. Allocate array of pointers: `(word_count + 1)` (extra for NULL terminator)
  3. Extract each word using `ft_substr`
  4. Set last element to NULL
- **Used for**: 
  - Splitting PATH by ':' → ["/usr/bin", "/bin", "/usr/local/bin"]
  - Splitting commands by ' ' → ["ls", "-l", "-a"]

---

## Path Utilities

### path_utils.c

#### find_path_var()
**Purpose**: Extract PATH environment variable from envp array.

```c
static char *find_path_var(char **envp)
```
- **Process**:
  - Iterates through envp array
  - Looks for string starting with "PATH="
  - Returns pointer to value part (after "PATH=")
- **Why static**: Internal helper function, not needed outside this file

#### build_cmd_path()
**Purpose**: Build full path by joining directory + "/" + command.

```c
static char *build_cmd_path(char *dir, char *cmd)
```
- **Example**: `build_cmd_path("/usr/bin", "ls")` → "/usr/bin/ls"
- **Memory**: Allocates new string, caller must free

#### free_split()
**Purpose**: Free array of strings created by ft_split.

```c
void free_split(char **arr)
```
- **Process**:
  1. Iterates through array
  2. Frees each string
  3. Frees the array itself
- **Safety**: Checks for NULL before dereferencing

#### get_cmd_path()
**Purpose**: Find the full path to a command executable.

```c
char *get_cmd_path(char *cmd, char **envp)
```
- **Algorithm**:
  1. **Absolute path check**: If cmd contains '/', test it directly with `access()`
  2. **Extract PATH**: Find PATH variable from envp
  3. **Split PATH**: Split by ':' to get directory list
  4. **Test each directory**: 
     - Build full path: `dir + "/" + cmd`
     - Test with `access(path, X_OK)` (X_OK checks execute permission)
     - Return first valid path found
  5. **Cleanup**: Free split array before returning
- **Returns**: 
  - Full path if found
  - NULL if command not found
- **Memory**: Returns malloc'd string, caller must free

**Example flow**:
```
cmd = "ls"
PATH = "/usr/bin:/bin:/usr/local/bin"
→ Tests: "/usr/bin/ls" → found! Returns "/usr/bin/ls"
```

---

## Pipe Utilities

### pipe_utils.c

#### create_pipe()
**Purpose**: Create a pipe and handle errors.

```c
void create_pipe(int *fd)
```
- **pipe() system call**:
  - Creates two file descriptors: `fd[0]` (read end) and `fd[1]` (write end)
  - Returns -1 on error
- **Error handling**: Calls `perror_exit()` if pipe creation fails

#### redirect_stdin()
**Purpose**: Redirect stdin to a file descriptor.

```c
void redirect_stdin(int fd)
```
- **dup2() system call**:
  - `dup2(fd, STDIN_FILENO)`: Makes STDIN_FILENO point to the same file as `fd`
  - After dup2, reading from stdin reads from `fd`
- **Close original**: Closes `fd` after redirection (we now have STDIN_FILENO pointing to it)

#### redirect_stdout()
**Purpose**: Redirect stdout to a file descriptor.

```c
void redirect_stdout(int fd)
```
- **Same as redirect_stdin** but for stdout
- `dup2(fd, STDOUT_FILENO)`: Makes stdout write to `fd`

#### close_fds()
**Purpose**: Safely close two file descriptors.

```c
void close_fds(int fd1, int fd2)
```
- **Safety check**: Only closes if fd >= 0 (valid file descriptor)
- **Why needed**: Prevents closing invalid fds (like -1)

---

## Execution Utilities

### exec_utils.c

#### parse_cmd()
**Purpose**: Parse command string into argv array.

```c
char **parse_cmd(char *cmd)
```
- **Process**: Uses `ft_split(cmd, ' ')` to split by spaces
- **Returns**: Array of strings, NULL-terminated (like argv)
- **Example**: `"ls -l -a"` → `["ls", "-l", "-a", NULL]`

#### execute_cmd()
**Purpose**: Execute a command with proper path resolution.

```c
void execute_cmd(char *cmd, char **envp)
```
- **Process**:
  1. **Parse command**: Split into argv array
  2. **Find path**: Get full path using `get_cmd_path()`
  3. **Error handling**: 
     - If parsing fails → error exit
     - If path not found → `cmd_not_found()` (exits with code 127, like shell)
  4. **Execute**: `execve(cmd_path, argv, envp)`
     - Replaces current process with command
     - Never returns on success
     - Returns -1 on error

**execve() explanation**:
- First argument: full path to executable
- Second argument: argv array (command + arguments)
- Third argument: environment variables
- **Important**: This function replaces the current process image, so it never returns on success

---

## Error Handling

### error_utils.c

#### error_exit()
**Purpose**: Print error message and exit.

```c
void error_exit(char *msg)
```
- Writes message to stderr (file descriptor 2)
- Exits with EXIT_FAILURE

#### perror_exit()
**Purpose**: Use perror() to print system error and exit.

```c
void perror_exit(char *msg)
```
- `perror()` prints: `msg: system error message`
- Useful for system call errors (open, pipe, fork, etc.)

#### cmd_not_found()
**Purpose**: Handle command not found errors.

```c
void cmd_not_found(char *cmd)
```
- Prints: `"pipex: command not found: cmd"`
- Exits with code 127 (standard shell exit code for command not found)

---

## Main Logic Flow

### main.c

#### Program Flow:

```c
int main(int ac, char **av, char **envp)
```

**Step 1: Argument Validation**
```c
validate_args(ac, av);
```
- Checks argc == 5
- Opens and closes infile (tests read permission)
- Opens and closes outfile (tests write/create permission)

**Step 2: Open Files**
```c
infile = open(av[1], O_RDONLY);
outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
```
- `O_RDONLY`: Open for reading only
- `O_WRONLY | O_CREAT | O_TRUNC`: Write only, create if doesn't exist, truncate if exists
- `0644`: Permissions (rw-r--r--)

**Step 3: Create Pipe**
```c
create_pipe(pipe_fd);
```
- Creates pipe: `pipe_fd[0]` (read), `pipe_fd[1]` (write)

**Step 4: Fork First Child**
```c
pid1 = fork();
if (pid1 == 0)
    first_child_process(infile, pipe_fd, av[2], envp);
```
- **fork()** creates a copy of the process
- Returns 0 in child, PID in parent
- Child process:
  - Closes pipe read end (doesn't need it)
  - Redirects stdin to infile
  - Redirects stdout to pipe write end
  - Executes cmd1
  - Never returns (replaced by execve)

**Step 5: Fork Second Child**
```c
pid2 = fork();
if (pid2 == 0)
    second_child_process(outfile, pipe_fd, av[3], envp);
```
- Similar to first child, but:
  - Redirects stdin to pipe read end
  - Redirects stdout to outfile
  - Executes cmd2

**Step 6: Parent Cleanup**
```c
close_fds(pipe_fd[0], pipe_fd[1]);
close_fds(infile, outfile);
waitpid(pid1, NULL, 0);
waitpid(pid2, NULL, 0);
```
- Closes all file descriptors (children have their own copies)
- `waitpid()`: Waits for children to finish
- Prevents zombie processes

---

## Bonus Features

### Heredoc (heredoc.c)

#### is_heredoc()
**Purpose**: Check if heredoc mode is requested.

```c
int is_heredoc(int argc, char **argv)
```
- Checks if argc >= 6 and argv[1] == "here_doc"

#### handle_heredoc()
**Purpose**: Handle heredoc input.

```c
int handle_heredoc(char *limiter)
```
- **Process**:
  1. Creates pipe
  2. Forks child process
  3. Child reads lines from stdin until limiter is found
  4. Writes lines to pipe
  5. Returns pipe read end to parent

**read_line()**:
- Reads character by character from stdin
- Builds string dynamically (reallocates for each character)
- Stops at newline

**Usage**: `./pipex here_doc LIMITER cmd1 cmd2 file`

### Multiple Pipes (multi_pipe.c)

#### execute_multi_pipe()
**Purpose**: Execute multiple commands in a pipeline.

```c
void execute_multi_pipe(int argc, char **argv, char **envp, int infile)
```
- **Algorithm**:
  1. For each command (except last):
     - Create pipe
     - Fork child
     - Child: redirect stdin from previous pipe, stdout to new pipe
     - Execute command
  2. Last command:
     - Fork child
     - Redirect stdin from last pipe, stdout to output file
     - Execute command
  3. Parent waits for all children

**Pipe chain**:
```
infile → cmd1 → pipe1 → cmd2 → pipe2 → cmd3 → outfile
```

**Usage**: `./pipex file1 cmd1 cmd2 cmd3 file2`

---

## Key Concepts for Evaluation

### Process Management
- **fork()**: Creates child process (copy of parent)
- **execve()**: Replaces process image (never returns on success)
- **waitpid()**: Parent waits for child to finish

### File Descriptors
- **STDIN_FILENO** (0): Standard input
- **STDOUT_FILENO** (1): Standard output
- **STDERR_FILENO** (2): Standard error
- **dup2()**: Duplicates file descriptor (redirects I/O)

### Pipes
- **pipe(fd)**: Creates two file descriptors
  - `fd[0]`: Read end
  - `fd[1]`: Write end
- Data written to `fd[1]` can be read from `fd[0]`
- Unidirectional communication

### Memory Management
- All `malloc()` calls must have corresponding `free()`
- `ft_split()` returns malloc'd array → must free with `free_split()`
- `get_cmd_path()` returns malloc'd string → must free

### Error Handling
- Check return values of all system calls
- Use appropriate error messages
- Exit codes: 0 (success), 1 (failure), 127 (command not found)

---

## Common Questions for Evaluation

**Q: Why do we close file descriptors in the parent after forking?**
A: Children have their own copies of file descriptors. Closing in parent doesn't affect children, but prevents resource leaks.

**Q: Why does execve never return?**
A: execve replaces the current process image with the new program. The old program code is gone, so there's nothing to return to.

**Q: What happens if a command is not found?**
A: `get_cmd_path()` returns NULL, we call `cmd_not_found()` which prints error and exits with code 127.

**Q: How does the pipe work between processes?**
A: Parent creates pipe, forks children. Children inherit pipe fds. First child writes to pipe[1], second child reads from pipe[0]. OS kernel handles the data transfer.

**Q: Why do we need to find the command path?**
A: execve() requires the full path to the executable. We search PATH directories to find where the command is located.

---

This explanation covers all major components. Study each function's purpose, the system calls used, and how data flows through the program. Good luck with your evaluation!

