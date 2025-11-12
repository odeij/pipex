# 🏗️ Pipex Architecture

## System Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        Parent Process                        │
│                                                              │
│  1. Validate Arguments                                       │
│  2. Open Input/Output Files                                 │
│  3. Create Pipe: pipe(pipe_fd)                             │
│  4. Fork Child 1 ───────────────────────┐                  │
│  5. Fork Child 2 ───────────────────┐   │                  │
│  6. Close File Descriptors          │   │                  │
│  7. Wait for Children               │   │                  │
└──────────────────────────────────────┼───┼──────────────────┘
                                       │   │
                    ┌──────────────────┘   └──────────────────┐
                    │                                          │
        ┌───────────▼───────────┐              ┌───────────────▼──────────┐
        │     Child Process 1   │              │     Child Process 2       │
        │                       │              │                           │
        │ 1. Close pipe[0]      │              │ 1. Close pipe[1]         │
        │ 2. dup2(infile, 0)   │              │ 2. dup2(pipe[0], 0)      │
        │ 3. dup2(pipe[1], 1)  │              │ 3. dup2(outfile, 1)     │
        │ 4. execve(cmd1)       │              │ 4. execve(cmd2)          │
        │                       │              │                           │
        └───────────────────────┘              └───────────────────────────┘
                │                                          │
                │ Data Flow                                │
                │                                          │
        ┌───────▼──────────┐                    ┌─────────▼──────────┐
        │   Input File     │                    │   Output File      │
        │   (file1)        │                    │   (file2)           │
        └──────────────────┘                    └────────────────────┘
```

## Data Flow Diagram

```
┌──────────┐
│  file1   │──read──┐
└──────────┘        │
                    │ STDIN
              ┌─────▼─────┐
              │  Child 1  │
              │   cmd1    │──executes──┐
              └─────┬─────┘            │
                    │ STDOUT           │
                    │                  │
              ┌─────▼─────┐            │
              │   Pipe    │            │
              │  [buffer] │            │
              └─────┬─────┘            │
                    │                  │
                    │ STDIN            │
              ┌─────▼─────┐            │
              │  Child 2  │            │
              │   cmd2    │◄───────────┘
              └─────┬─────┘
                    │ STDOUT
                    │
              ┌─────▼─────┐
              │   file2   │
              └───────────┘
```

## File Descriptor Management

### Initial State
```
Parent Process:
  STDIN_FILENO  (0) → terminal
  STDOUT_FILENO (1) → terminal
  STDERR_FILENO (2) → terminal
  infile        (3) → file1
  outfile       (4) → file2
  pipe[0]       (5) → read end
  pipe[1]       (6) → write end
```

### After Fork - Child 1
```
Child 1:
  STDIN_FILENO  (0) → file1    (after dup2)
  STDOUT_FILENO (1) → pipe[1]  (after dup2)
  pipe[0]       (5) → closed
  pipe[1]       (6) → closed (duplicated to stdout)
```

### After Fork - Child 2
```
Child 2:
  STDIN_FILENO  (0) → pipe[0]  (after dup2)
  STDOUT_FILENO (1) → file2    (after dup2)
  pipe[0]       (5) → closed (duplicated to stdin)
  pipe[1]       (6) → closed
```

## Function Call Hierarchy

```
main()
├── validate_args()
│   ├── open() - input file
│   └── open() - output file
│
├── open() - input file (reopen)
├── open() - output file (reopen)
│
├── create_pipe()
│   └── pipe()
│
├── fork() ──→ Child 1
│   └── first_child_process()
│       ├── redirect_stdin()
│       │   └── dup2()
│       ├── redirect_stdout()
│       │   └── dup2()
│       └── execute_cmd()
│           ├── parse_cmd()
│           │   └── ft_split()
│           ├── get_cmd_path()
│           │   ├── find_path_var()
│           │   ├── ft_split() - PATH
│           │   ├── build_cmd_path()
│           │   │   └── ft_strjoin()
│           │   └── access()
│           └── execve()
│
├── fork() ──→ Child 2
│   └── second_child_process()
│       ├── redirect_stdin()
│       ├── redirect_stdout()
│       └── execute_cmd()
│
├── close_fds()
└── waitpid() × 2
```

## Memory Management

### Allocation Points
1. **ft_split()** - Creates array of strings for PATH directories
2. **ft_split()** - Creates argv array for command arguments
3. **ft_strjoin()** - Builds command paths
4. **ft_strdup()** - Duplicates strings

### Deallocation Points
1. **free_split()** - Frees PATH directory array
2. **free_split()** - Frees command argv array (in exec_utils.c)
3. **free()** - Frees command path string

### Memory Flow
```
get_cmd_path()
  ├── ft_split(PATH) ──→ malloc'd array
  ├── build_cmd_path() ──→ malloc'd string
  └── free_split() ──→ frees array
  Returns: malloc'd string (caller must free)

execute_cmd()
  ├── parse_cmd() ──→ malloc'd argv array
  ├── get_cmd_path() ──→ malloc'd path string
  └── execve() ──→ Process replaced, memory freed by OS
```

## Error Handling Flow

```
System Call
    │
    ├── Success ──→ Continue
    │
    └── Failure ──→ Error Handler
                      │
                      ├── perror_exit() ──→ System errors
                      │   (open, pipe, fork, dup2)
                      │
                      ├── cmd_not_found() ──→ Command not found
                      │   (exit code 127)
                      │
                      └── error_exit() ──→ General errors
                          (exit code 1)
```

## Process States

### Parent Process States
1. **Running** - Validating, opening files, creating pipe
2. **Waiting** - Waiting for children (waitpid)
3. **Terminated** - After children finish

### Child Process States
1. **Running** - Setting up file descriptors
2. **Executing** - Running command (execve)
3. **Terminated** - Command finished

## Inter-Process Communication

### Pipe Buffer
- **Size**: Typically 64KB (system dependent)
- **Behavior**: 
  - Writing blocks when buffer is full
  - Reading blocks when buffer is empty
  - Closing write end signals EOF to reader

### Synchronization
- **Parent waits** for children using `waitpid()`
- **Children execute** concurrently (parallel)
- **Data flows** sequentially through pipe

## Performance Considerations

### Fork Overhead
- Each `fork()` creates a copy of the process
- Copy-on-write optimization reduces memory usage
- Still has overhead for process creation

### Pipe Efficiency
- Kernel buffer provides efficient data transfer
- No disk I/O for inter-process communication
- Minimal copying (kernel handles efficiently)

### File Descriptor Limits
- System has limit on open file descriptors
- Each process inherits parent's open files
- Important to close unused descriptors

---

This architecture ensures:
- ✅ Proper process isolation
- ✅ Efficient data transfer
- ✅ Resource cleanup
- ✅ Error handling at every level
- ✅ Memory safety

