# Circular Buffer

[Circular Buffer](https://en.wikipedia.org/wiki/Circular_buffer) is a data structure, also knows as circular queue, cyclic buffer or ring buffer.

![Alt text](https://techvidvan.com/tutorials/wp-content/uploads/sites/2/2021/06/TV-Queue-normal-images-06.jpg)

## Characteristics

- Is a array of constant length.
- Store data in a continuos loop.
- When the buffer is filled entries get overridden automatically.
- The last element of the queue is connected to the first element.
- Follows the FIFO(first in, first out) approach.

## How works?
 
The circular buffer has two pointers, commonly called for the implementation Head and Tail.

 - Head (Write): The head pointer increments as the data is added to the buffer. To the actions to add elements to the buffer
 is also refered as write data.

  - Tail (Read): The head pointer increments as the data is removed from the buffer. To the actions to remover elements from the buffer is also refered as read data.

Buffer is full when the pointers Head and Tail meet.

## Operations 

To implement a circular buffer we are going to need to create the following operations:

1. **AppBuffer_writeData():** Interface that allows writing to the buffer.
2. **AppBuffer_writeData():** Interface that allows writing to the buffer.
3. **AppBuffer_readData():** Interface that allows reading from the buffer.
4. **AppBuffer_isBufferEmpty:** Interface that reports if the buffer is empty.