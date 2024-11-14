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

 - Head (Write-Front): The head pointer increments as the data is added to the buffer. To the actions to add elements to the buffer
 is also refered as write data. Head points to where the next write will occur.

  - Tail (Read-Rear): The head pointer increments as the data is removed from the buffer. To the actions to remover elements from the buffer is also refered as read data.Tail points to where the next read will occur.

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

# Circular_buffer.h

## Data structure definition

```c
typedef struct
{
    unsigned char *Buffer;              /*!< Array of values */ 
    unsigned long Elements;             /*!< How many elements we have in the buffer */ 
    unsigned long Head;                 /*!< Write - Front of the array */
    unsigned long Tail;                 /*!< Read - Rear of the array*/ 
    unsigned char Empty;                /*!< Flag Empty  */ 
    unsigned char Full;                 /*!< Flag Full */
    unsigned char Head_wrap;            /*!< Flag when Head is wrap */
    unsigned char Tail_wrap;            /*!< Flag when Head is wrap */
} AppBuffer_Buffer;
```
This the structure to handle the buffer and its pointers.

- **unsigned char *Buffer :** The buffer is a pointer to a byte array.
- **Tail_wrap and Head_wrap:** A flag indicating if the head/tail has wrapped around to the beginning of the buffer.


# Circular_buffer.c

## Initialize the circular buffer

```c
void AppBuffer_initBuffer( AppBuffer_Buffer *hbuffer )
{
    hbuffer ->  Full = FALSE;       /* The buffer is not full */
    hbuffer ->  Empty = TRUE;       /* The buffer doesnt have any elements */
    hbuffer ->  Head = 0;           /* Head in the position 0 */
    hbuffer ->  Tail = 0;           /* Tail in the position 0 */
    hbuffer ->  Head_wrap = FALSE;      /* Flag when Head is wrap */
    hbuffer ->  Tail_wrap = FALSE;      /* Flag when Tail is wrap */
}
```
In this function, we initialize several elements of the data structure:
- Since no data has been written, the Full flag is set to FALSE and the Empty flag is set to TRUE.
- The Head and Tail pointers are both set to 0.
- The Head_wrap and Tail_wrap flags are set to FALSE as the conditions for them to be TRUE have not occurred.

## Write in the circular buffer

Write a new 8-bit data to the buffer if there is available space; if there is no space,
no data will be written. Function will determine if the queue is full with the last data written.

```c
void AppBuffer_writeData( AppBuffer_Buffer *hbuffer, unsigned char data )
{
    if ( ((hbuffer -> Tail) == (hbuffer -> Head)) && (hbuffer -> Head_wrap != hbuffer -> Tail_wrap))
    {
        return; 
    }
    else
    {
        hbuffer -> Buffer[hbuffer -> Head] = data; /* Save the data */
        if((hbuffer -> Head + 1) == (hbuffer -> Elements))
        {
            hbuffer -> Head_wrap = 1;
            hbuffer -> Head = 0;
        } 
        else
        {
            hbuffer -> Head = (hbuffer -> Head + 1) % hbuffer -> Elements; /* Move the Head */
        }
    }
}
```

### Explanation of the Write Function

#### Check if the buffer is full

First we need to check if the buffer has space to write a element. For that we need to do the following evaluation:

```c
if (((hbuffer->Tail) == (hbuffer->Head)) && (hbuffer->Head_wrap != hbuffer->Tail_wrap)) 
{ 
    return; /* Exit the function if the buffer is full */
}
```
- The buffer is considered full if the Head and Tail pointers are equal and the Head_wrap and Tail_wrap flags are different.
- When the buffer is initialized, Head and Tail have the same value, but this does not mean the buffer is full. The Head_wrap and Tail_wrap flags help distinguish between an empty and a full buffer.


#### Write data to the buffer
```c
hbuffer->Buffer[hbuffer->Head] = data;
```
- If the buffer is not full, the data is written to the position indicated by the Head pointer.

#### Update the head pointer
```c
    hbuffer -> Buffer[hbuffer -> Head] = data; /* Save the data */
        
    if((hbuffer -> Head + 1) == (hbuffer -> Elements))
    {
        hbuffer -> Head_wrap = 1; /* Set the head wrap flag */
        hbuffer -> Head = 0; /* Wrap the head to the beginning */
    } 
    else
    {
        hbuffer -> Head = (hbuffer -> Head + 1) % hbuffer -> Elements; /* Move the Head */
    }

```

- If the Head pointer reaches the end of the buffer, it wraps around to the beginning and the Head_wrap flag is set to TRUE.
- Otherwise, the Head pointer is incremented.