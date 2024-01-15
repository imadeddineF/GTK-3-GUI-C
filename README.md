# GTK 3 Linked List Visualizer

This GTK 3 desktop application is designed to visually represent and manipulate a linked list data structure. It offers a modern and interactive user interface, allowing users to perform operations on a linked list and see the results in real-time.

### The libraries and data types used with GTK

**Cairo:** The Cairo graphics library is used for 2D vector graphics and rendering. It's used here for drawing shapes and text on the GTK drawing area.

**Pango:** Pango is a library for laying out and rendering text. It's used for text rendering within the capsules in the linked list visualization.

**g_object:** g_object is not a library but a GObject, which is a fundamental type in GTK programming. It's part of the GObject system, providing a framework for object-oriented programming in C. It's used for managing objects and their properties in GTK applications.

**gint:** gint is a typedef for a signed integer used in GLib and GTK. It's often used for integer data types in GTK applications.

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
