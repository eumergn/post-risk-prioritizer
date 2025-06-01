# Risk Manager for Social Network

This project implements a C-based system to manage risky posts on a social network. It efficiently sorts and processes posts based on their risk levels, prioritizing the riskiest ones.

<hr>

## Project Structure

project/  
│  
├── include/  
│   └── tree.h  
│  
├── src/  
│   ├── tree.c  
│   └── main.c  
│  
├── tests/  
│   └── test_tree.c  
│  
└── Makefile  

<hr>

## Compilation

To compile the project, use `make`:
```bash
>  make
```
If you want compile and launch tests, use :
```bash
>  make test
>  # This will generate the `risk_manager` executable in the `bin` directory.
```  

To check for memory leaks using Valgrind, use:
```bash
> make valgrind
```

## Launch menu
This menu create with ncurses is useful for manual testing.
```bash
>  cd bin/
> ./risk_manager <tree_size>
```
You must have ncurses installed on your machine
```bash
>  sudo apt-get install libncurses5-dev libncursesw5-dev
```

<hr>
