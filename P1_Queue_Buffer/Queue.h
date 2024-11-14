/* ---- Headerswitch on (for prevention of nested includes) ------------------*/

#ifndef QUEUE_H_
#define QUEUE_H_

/**
 * \file       Queue.h
 * \author     Jennifer Reynaga
 * \brief      Header file for the Queue.
 */

/*----------------------------------------------------------------------------*/
/*                                  Includes                                  */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
/*----------------------------------------------------------------------------*/
/*                             Defines and macros                             */
/*----------------------------------------------------------------------------*/
#define FALSE                0u
#define TRUE                 1u
#define NEW                  0u

/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/

/**
 * @brief Initialization function for the queue
 */
void AppQueue_initQueue( AppQue_Queue *queue );

/**
 * @brief Function that allows writing an element to the queue.
 */
uint8_t AppQueue_writeData( AppQue_Queue *queue, void *data );

/**
 * @brief Function that allows reading an element from the queue.
 */
uint8_t AppQueue_readData( AppQue_Queue *queue, void *data );

/**
 * @brief Function that indicates if the queue is empty.
 */
uint8_t AppQueue_isQueueEmpty( AppQue_Queue *queue );

/**
 * @brief Function that empties the queue.
 */
void AppQueue_flushQueue( AppQue_Queue *queue );


#endif /* QUEUE_H_ */