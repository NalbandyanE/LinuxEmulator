"# LinuxEmulator" 
The Linux Emulator Project is a simple command-line interface (CLI) application that simulates a basic Linux terminal environment. It allows users to execute various commands, navigate the file system, and interact with a virtual Linux server.

## Getting Started

### Prerequisites

- C++ compiler (C++11 or later)
- Git (optional, for cloning the repository)

### Installation

1. Clone the repository (if you haven't already):

2. Compile the source code:

To run the Linux Emulator, execute the compiled binary:

Follow the on-screen instructions to interact with the Linux Emulator.

## Features

- Command execution: Execute various Linux commands.
- File System: Navigate and manipulate a simulated file system.
- Virtual Terminal: Access a virtual Linux server using the "ssh" command.
- Command History: View the history of executed commands.

## Example Commands

- `ls`: List files and directories in the current directory. (-l, -la, -lt)
- `cd <directory>`: Change the current directory.
- `pwd`: Display current working directory.
- `mkdir <directory>`: Create a new directory.
- `touch <file>`: Create a new file.
- `rm <file>`: Remove a file.
- `rmdir <directory>`: Remove an empty directory.
- `cp <source> <destination>`: Copy a file or directory.
- `mv <source> <destination>`: Move or rename a file or directory.
- `cat <file>`: Display the contents of a file.
- `head <count> <file>`: Display first count lines of file.
- `tail <count> <file>`: Display last count lines of file.
- `wc <file>`: Display count of lines, words and characters in file.
- `file <file>`: Display format of file.
- `vim <file>`: Create a new file and write content in it.
- `chmod <permissions> <file>`: Change the access permissions.
- `echo <text>`: Display line of text/string that are passed as an argument. 
- `useradd <username>`: Add new user.
- `passwd`: Change user's password.
- `id`: Display user's user id and group id.
- `whatis <command>` : Display one-line manual page description of command.
- `date`: Current date and time.
- `cal`: Calendar of current month.
- `df`: Free space amount on disk.
- `free`: Free memory space.
- `help`: List of valid commands.
- `history`: Display history the previously executed commands.
- `clear`: Clear the terminal screen.
- `ln <file> <link>`: Create hard link of file.
- `ssh <username>@<server>`: Access a virtual server with password "1111".

## Contributing

Contributions are welcome! If you find a bug or have an improvement idea, please open an issue or submit a pull request.


## Acknowledgments

- The project was inspired by the Unix/Linux command-line interface.
