# smpl

smpl is a superset of [brainfuck](https://esolangs.org/wiki/Brainfuck) with 3 additional operators for dynamic memory allocation and management. This allows smpl to compilation from much higher level languages, as implementations for patterns like `vector` and `string` are possible.

# Operators

| Command | Description |
|---------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| > | Move the pointer to the right |
| < | Move the pointer to the left |
| + | Increment the memory cell under the pointer |
| - | Decrement the memory cell under the pointer |
| . | Output the character stored at the cell under the pointer |
| , | Input a character and store it in the cell under the pointer |
| [ | Jump past the matching `]` if the cell under the pointer is zero |
| ] | Jump back to the matching `[` if the cell under the pointer is not zero |
| * | Set the pointer equal to the value of the current cell |
| & | Set the pointer back to the value it was before the last `*` |
| ? | With the value of the cell under the pointer, store the address of the first instance of that many consecutive zeros from the left of the tape at the current cell |

# Tape Attributes

The user should _*ALWAYS*_ be able to set tape length for each implementation of smpl. A default value for the tape length may be used, but _*DO NOT*_ bar the user from setting it.

Cells *must* be unsigned 32 bit integers, and the pointer is an unsigned 32 bit integer as well.

# Extreme Behavior

Extreme behavior describes behavior that is very important to keep note of while implementing smpl. If the implementation does not implement extreme behavior properly, then it is not a valid implmentation of smpl.
The goal of documenting extreme behavior is to help guarantee that all implementations of smpl are compatible. Keeping note of the very specific behaviors of smpl helps ensure this.

- When the `&` operator is called before the `*` operator has been called, the pointer jumps to cell `0`
- When a cell's value is `0` and the `-` operator is called, the cell overflows to the maximum value a 32 bit integer can store.
- After the `,` operator first returns `EOF`, it will continue to return `EOF` every time it is called until the program ends.
- `?` will replace the `size` parameter of the current cell with the address of free memory with that size. The address stored in the current cell will point to the _*leftmost*_ cell in that block of memory.
- The `.` operator will convert the value in the cell to a character and print the character. If the number is greater than `255`, the `.` operator will divide by `256` and print the remainder as a character.
- The `&` operator will only record the previous `256` `*` calls.

# Undefined Behavior / Errors

Undefined behavior is EXTREMELY frowned upon. If possible, make undefined behavior throw errors in your implementation.

Undefined behavior includes
- Overflowing the pointer
- Setting / Incrementing the pointer past the length of the tape
- Attempting to allocate an unavailable amount of memory with the `?` operator
