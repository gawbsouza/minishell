# 42-Minishell

Minishell is a miniature shell implementation developed as part of the 42 curriculum. It provides a simple command-line interface with support for pipes, redirections, and various builtin commands.

## Features

### Builtin Commands
The shell includes the following builtins:
- `echo` with `-n` option
- `cd` with relative or absolute paths
- `pwd` (print working directory)
- `export` (set environment variables)
- `unset` (remove environment variables)
- `env` (list environment variables)
- `exit` (terminate the shell)

### Operators and Redirections
Support for complex command structures using:
- `|` (Pipes): Pipe the output of one command to the input of the next.
- `<` (Input Redirection): Read input from a file.
- `>` (Output Redirection): Overwrite output to a file.
- `<<` (Here-doc): Read input until a specific delimiter is encountered.
- `>>` (Append Redirection): Append output to a file.

### Other Functionalities
- **Environment Variables**: Expansion of `$VAR` to its corresponding value.
- **Quoting**: 
  - `'` (Single quotes): Prevents the shell from interpreting meta-characters.
  - `"` (Double quotes): Prevents interpretation except for `$`.
- **Signals**: 
  - `Ctrl-C`: Interrupts the current process and displays a new prompt.
- `Ctrl-D`: Exits the shell (EOF).
  - `Ctrl-\`: Does nothing in the main prompt.

## Examples

To demonstrate Minishell's capabilities, here are some common usage scenarios:

### Pipes and Simple Commands
```bash
ls -l | grep ".c" | wc -l
```

### Redirections
```bash
# Redirecting output to a file (overwrite and append)
echo "Hello Minishell" > info.txt
echo "Adding more info" >> info.txt

# Redirecting input from a file
cat < info.txt
```

### Here-Doc
```bash
# Read input until 'END'
cat << END
This is a multiline
input in Minishell
END
```

### Environment Variables and Quoting
```bash
export USERNAME="gabriel"
echo "Hello $USERNAME"    # Output: Hello gabriel
echo '$USERNAME'          # Output: $USERNAME
```

## Norminette Compliance
This project follows the [Norminette](https://github.com/42School/norminette) coding standards of the 42 School, ensuring consistent and clean code architecture.

## Requirements

To compile and run this project, you need the following dependencies:

### Readline
Used for line editing and history management.

**Installation (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install libreadline-dev
```

## Compilation

Compile the project using the provided `Makefile`:
```bash
make
```

## Usage

After compilation, run the executable:
```bash
./minishell
```

## Testing
To run the automated tests:
```bash
make test
```
Or use the provided test scripts in the `tests/` directory.
