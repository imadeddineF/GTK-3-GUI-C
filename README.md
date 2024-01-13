# GTK 3 Linked List Visualizer

This GTK 3 desktop application is designed to visually represent and manipulate a linked list data structure. It offers a modern and interactive user interface, allowing users to perform operations on a linked list and see the results in real-time.

## Features

**Dynamic Linked List Operations:** Add, remove, and search for elements in the linked list.

**Modern User Interface:** Styled with CSS for a sleek and responsive design.

**Real-time Visual Feedback:** Visualize changes in the linked list as they happen.

**Interactive Animations:** Special animations during search operations for enhanced user experience.

## Technology Stack

**GTK+ 3:** For creating the graphical user interface.

**C Programming Language:** The core logic of the application.

## Compilation Instructions:

To compile the GTK-3 code, use the following command:

```
gcc $(pkg-config --cflags gtk+-3.0) -o main main.c $(pkg-config --libs gtk+-3.0)
```

## Execution Instructions

After compiling, run the application with:

```
./main
```
