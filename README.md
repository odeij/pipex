# 🚀 Pipex - 42 Project

<div align="center">

![42](https://img.shields.io/badge/42-School-000000?style=for-the-badge&logo=42)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![macOS](https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white)

**Recreate shell pipe functionality in C**

[Features](#-features) • [Installation](#-installation) • [Usage](#-usage) • [Architecture](#-architecture) • [Documentation](#-documentation)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [Architecture](#-architecture)
- [How It Works](#-how-it-works)
- [Project Structure](#-project-structure)
- [Documentation](#-documentation)
- [Testing](#-testing)
- [Technologies](#-technologies)

---

## 🎯 Overview

**pipex** is a 42 School project that recreates the shell pipe functionality (`|`) in C. It demonstrates deep understanding of:
- Process management (fork, execve, waitpid)
- Inter-process communication (pipes)
- File descriptor manipulation (dup2, open, close)
- Environment variable parsing (PATH resolution)
- Memory management and error handling

### What It Does

```bash
# Instead of:
$ cmd1 < file1 | cmd2 > file2

# You can use:
$ ./pipex file1 cmd1 cmd2 file2
```

---

## ✨ Features

### Mandatory Features
- ✅ **Basic Pipe Functionality**: Execute two commands with pipe between them
- ✅ **File Redirection**: Read from input file, write to output file
- ✅ **PATH Resolution**: Automatically finds commands in PATH environment variable
- ✅ **Error Handling**: Comprehensive error messages matching shell behavior
- ✅ **Memory Management**: Proper allocation and deallocation

### Bonus Features
- 🎁 **Heredoc Support**: Handle `here_doc` delimiter for interactive input
- 🎁 **Multiple Pipes**: Support for 3+ commands in a pipeline
- 🎁 **Robust Parsing**: Handles command arguments properly

---

## 🛠️ Installation

### Prerequisites
- GCC compiler
- Make
- Linux or macOS

### Build

```bash
# Clone the repository
git clone https://github.com/odeij/pipex.git
cd pipex

# Build mandatory version
make

# Build bonus version
make bonus

# Clean object files
make clean

# Clean everything
make fclean

# Rebuild
make re
```

---

## 📖 Usage

### Basic Usage

```bash
# Basic pipe: cmd1 < file1 | cmd2 > file2
./pipex input.txt "grep hello" "wc -l" output.txt

# With absolute paths
./pipex /etc/passwd "cat" "head -5" result.txt

# Multiple commands (bonus)
./pipex input.txt "cat" "grep test" "wc -w" output.txt
```

### Heredoc (Bonus)

```bash
# Interactive input until delimiter
./pipex_bonus here_doc END "cat" "grep hello" output.txt
# Type lines, then type "END" to finish
```

### Examples

```bash
# Count lines in a file
./pipex file.txt "cat" "wc -l" result.txt

# Search and filter
./pipex log.txt "grep ERROR" "sort" errors.txt

# Process and format
./pipex data.txt "cut -d: -f1" "sort -u" unique.txt
```

---

## 🏗️ Architecture

### System Calls Used

| System Call | Purpose | Usage |
|------------|---------|-------|
| `fork()` | Create child process | Execute commands in separate processes |
| `execve()` | Replace process image | Execute the actual command |
| `pipe()` | Create IPC channel | Connect commands |
| `dup2()` | Redirect file descriptors | Redirect stdin/stdout |
| `open()` | Open files | Access input/output files |
| `waitpid()` | Wait for child process | Prevent zombie processes |
| `access()` | Check file permissions | Verify command executability |

### Process Flow

```
┌─────────┐
│  Parent │
│ Process │
└────┬────┘
     │
     ├─── fork() ───┐
     │               │
     │          ┌────▼────┐
     │          │ Child 1 │
     │          │  cmd1   │
     │          └────┬────┘
     │               │
     │          [pipe]
     │               │
     ├─── fork() ───┐
     │               │
     │          ┌────▼────┐
     │          │ Child 2 │
     │          │  cmd2   │
     │          └─────────┘
     │
     └─── waitpid() ──── Wait for children
```

### File Descriptor Flow

```
infile (fd) ──dup2──> STDIN_FILENO (Child 1)
                              │
                              │ cmd1 executes
                              │
pipe[1] (fd) ──dup2──> STDOUT_FILENO (Child 1)
     │
     │ [Data flows through pipe]
     │
pipe[0] (fd) ──dup2──> STDIN_FILENO (Child 2)
                              │
                              │ cmd2 executes
                              │
outfile (fd) ──dup2──> STDOUT_FILENO (Child 2)
```

---

## 🔍 How It Works

### Step-by-Step Execution

1. **Argument Validation**
   - Check for exactly 5 arguments (file1, cmd1, cmd2, file2)
   - Verify input file exists and is readable
   - Verify output file can be created/written

2. **File Opening**
   - Open input file in read-only mode
   - Open/create output file in write mode

3. **Pipe Creation**
   - Create pipe using `pipe()` system call
   - Get two file descriptors: read end and write end

4. **First Child Process**
   - Fork to create child process
   - Redirect stdin to input file
   - Redirect stdout to pipe write end
   - Execute first command using `execve()`

5. **Second Child Process**
   - Fork to create second child process
   - Redirect stdin to pipe read end
   - Redirect stdout to output file
   - Execute second command using `execve()`

6. **Parent Cleanup**
   - Close all file descriptors
   - Wait for both children to finish
   - Exit with appropriate status code

### PATH Resolution

The program searches for commands in the PATH environment variable:

```
PATH="/usr/bin:/bin:/usr/local/bin"
Command: "ls"

1. Test: /usr/bin/ls → Found! ✅
2. Return: "/usr/bin/ls"
```

If command contains `/`, it's treated as an absolute path and tested directly.

---

## 📊 Visual Architecture

For detailed architecture diagrams and system call flow, see [ARCHITECTURE.md](ARCHITECTURE.md).

## 📁 Project Structure

```
pipex/
├── includes/
│   └── pipex.h              # Header file with all declarations
├── src/
│   ├── utils/               # String utility functions
│   │   ├── ft_strlen.c
│   │   ├── ft_strncmp.c
│   │   ├── ft_strchr.c
│   │   ├── ft_strdup.c
│   │   ├── ft_substr.c
│   │   ├── ft_strjoin.c
│   │   └── ft_split.c
│   ├── bonus/               # Bonus features
│   │   ├── bonus_main.c
│   │   ├── heredoc.c
│   │   └── multi_pipe.c
│   ├── main.c               # Main program logic
│   ├── args_check.c         # Argument validation
│   ├── path_utils.c         # PATH resolution
│   ├── pipe_utils.c         # Pipe operations
│   ├── exec_utils.c         # Command execution
│   └── error_utils.c        # Error handling
├── Makefile                 # Build configuration
├── CODE_EXPLANATION.md      # Detailed code explanations
└── README.md               # This file
```

---

## 📚 Documentation

### Available Documentation

- **[CODE_EXPLANATION.md](CODE_EXPLANATION.md)** - Comprehensive line-by-line code explanations
  - Function-by-function breakdown
  - System call explanations
  - Memory management details
  - Common evaluation questions
  
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture and design
  - Process flow diagrams
  - File descriptor management
  - Memory flow visualization
  - Error handling flow

### Key Functions

#### `get_cmd_path()`
Finds the full path to a command executable by searching PATH directories.

#### `execute_cmd()`
Parses command string, finds executable path, and executes using `execve()`.

#### `create_pipe()`
Creates a pipe for inter-process communication.

#### `redirect_stdin()` / `redirect_stdout()`
Redirects standard input/output using `dup2()`.

---

## 🧪 Testing

### Automated Testing

Run the test suite:
```bash
make
./test.sh
```

### Manual Test Cases

```bash
# Test 1: Basic functionality
echo "hello world" > test.txt
./pipex test.txt "cat" "wc -w" result.txt
cat result.txt  # Should output: 2

# Test 2: Error handling
./pipex nonexistent.txt "cat" "wc" output.txt
# Should show error message

# Test 3: Command not found
./pipex test.txt "nonexistentcmd" "wc" output.txt
# Should show: command not found

# Test 4: Multiple pipes (bonus)
./pipex_bonus test.txt "cat" "grep hello" "wc -l" result.txt

# Test 5: Heredoc (bonus)
./pipex_bonus here_doc END "cat" "wc -l" result.txt
# Type lines, then "END"
```

### Comparison with Shell

```bash
# Shell command
$ cat file.txt | grep test | wc -l

# Equivalent pipex (bonus)
$ ./pipex_bonus file.txt "cat" "grep test" "wc -l" output.txt
```

---

## 💻 Technologies

- **Language**: C (C99 standard)
- **System Calls**: fork, execve, pipe, dup2, open, close, waitpid, access
- **Build System**: Make
- **Standards**: 42 School Norm (norminette compliant)

---

## 🎓 Learning Outcomes

This project demonstrates understanding of:

- ✅ **Process Management**: Creating and managing child processes
- ✅ **IPC**: Inter-process communication using pipes
- ✅ **File I/O**: File descriptor manipulation and redirection
- ✅ **System Programming**: Direct use of Unix system calls
- ✅ **Memory Management**: Proper allocation and deallocation
- ✅ **Error Handling**: Comprehensive error checking and reporting
- ✅ **Environment Variables**: Parsing and using PATH variable

---

## 📝 Notes

- This project follows **42 School Norminette** coding standards
- All code is commented and documented for evaluation purposes
- Memory leaks are prevented through proper cleanup
- Error handling matches shell behavior

---

## 🤝 Contributing

This is a 42 School project. If you're a 42 student working on this project:

1. **Don't copy this code directly** - You need to understand and write it yourself
2. **Use this as a reference** - Study the concepts and implement your own solution
3. **Learn from the explanations** - The documentation helps understand the concepts

---

## 📄 License

This project is part of the 42 School curriculum. Educational use only.

---

## 👤 Author

**Odeij Amaleddine**
- GitHub: [@odeij](https://github.com/odeij)
- 42: [@ojamaled](https://profile.intra.42.fr/users/ojamaled)

---

<div align="center">

**⭐ If you found this project helpful, consider giving it a star! ⭐**

Made with ❤️ at 42 School

</div>

