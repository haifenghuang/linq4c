/*dataStructure.c
 *
 * implementation of the data structure
 */

#include <string.h>
#include "dataStructure.h"

static pthread_mutex_t _ds_mtx = PTHREAD_MUTEX_INITIALIZER;

DataStructure* DataStructure_init()
{
    DataStructure* ds = (DataStructure*) malloc(sizeof(DataStructure));
    ds->head = NULL;
    ds->tail = NULL;
    ds->linkSize = 0;
    return ds;
}

void DataStructure_destroy(DataStructure* ds)
{
    if(ds == NULL)
        return;
    Node* current = ds->head;
    Node* temp;
    while(current != NULL)
    {
        temp = current->next;
        current->next = NULL;
        free(current->address);
        free(current);
        current = temp;
    }
    ds->head = NULL;
    ds->tail = NULL;
    ds->linkSize = 0;
    free(ds);
    ds = NULL;
}

void Node_insert(DataStructure* ds, void* address, size_t sizeAllocated){
    if(ds == NULL)
    {
        fprintf(stderr, "DataStructure is not initialized\n");
        return;
    }

    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->mark = 0;
    newNode->size = sizeAllocated;
    newNode->address = address;

    /* When there is no node in the DataStructure, 
        insert the node and set head and tail. */
    if(ds->linkSize == 0)
    {
        ds->head = newNode;
        ds->tail = newNode;
        newNode->next = NULL;
        ds->linkSize++;
        return;
    }

    /* When there are nodes in the DataStructure,
        insert the node based on the size of address. */
    /* Insert from head*/
    if(newNode->address < ds->head->address)
    {
        newNode->next = ds->head;
        ds->head = newNode;
        ds->linkSize++;
    }


    /* Insert from tail */
    else if(newNode->address > ds->tail->address)
    {
        newNode->next = NULL;
        ds->tail->next = newNode;
        ds->tail = newNode;
        ds->linkSize++;
    }
    /* Insert between head and tail */
    else
    {
        Node* current = ds->head;
        Node* previous = NULL;
        while(current != NULL && (current->address < newNode->address))
        {
            previous = current;
            current = current->next;
        }

        //current must not equal to NULL
        if(current == NULL)
        {
             fprintf(stderr, "Something Went Wrong. Line: %d\n", __LINE__);
             exit(EXIT_FAILURE);
        }   

        previous->next = newNode;
        newNode->next = current;
        ds->linkSize++;
    }
    return;
}

void Node_insert_r(DataStructure* ds, void* address, size_t sizeAllocated){
    if(ds == NULL)
    {
        fprintf(stderr, "DataStructure is not initialized\n");
        return;
    }

    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->mark = 0;
    newNode->size = sizeAllocated;
    newNode->address = address;

    /* When there is no node in the DataStructure, 
        insert the node and set head and tail. */
    pthread_mutex_lock(&_ds_mtx);
    if(ds->linkSize == 0)
    {
        ds->head = newNode;
        ds->tail = newNode;
        newNode->next = NULL;
        ds->linkSize++;
        pthread_mutex_unlock(&_ds_mtx);
        return;
    }

    /* When there are nodes in the DataStructure,
        insert the node based on the size of address. */
    /* Insert from head*/
    if(newNode->address < ds->head->address)
    {
        newNode->next = ds->head;
        ds->head = newNode;
        ds->linkSize++;
    }


    /* Insert from tail */
    else if(newNode->address > ds->tail->address)
    {
        newNode->next = NULL;
        ds->tail->next = newNode;
        ds->tail = newNode;
        ds->linkSize++;
    }
    /* Insert between head and tail */
    else
    {
        Node* current = ds->head;
        Node* previous = NULL;
        while(current != NULL && (current->address < newNode->address))
        {
            previous = current;
            current = current->next;
        }

        //current must not equal to NULL
        if(current == NULL)
        {
             fprintf(stderr, "Something Went Wrong. Line: %d\n", __LINE__);
             exit(EXIT_FAILURE);
        }   

        previous->next = newNode;
        newNode->next = current;
        ds->linkSize++;
    }
    pthread_mutex_unlock(&_ds_mtx);
    return;
}

void Node_remove(DataStructure* ds, void* address)
{
    if(ds == NULL || ds->linkSize == 0)
    {
        fprintf(stderr, "The dataStructure is empty\n");
        return;
    }
    //Only one node in the structure
    if(ds->linkSize == 1)
    {
        free(ds->head->address);
        free(ds->head);
        ds->head = NULL;
        ds->tail = NULL;
        ds->linkSize--;
        return;
    }

    //More than one node in the structure
    //Remove the head
	else if(ds->head->address == address)
    {
        Node* temp = ds->head->next;
		free(ds->head->address);
        free(ds->head);
        ds->head = temp;
        ds->linkSize--;
    }
    //Remove the tail
    else if(ds->tail->address == address)
    {
        Node* current = ds->head;
        while(current->next != ds->tail )
            current = current->next;
        current->next = NULL;
		free(ds->tail->address);
        free(ds->tail);
        ds->tail = current;
        ds->linkSize--;
    }
    //Remove from middle
    else
    {
        Node* current = ds->head;
        Node* previous;
        while(current != NULL && current->address != address)
        {
            previous = current;
            current = current->next;
        }   
        if(current == NULL)
        {
            fprintf(stderr, "No Node Matched To Be Removed. Line: %d\n", __LINE__);
            return;
        }
        previous->next = current->next;
		free(current -> address);
        free(current);
        current = NULL;
        ds->linkSize--;
    }
    return;
}

void Node_remove_r(DataStructure* ds, void* address)
{
    if(ds == NULL || ds->linkSize == 0)
    {
        fprintf(stderr, "The dataStructure is empty\n");
        return;
    }

    pthread_mutex_lock(&_ds_mtx);
    //Only one node in the structure
    if(ds->linkSize == 1)
    {
        free(ds->head->address);
        free(ds->head);
        ds->head = NULL;
        ds->tail = NULL;
        ds->linkSize--;
        pthread_mutex_unlock(&_ds_mtx);
        return;
    }

    //More than one node in the structure
    //Remove the head
    if(ds->head->address == address)
    {
        Node* temp = ds->head->next;
		free(ds->head->address);
        free(ds->head);
        ds->head = temp;
        ds->linkSize--;
    }
    //Remove the tail
    else if(ds->tail->address == address)
    {
        Node* current = ds->head;
        while(current->next != ds->tail )
            current = current->next;
        current->next = NULL;
		free(ds->tail->address);
        free(ds->tail);
        ds->tail = current;
        ds->linkSize--;
    }
    //Remove from middle
    else
    {
        Node* current = ds->head;
        Node* previous;
        while(current != NULL && current->address != address)
        {
            previous = current;
            current = current->next;
        }   
        if(current == NULL)
        {
            fprintf(stderr, "No Node Matched To Be Removed. Line: %d\n", __LINE__);
            return;
        }
        previous->next = current->next;
		free(current -> address);
        free(current);
        current = NULL;
        ds->linkSize--;
    }
    pthread_mutex_unlock(&_ds_mtx);
    return;
}

void* DataStructure_findNode(DataStructure* ds, void* address)
{
    if(ds == NULL || ds->linkSize == 0)
    {
        fprintf(stderr, "The dataStructure is empty\n");
        return NULL;
    }
    Node* current = ds->head;
    while(current != NULL && current->address != address)
    {
        current = current->next;
    }
    if(current == NULL)
    {
        fprintf(stderr, "No Match Found. Line: %d.\n", __LINE__);
        return NULL;
    }
    return (void*)current->address;
}

void DataStructure_display(DataStructure* ds)
{
    if(ds == NULL || ds->linkSize == 0)
    {
        fprintf(stderr, "The dataStructure is empty\n");
        return;
    }
    Node* current = ds->head;
    int count = 0;
    fprintf(stderr, "Print Each Node:");
    while(current != NULL)
    {
        if(count % 5 == 0)
        fprintf(stderr, "\n");
        fprintf(stderr, "%p ", current->address);
        current = current->next;
        count++;
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "End of Print. \n");
}

void DataStructure_plot(DataStructure* ds)
{
    if(ds == NULL || ds->linkSize == 0)
    {
        fprintf(stderr, "The dataStructure is empty.\n");
        return;
    }
    char** graph = (char**) malloc(sizeof(char*)*5);
    size_t line_index;
    size_t line_length = (ds->linkSize)*16 + 1;
    //initiate all lines, with initiate length of 100
    for(line_index = 0; line_index < 5; line_index++)
    {
        graph[line_index] = (char*) malloc(sizeof(char)*line_length);
        graph[line_index][0] = '\0';
    }
    Node* current = ds->head;
    
    while(current != NULL)
    {
        for(line_index = 0; line_index < 5; line_index++)
        {
            if(line_index == 0)
            {
                strcat(graph[line_index], "**********      ");
            }
            else if(line_index == 1)
            {
                char strip[17];
                sprintf(strip, "* mark ");
                char marked[2];
                marked[0] = current-> mark + '0';
                marked[1] = '\0';
                strcat(strip, marked);
                strcat(strip, " *   *  ");
                strip[16] = '\0';
                strcat(graph[line_index], strip);
            }
            else if(line_index == 2)
            {
                strcat(graph[line_index], "*  size  * **** ");
            }
            else if(line_index == 3)
            {
                char strip[17];
                sprintf(strip, "*%lu", current->size);
                strip[strlen(strip)] = '\0';
                while(strlen(strip) < 9)
                {
                    strcat(strip, " ");
                }
                strcat(strip, "*   *  ");
                strip[16] = '\0';
                strcat(graph[line_index], strip);
            }
            else//line_index = 4
            {
                strcat(graph[line_index], "**********      ");
            }
        }
        current = current->next;
    }

    for(line_index = 0; line_index < 5; line_index++)
    {
        fprintf(stderr, "%s\n", graph[line_index]);
    }
    for(line_index = 0; line_index < 5; line_index++)
    {
        free(graph[line_index]);
    }
    free(graph);
}
