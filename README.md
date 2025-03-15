# OS-Linux-Kernel

A Linux project demonstrating process hierarchy creation and memory map analysis, combining a kernel module and user-space program.

## Features

- **Kernel Module**
  - Logs module load/unload events to the kernel ring buffer
  - Demonstrates basic kernel module development
  - Uses proper kernel memory management practices

- **User-Space Program**
  - Creates a hierarchical process tree (parent → children → grandchildren)
  - Dynamically allocates memory in each process
  - Generates process tree visualization with memory maps
  - Automatically cleans up child processes

## Prerequisites

- Ubuntu 22.04 LTS (or later)
- VMware Fusion/VirtualBox (for Mac users)
- Build essentials:
  ```bash
  sudo apt install build-essential gcc make linux-headers-$(uname -r)
  ```

- Kernel development tools
- Root/sudo access

## Installation
**1. Clone the repository:**
```bash
git clone https://github.com/your-username/jackfruit-problem.git
cd jackfruit-problem
```
**2. Compile components:**
```bash
#1. Create a project directory
mkdir ~/kernel_project && cd ~/kernel_project
```

```bash
#2. Write down the Kernel Module (in Ubuntu Terminal)
nano process_tracker.c
```

```bash
#3. Write down the code in the Makefile (in Ubuntu Terminal)
nano Makefile
```

```bash
#4. This generates process_tracker.ko.
make
```

```bash
#5. Check if the files are there in the directory or not
ls -l
```

```bash
#6. Load the module
sudo insmod process_tracker.ko
```

```bash
#7. Verify (check for "Kernel Module Loaded" message)
sudo dmesg | tail
```

```bash
#8. Type/Paste the User Program Code after the following command
nano process_tree.c
```

```bash
#9. Verify if the code is present
cat process_tree.c
```
```bash
#10. Compile and Run
  #10a.Compile
gcc process_tree.c -o process_tree

  #10b.Run (no sudo needed)
./process_tree
```
## Final Output
-**The final output should look like:**
<img width="1432" alt="1-OS" src="https://github.com/user-attachments/assets/ee06c424-a23a-4396-8b91-c89b713d6ff8" /># OS-Linux-Kernel Process Tracking System

