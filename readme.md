# 🐚 Simple Shell in C

A lightweight custom shell built in C that supports:

✅ Command execution  
✅ Input/Output redirection (`<`, `>`)  
✅ Background processes (`&`)  
✅ Built-in `cd` and `exit` commands  
✅ Basic zombie process cleanup

This project is designed to help you understand how `fork()`, `execvp()`, and process management work in UNIX/Linux systems.

---

## 📁 Features

| Feature | Description |
|---------|-------------|
| **Command execution** | Runs external commands using `execvp()` |
| **Input redirection (`<`)** | Reads input from a file instead of standard input |
| **Output redirection (`>`)** | Redirects output to a file instead of the terminal |
| **Background processes (`&`)** | Runs processes in the background (non-blocking) |
| **Change directory (`cd`)** | Built-in command to switch directories |
| **Exit command (`exit`)** | Gracefully terminates the shell |
| **Zombie cleanup** | Uses `waitpid(-1, NULL, WNOHANG)` to prevent zombie processes |

---

## ⚙️ Compilation

Use `gcc` to compile the program:

```bash
gcc src/main.c
```

---

## ▶️ Running the Shell

After compilation, run your shell using:

```bash
./a.out
```

You'll see a prompt like:

```
=== Simple Shell with I/O Redirection ===
Commands:
  ls                    - list files
  ls > output.txt       - save output to file
  cat < input.txt       - read from file
  cat < in.txt > out.txt - read from in.txt, write to out.txt
  echo hello > file.txt - write to file
  ls &                  - background process
  cd <directory>        - change directory
  exit                  - quit shell

mysh:/home/user>
```

---

## 💡 Examples

| Command | Description |
|---------|-------------|
| `ls` | List files in current directory |
| `ls > files.txt` | Save list of files to `files.txt` |
| `cat < input.txt` | Display content of `input.txt` |
| `cat < input.txt > output.txt` | Copy content of `input.txt` into `output.txt` |
| `echo Hello > hello.txt` | Write "Hello" to `hello.txt` |
| `ls &` | Run `ls` in background |
| `cd /home` | Change directory to `/home` |
| `exit` | Exit the shell |

---

## 🧠 How It Works

### 1. User Input
- Reads input using `fgets()` and removes the trailing newline.
- Detects background (`&`) and I/O redirection symbols (`<`, `>`).

### 2. Parsing
- Uses `strtok()` to split input into command tokens.
- Detects files for redirection and stores their names separately.

### 3. Execution
- Forks a new process:
  - **Child:** handles I/O redirection using `dup2()`, then runs command with `execvp()`.
  - **Parent:** waits for the child unless it's a background process.

### 4. Zombie Cleanup
- Calls `reap_zombies()` before each prompt to clean up finished background processes.

---

## 🧩 Code Highlights

### 🔁 Zombie Cleanup
```c
void reap_zombies() {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
```

### 🔄 Input/Output Redirection
```c
int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd_out, STDOUT_FILENO);
```

### 🧍 Background Execution
```c
if (isBackground == 0)
    waitpid(pid, NULL, 0);
else
    printf("[Background process started with PID %d]\n", pid);
```

---

## 📚 Learning Goals

This project helps you learn:

- Process creation with `fork()`
- Executing programs with `execvp()`
- File descriptor manipulation using `dup2()`
- Waiting for processes (`waitpid`)
- Basic shell parsing logic
