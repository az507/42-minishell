 # Minishell

A simplified Unix shell implemented in C as part of the 42 School curriculum.  
Minishell replicates the behavior of a standard shell, including parsing, executing built-in commands, managing processes, and handling redirections/heredocs and signals.

## Project Overview

Minishell is a project designed to deepen understanding of Unix systems programming, including process control, file descriptors, pipes, and terminal behavior. It recreates the core functionalities of a shell environment, with a focus on proper memory handling, input parsing, and user interaction.

## Features

- Prompt display and interactive command input
- Execution of built-in and external commands
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Argument and quote parsing (`'`, `"`, `$VAR` expansion)
- Redirections: `>`, `>>`, `<`
- Heredocs: `<<`, `EOF` or `'EOF'`
- Pipes: `command1 | command2 | ...`
- Environment variable handling
- Signal handling (`SIGINT`, `SIGQUIT`)
- Exit status propagation `$?`

## Requirements

- GNU/Linux environment or Docker Desktop/Engine
- `make`
- C compiler (compatible with `cc`)
- POSIX-compliant terminal

## Dependencies
Ubuntu:
```bash
sudo apt-get update && sudo apt-get install -y libncurses-dev libreadline-dev
```
## Usage
Docker:
```bash
make docker # host filesystem is bind-mounted at /hostfs in docker container as read-only
```
GNU/Linux:
```bash
git clone https://github.com/az507/42-minishell.git
cd 42-minishell
make
./minishell
