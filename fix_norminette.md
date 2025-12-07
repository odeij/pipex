# Norminette Fixes Needed

## Critical fixes (functionality):
1. exec_utils.c - Memory leak fix (free argv before cmd_not_found)
2. bonus_main.c - Heredoc append mode
3. multi_pipe.c - Heredoc command index fix

## Norminette compliance:
1. Remove trailing newlines at EOF (all files)
2. Fix header line 4 too long (all files)
3. Shorten function names over 80 chars
4. Split functions over 25 lines
5. Fix heredoc.c - too many vars, assignment in control

Files to fix:
- main.c: first_child_process → first_child, second_child_process → second_child
- heredoc.c: read_line() - split function, fix assignment in while
- path_utils.c: get_cmd_path() - split function
- ft_strjoin.c: ft_strjoin() - split function
- bonus_main.c: validate_bonus_args() - split function
- multi_pipe.c: execute_multi_pipe() - split function
