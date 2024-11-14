/**
 * \file       Queue.c
 * \author     Jennifer Reynaga
 * \brief      Implementation for Queue
 */

/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

/*----------------------------------------------------------------------------*/
/*                               Local defines                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Declaration of local functions                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                     Implementation of global functions                     */
/*----------------------------------------------------------------------------*/

void AppQueue_initQueue( AppQue_Queue *queue )
{
    queue ->  Full = FALSE;     /* The buffer is not full */
    queue ->  Empty = TRUE;     /* The buffer doesnt have any elements */
    queue ->  Head = 0;         /* Head in the position 0 */
    queue ->  Tail = 0;         /* Tail in the position 0 */
    queue ->  Head_wrap = 0;    /* Flag when Head is wrap */
    queue ->  Tail_wrap = 0;    /* Flag when Tail is wrap */
}

uint8_t AppQueue_writeData( AppQue_Queue *queue, void *data )
{
    uint8_t write_status;
    /* If the queue is full we CAN NOT add more elements */
    if (((queue -> Tail) == (queue -> Head)) && (queue -> Head_wrap != queue -> Tail_wrap))
    {   
        queue -> Full = TRUE;            /* Set Full flag TRUE */
        write_status = FALSE;    /* The write WAS NOT successful */

    } 
    else 
    {   /* Queue is NOT FULL*/
        queue -> Full = FALSE;        /* Set Full flag FLAG FALSE*/
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

void AppQueue_flushQueue( AppQue_Queue *queue )
{
    queue ->  Full = FALSE;         /* The buffer is not full */
    queue ->  Empty = TRUE;         /* The buffer doesnt have any elements */
    queue ->  Head = 0;             /* Head in the position 0 */
    queue ->  Tail = 0;             /* Tail in the position 0 */
    queue ->  Head_wrap = 0;        /* Flag when Head is wrap */
    queue ->  Tail_wrap = 0;        /* Flag when Tail is wrap */

}