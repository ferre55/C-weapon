/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include "Queue.h"

/* Declare the message to be handle by the queue */
typedef struct
{
    uint8_t msg;        /**< Message identifier */
    uint8_t value;      /**< First value */
    uint8_t value2;     /**< Second value */
} MsgType_Message;


int main( void )
{
    uint32_t empty_status; 

    MsgType_Message MsgToWrite;
    MsgType_Message MsgToRead;
    MsgType_Message buffer[ 3u ]; /* Array of 3 MsgType_Message structures for the queue’s memory space */

    AppQue_Queue Queue;/* Queue control structure */

    printf( "sizeof( Queue ): %d\n", sizeof( Queue ));

    Queue.Buffer = (void*)buffer; /* Sets the queue’s buffer to the address of the buffer array */
    Queue.Elements = 3u;   /* /**< Queue length, must be the same or less than array buffer size*/
    Queue.Size = sizeof(MsgType_Message); /* Sets the size of each element in the queue */
    printf( "sizeof( Queue ): %d\n", sizeof( Queue ));
    
    /* Initializes the queue with the provided buffer, element count, and element size*/
    AppQueue_initQueue( &Queue );   

    /** Write messages to the queue */
    MsgToWrite.msg = NEW;
    MsgToWrite.value = 100u; /* Sets the value field of MsgToWrite to 100 */ 
    MsgToWrite.value2 = 8u; /* Sets the value2 field of MsgToWrite to 8 */ 
    AppQueue_writeData(&Queue, &MsgToWrite); /* Writes the MsgToWrite structure to the queue */ 
    
    MsgToWrite.msg = 2; 
    MsgToWrite.value = 55u; /* Sets the value field of MsgToWrite to 55 */ 
    MsgToWrite.value2 = 18u; /* Sets the value2 field of MsgToWrite to 18 */ 
    AppQueue_writeData(&Queue, &MsgToWrite); /* Writes the MsgToWrite structure to the queue */ 
    
    MsgToWrite.msg = 8; 
    MsgToWrite.value = 88u; /* Sets the value field of MsgToWrite to 88 */ 
    MsgToWrite.value2 = 28u; /* Sets the value2 field of MsgToWrite to 28 */ 
    AppQueue_writeData(&Queue, &MsgToWrite);
    
    /* Test AppQueue_isQueueEmpty */
    empty_status = AppQueue_isQueueEmpty( &Queue);
    printf( "empty_status = %d\n", empty_status );

    /* Read all the messages */ 
    while( AppQueue_isQueueEmpty( &Queue ) == 0u )
    {
        AppQueue_readData( &Queue, &MsgToRead );
        printf( "msg read from the queue %d\n", MsgToRead.msg ); /* This prints the msg field of the read message.*/
        printf( "value read from the queue %d\n", MsgToRead.value ); /* This prints the value field of the read message. */
        printf( "value2 read from the queue %d\n", MsgToRead.value2 ); /* This prints the value field of the read message. */
    } 

    /* Check the new empty status */
    empty_status = AppQueue_isQueueEmpty( &Queue);
    printf( "empty_status = %d\n", empty_status );

    /* Add a new message*/
    MsgToWrite.msg = 15;
    MsgToWrite.value = 55u; 
    MsgToWrite.value2 = 50u; 
    printf("Writing msg: %d, value: %d\n", MsgToWrite.msg, MsgToWrite.value);
    AppQueue_writeData( &Queue, &MsgToWrite ); /* This writes the MsgToWrite structure to the queue.*/

    empty_status = AppQueue_isQueueEmpty( &Queue);
    printf( "empty_status = %d\n", empty_status );

    /* Test AppQueue_flushQueue */
    AppQueue_flushQueue( &Queue);

    empty_status = AppQueue_isQueueEmpty( &Queue);
    printf( "empty_status = %d\n", empty_status );
    
    return 0;
}