🐚 **THE MINISHELL** 🐚

In this repository you will find **the minishell**, an implementation of bash-like command interpreter written from scratch in C!

Don't let the name fool you though - it is one of the biggest and most demanding project of 42 school curriculum, which introduces and reinforces concepts such as *processes, file descriptors, signals, parsing, memory management, environment management*, among others.

It was done over the course of approximately 7 weeks at Hive Helsinki, together with the parsing master and the lord of the signals - **[Leo Laaksonen](https://github.com/llaakson)**.

You will need GCC or Clang as well as GNU Readline library.

In order to build it, open terminal and:
```bash
git clone https://github.com/huskyhania/minishell
cd minishell
make
```
Later on just type ```./minishell``` and enjoy all it has to offer!

## ✨ Key features

- Bash-like prompt with line editing and history (via `readline`)
- Command parsing with syntax tree
- Support for:
  - Pipes (`|`)
  - Redirections (`>`, `<`, `>>`, `<<`)
  - Environment variable expansion (`$HOME`)
  - Quotes: `"double"` and `'single'`
- Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`
- Signal handling: `Ctrl+C`, `Ctrl+\`, EOF
- Heredocs with proper signal behavior
- Accurate bash-style exit codes and error messages
- Memory and file descriptor leak-free (verified with Valgrind)

## 🧩 Wanna know more?

1. The work was divided in two main parts - parsing and execution.

2. For the parsing part, user input is taken with the help of readline functions. History of previously used commands is available to the user.

3. User input is validated, tokenized and used to populate a syntax tree.

4. Double and single quotes are handled exactly as in bash and environment variables are expanded (when necessary). You can test quotes with in quotes or see how expansion behaves with single and double quotes.

5. The nodes of syntax tree contain information about the command (or commands), as well as input and output redirections (if any).

6. Recursion is used to traverse the syntax tree and execute commands in order they were provided by the user (from left to right).

7. Environment variables are inherited from the original shell you run the program in, and stored in a linked list, but you can also add and delete them freely, as well as run the shell without any. You can safely try to expand non-existent variables.

8. Our minishell has several builtin commands: **echo** (and echo -n), **export, unset, env, cd, pwd** and **exit**. They behave exactly as in bash, handle redirections, etc. Their behaviour mimics bash also when it comes to a pipeline of commands.

9. Signal handling of SIGINT, SIGQUIT and EOF is implemented and its behaviour reflects bash both when testing single commands, a pipeline of commands and heredocs.

10. User is able to create multiple heredoc files, both for single and multiple commands.

11. Exit codes mimic those in bash and reflect the order of operations.

12. Error messages are informative and bash-like, they encompass file descriptor issues, syntax errors, ambiguous redirections, permissions, commands not found, among others.

13. No memory or file descriptor leaks were ever found ;)

## ⚠️ Known limitations:

- No support for wildcard expansion (`*`)
- No logical operators (&&, ||)
