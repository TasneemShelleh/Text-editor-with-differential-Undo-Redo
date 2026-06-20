# Text Editor with Differential Undo/Redo

This project is a C-based text editor that supports differential Undo and Redo operations using data structures. The editor loads text from an input file, allows the user to insert or remove words, tracks every change, and saves the final edited text to an output file.

The project demonstrates the practical use of stacks and queues in text editing operations.

## Project Overview

The main goal of this project is to implement a simple text editor that can:

* Load text from an input file
* Display the current text
* Insert new strings into the text
* Remove strings from the text
* Undo previous editing operations
* Redo previously undone operations
* Print the Undo and Redo stacks
* Save the updated text to an output file

The program uses a menu-based interface that allows the user to choose the operation they want to perform.

## Data Structures Used

### 1. Undo Stack

The Undo Stack stores all changes made to the text. Each node contains:

* The word or string involved in the operation
* The operation type, such as insert or remove
* The position/index where the operation happened

When the user performs an Undo operation, the most recent change is removed from the Undo Stack and moved to the Redo Stack.

### 2. Redo Stack

The Redo Stack stores operations that were undone by the user.

When the user performs a Redo operation, the most recent undone change is removed from the Redo Stack and applied again to the text.

### 3. Add Queue

The Add Queue temporarily stores words from an inserted string. These words are then added to the text and stored in the Undo Stack.

## Repository Files

```text
Text-editor-with-differential-Undo-Redo/
│
├── p2_1220439_TasneemShelleh_3.c   # Main C source code
├── input.txt                       # Input file containing the initial text
├── Output.txt                      # Output file containing the modified text
├── Project+2.pdf                   # Project description and requirements
└── README.md                       # Project documentation
```

## Features

* File input and output handling
* Menu-driven program
* Insert text operation
* Remove text operation
* Undo operation
* Redo operation
* Print current text
* Print Undo Stack
* Print Redo Stack
* Save modified text to an output file
* Stack implementation using linked lists
* Queue implementation for inserted strings

## Program Menu

The application provides a menu similar to the following:

```text
1. Load the input file
2. Print the loaded text
3. Insert strings to the text
4. Remove strings from the text
5. Perform Undo operation
6. Perform Redo operation
7. Print the Undo Stack and Redo Stack
8. Save the updated text to the output file
9. Exit
```

## How the Program Works

### Loading Text

The program starts by reading the initial text from `input.txt` and storing it in memory.

### Inserting Text

When the user inserts a string, the words are first stored in the Add Queue. Then, the words are added to the text and recorded in the Undo Stack as insert operations.

### Removing Text

When the user removes a string, the program searches for the first matching occurrence in the text. If the string is found, it is deleted and the remove operation is saved in the Undo Stack with its original position.

### Undo Operation

Undo reverses the most recent operation.

* If the last operation was insert, Undo removes the inserted word.
* If the last operation was remove, Undo restores the removed word.

The undone operation is then pushed into the Redo Stack.

### Redo Operation

Redo reapplies the most recently undone operation.

* If the Redo operation was insert, the word is inserted again.
* If the Redo operation was remove, the word is removed again.

The redone operation is moved back to the Undo Stack.

### Saving Text

When the user chooses to save, the final updated text is written to `Output.txt`. After saving, the Undo and Redo stacks are cleared.

## Example

Initial text in `input.txt`:

```text
Welcome to Data Structures and Algorithms.
```

If the user inserts:

```text
Course code is COMP 2421.
```

The updated text becomes:

```text
Welcome to Data Structures and Algorithms. Course code is COMP 2421.
```

If the user removes:

```text
Algorithms
```

The text becomes:

```text
Welcome to Data Structures and. Course code is COMP 2421.
```

The removed word is stored in the Undo Stack with its operation type and position, allowing it to be restored later using Undo.

## How to Compile and Run

### Using GCC

Compile the C file:

```bash
gcc p2_1220439_TasneemShelleh_3.c -o text_editor
```

Run the program:

```bash
./text_editor
```

### On Windows

Compile:

```bash
gcc p2_1220439_TasneemShelleh_3.c -o text_editor.exe
```

Run:

```bash
text_editor.exe
```

## Requirements

* C compiler, such as GCC
* `input.txt` file in the same project folder
* Basic terminal or command prompt

## Technologies Used

* C programming language
* Linked lists
* Stack data structure
* Queue data structure
* File handling
* Dynamic memory allocation

## Learning Outcomes

This project demonstrates:

* How stacks can be used to implement Undo and Redo
* How queues can be used to process inserted words
* How linked lists can be used to build dynamic data structures
* How file input/output is handled in C
* How text editing operations can be represented as differential changes

## Author

Tasneem Shelleh

