# Queue Buffer

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

To implement a Queue buffer, we need to create the following functions:

1. **AppQueue_initQueue:** Function to initialize the buffer.
2. **AppQueue_writeData:** Function that allows writing to the buffer.
3. **AppQueue_readData:** Function that allows reading from the buffer.
4. **AppQueue_isQueueEmpty:** Function that reports if the buffer is empty.
5. **AppQueue_flushQueue:** Function that empties the buffer.

# Implementation in C

For the implementation, we'll use a three-file structure following C programming best practices:

1. [Main.c](Main.c): The main source file that contains:
- Program entry point (main() function)
- Function calls to test the queue
- Required library includes
2. [Queue.c](Queue.c): The implementation source file that contains:
- Function implementations
- Required library includes
3. [Queue.h](Queue.h):The header file that contains:
- Structure definitions
- Function declarations (prototypes)
- Constant definitions
- Include guards

# Queue.h

## Data structure definition

```c
typedef struct
{
    void        *Buffer;                /*!< Pointer to array that store buffer data - 40 bits*/
    uint32_t    Elements;               /*!< Number of elements to store (the queue lenght) */
    uint8_t     Size;                   /*!< Size of the elements to store */
    uint8_t     Head;                   /*!< Variable to signal the next queue space to write */
    uint8_t     Tail;                   /*!< Variable to signal the next queue space to read */
    uint8_t     Empty;                  /*!< Flag to indicate if the queue is empty */
    uint8_t     Full;                   /*!< Flag to indicate if the queue is full */
    uint8_t     Head_wrap;              /*!< Flag when Head is wrap */
    uint8_t     Tail_wrap;              /*!< Flag when Head is wrap */  
} AppQue_Queue;
```
This the structure to handle the buffer and its pointers.

- **void *Buffer :** The buffer is a void pointer, which means it can point to any data type.
- **Tail_wrap and Head_wrap:** A flag indicating if the head/tail has wrapped around to the beginning of the buffer.


# Queue.c

## Initialize the queue buffer

```c
void AppQueue_initQueue( AppQue_Queue *queue )
{
    queue ->  Full = FALSE;         /* The buffer is not full */
    queue ->  Empty = TRUE;         /* The buffer doesnt have any elements */
    queue ->  Head = 0;             /* Head in the position 0 */
    queue ->  Tail = 0;             /* Tail in the position 0 */
    queue ->  Head_wrap = FALSE;    /* Flag when Head is wrap */
    queue ->  Tail_wrap = FALSE;    /* Flag when Tail is wrap */
}
```
In this function, we initialize several elements of the data structure:
- Since no data has been written, the Full flag is set to FALSE and the Empty flag is set to TRUE.
- The Head and Tail pointers are both set to 0.
- The Head_wrap and Tail_wrap flags are set to FALSE as the conditions for them to be TRUE have not occurred.

## Write to the queue buffer

Write a new data to the buffer if there is available space; if there is no space, no data will be written. The function will determine if the queue is full with the last data written.

```c
uint8_t AppQueue_writeData( AppQue_Queue *queue, void *data )
{
    uint8_t write_status;
    /* If the queue is full we CAN NOT add more elements */
    if (((queue -> Tail) == (queue -> Head)) && (queue -> Head_wrap != queue -> Tail_wrap))
    {   
        queue -> Full = TRUE;               /* Set Full flag TRUE */
        write_status = FALSE;               /* The write WAS NOT successful */
    } 
    else 
    {   /* Queue is NOT FULL*/
        queue -> Full = FALSE;              /* Set Full flag FLAG FALSE*/
        void *write_position = (uint8_t *)queue -> Buffer + (queue -> Head * queue -> Size);
        memcpy(write_position, data, queue -> Size); /* Copies a data from a source to a destination */

        if((queue -> Head + 1) == (queue -> Elements))
        {   
            queue -> Head_wrap = 1;
            queue -> Head = 0;
        }
        else
        {
            queue -> Head = (queue -> Head + 1) % queue -> Elements; /* To move the Head */
        }
        write_status = TRUE;    /* The write WAS successful */
    }
    return write_status;
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
    void *write_position = (uint8_t *)queue -> Buffer + (queue -> Head * queue -> Size);
    memcpy(write_position, data, queue -> Size); /* Copies a data from a source to a destination */
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

## Check if the queue buffer is empty
The function returns one if there are no more elements that can be read from the queue buffer, and zero if there is at least one element that can be read. It is necessary to use this function before using the read function.

```c
uint8_t AppQueue_isQueueEmpty( AppQue_Queue *queue )
{
    uint8_t status;

    if((queue -> Tail) == (queue -> Head) && (queue -> Head_wrap == queue -> Tail_wrap))
    {   
        queue -> Empty = TRUE;          /* Set Empty flag TRUE */
        status = TRUE;                  /* There are no more elements that can be read from the queue */
    } 
    else
    {   
        queue -> Empty = FALSE;         /* Set Empty flag FALSE */
        status = FALSE;                 /* There are elements that can be read from the queue */
    }
    return status;   
}
```
### Explanation of the Empty Function
- The buffer is considered empty if the Tail and Head pointers are equal and the Head_wrap and Tail_wrap flags are also equal.
- If both conditions are met, the status is set to TRUE (buffer is empty).
- Otherwise, the status is set to FALSE (buffer is not empty).
- The function returns the status indicating whether the buffer is empty or not.

## Read from the queue buffer
Reads a data from the buffer, the data that is read will no longer exist within the buffer.
If the buffer is empty, no data will be read, and the value returned by the function will not be valid.
Function will determine if the queue is empty with the last data read.

```c
uint8_t AppQueue_readData( AppQue_Queue *queue, void *data )
{
    uint8_t read_status;

    /* If the Buffer is empty */
    if (AppQueue_isQueueEmpty(queue) == TRUE)
    {   
        read_status = FALSE;      /* Read WAS NOT successful */
    } 
    else
    {
        void *read_position = (uint8_t*)queue -> Buffer + (queue -> Tail * queue -> Size);
        memcpy(data, read_position, queue -> Size);
        if ((queue -> Tail + 1) == (queue -> Elements))
        {
            queue -> Tail_wrap = 1;     /* SET FLAG TAIL WRAP */
            queue -> Tail = 0;           
        }
        else
        {
            queue -> Tail = (queue -> Tail + 1) % queue -> Elements;
        }
        read_status = TRUE;      /* Read WAS successful*/
    }
    return read_status;
}
```
- The function returns the result which contains the data read from the buffer.

### Explanation of the Read Function
#### Check if the buffer is empty
```c
if (AppQueue_isQueueEmpty(queue) == TRUE) 
{ 
    read_status = FALSE; /* The buffer is empty */ 
}
```
- The function first checks if the buffer is empty by calling AppQueue_isQueueEmpty.
- If the buffer is empty, read_status is set to FALSE.

#### Read Data from the Buffer
```c
void *read_position = (uint8_t*)queue->Buffer + (queue->Tail * queue->Size);
 memcpy(data, read_position, queue->Size);
```
- If the buffer is not empty, the data at the Tail pointer is read and stored in result.

#### Update the Tail Pointer
```c
if((hbuffer -> Tail + 1) == ( hbuffer -> Elements))
{
    hbuffer -> Tail_wrap = 1;
    hbuffer -> Tail = 0; /* Set flag Tail wrap */    
}
else
{
    hbuffer -> Tail = (hbuffer -> Tail + 1) % hbuffer -> Elements;
}
```
- If the Tail pointer reaches the end of the buffer, it wraps around to the beginning and the Tail_wrap flag is set to TRUE.

- Otherwise, the Tail pointer is incremented.

## Flush the Queue
The function returns one if there are no more elements that can be read from the circular buffer,
and zero if there is at least one element that can be read. It is necessary to use this function
before using the read function.

```c
void AppQueue_flushQueue( AppQue_Queue *queue )
{
    AppQueue_initQueue(queue); /* Reinitialize the queue to its initial state */

}
```


### Explanation of the Flush Function
- The function calls AppQueue_initQueue to reset the queue, which sets all the flags and pointers to their initial values, effectively emptying the queue.
