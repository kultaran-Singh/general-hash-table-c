# Generic Hash Table in C

A simple, robust, and generic hash table implementation written in pure C. This project provides a classic key-value store that uses external chaining with a linked list to handle hash collisions. It is designed to be reusable and flexible, allowing for any data type to be used for both keys and values through the use of `void*` pointers.

This project was built as a practical exercise in C programming, focusing on manual memory management, pointer manipulation, and the implementation of fundamental data structures.

## Features

* **Generic Key-Value Storage**: Uses `void*` pointers to store keys and values of any data type.
* **External Chaining**: Resolves hash collisions by storing entries in a linked list at each bucket. (This implementation uses the `general-linked-list-c` library as a dependency).
* **Customizable Behavior**: Accepts user-defined functions for hashing, key comparison, and memory deallocation via a configuration struct.
* **Automatic Resizing**: The hash table automatically doubles in size when the load factor exceeds 50% to maintain average O(1) performance for insertions and lookups.
* **Clean and Documented API**: Provides a simple interface for creating, inserting into, retrieving from, removing from, and destroying the hash table.

## File Structure

The project is organized to separate the library code from the demonstration program.

general-hash-table-c/
├── lib/
│   ├── linked_list.c       # Dependency: Generic Linked List implementation
│   └── linked_list.h
├── src/
│   ├── hash_table.c        # Hash Table implementation
│   └── hash_table.h
├── main.c                  # A demonstration program showing how to use the library
├── Makefile                # Compiles the project and the demo
└── README.md