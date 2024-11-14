# Circular Buffer

[Circular Buffer](https://en.wikipedia.org/wiki/Circular_buffer) is a data structure, also knows as circular queue, cyclic buffer or ring buffer.

![Alt text](https://techvidvan.com/tutorials/wp-content/uploads/sites/2/2021/06/TV-Queue-normal-images-06.jpg)

## Characteristics

- Is a array of constant length.
- Store data in a continuous loop.
- When the buffer is filled entries get overridden automatically.
- The last element of the queue is connected to the first element.
- Follows the FIFO(first in, first out) approach.

## How works?
 
The circular buffer has two pointers, commonly called for the implementation Head and Tail.

 - Head (Write): The head pointer increments as the data is added to the buffer. To the actions to add elements to the buffer
 is also refered as write data. Head points to where the next write will occur.

  - Tail (Read): The head pointer increments as the data is removed from the buffer. To the actions to remover elements from the buffer is also refered as read data.Tail points to where the next read will occur.

Buffer is full when the pointers Head and Tail meet.

## Functions

To implement a circular buffer, we need to create the following functions:

1. **AppBuffer_initBuffer():** Function to initialize the buffer.
2. **AppBuffer_writeData():** Function that allows writing to the buffer.
3. **AppBuffer_readData():** Function that allows reading from the buffer.
4. **AppBuffer_isBufferEmpty:** Function that reports if the buffer is empty.

# Implementation in C

For the implementation, we'll use a three-file structure following C programming best practices:

1. [Main.c](Main.c): The main source file that contains:
- Program entry point (main() function)
- Function calls to test the circular buffer
- Required library includes
2. [Circular_buffer.c](Circular_buffer.c): The implementation source file that contains:
- Function implementations
- Required library includes
3. [Circular_buffer.h](Circular_buffer.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards

