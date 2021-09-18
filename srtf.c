/* Fill in your Name and GNumber in the following two comment fields
 * Name: Laraib Pervez
 * GNumber:01073542
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"
#include "srtf.h"

/* Feel free to create any helper functions you like! */
// This method will insert it into Queue state in Ascending PID. 
int insert(queue_t *queue, process_t *process, int mask) {
    //invalid input then return -1.
    if (queue != NULL) {
        //Initialize process flag, temporary, previous.
        int counter = 0; 
        process->flags = process->flags^mask;
        process_t *current = queue->head; 
        process_t *previous = NULL;   

       // Ascending order of pid
       while (current != NULL) {
          if(current->pid > process->pid) {
              
            if(previous == NULL) {
            process->next = current;
             queue->head = process;
             counter = 1;
             break; 

            } else {
             previous->next = process;
             process->next = current;
             counter = 1;
             break;
            }
          }
          previous = current;
          current = current->next;
       }

       if(counter == 0) {
           //if it is Endpoint and if the Queue is empty.
          if (previous != NULL) {
            previous->next = process;
          } else { 
            queue->head = process;
          }
       }
       //increment count
       queue->count += 1;
       return 0;
    } else {
        return -1;
    }
}
/* Initialize the header_t Struct
 * Follow the specification for this function.
 * Returns a pointer to the new header_t or NULL on any error.
 */
header_t *srtf_init() {
    //Initialize the header_t
    header_t *Header_t = (header_t *) malloc(sizeof(header_t));
    queue_t *ready_queue = (queue_t *) malloc(sizeof(queue_t));
    queue_t *stopped_queue = (queue_t *) malloc(sizeof(queue_t));
    queue_t *defunct_queue = (queue_t *) malloc(sizeof(queue_t));

    if(Header_t == NULL || ready_queue == NULL || stopped_queue == NULL ||
    defunct_queue == NULL) {
        return NULL;
    }
 
    // initialize the variables for ready queue
    ready_queue->head = NULL;
    ready_queue->count = 0;

    stopped_queue->head = NULL;
    stopped_queue->count = 0;
    
    defunct_queue->head = NULL;
    defunct_queue->count = 0;

    //Initializing the queue states 
    Header_t->ready_queue = ready_queue;
    Header_t->stopped_queue = stopped_queue;
    Header_t->defunct_queue = defunct_queue;

     return Header_t; // Replace Me with Your Code!
}
/* Add the process into the appropriate linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_add(header_t *header, process_t *process) {
    //invalid input
    if (header == NULL || process == NULL) {
      return -1;
    }
    // If the state is STATE_CREATED
    if ((process->flags & STATE_CREATED) == STATE_CREATED) {
        //binary operation 
        process->flags = (process->flags & ~(1 << 0)) | ((0 << 0) & (1 << 0));
        process->flags = process->flags | STATE_READY;
        
        queue_t * ready_queue = header->ready_queue;
        //Ascending PID order.
        return insert(ready_queue, process, 2);
    
    } else if ((process->flags & STATE_READY) == STATE_READY) {
        // if time_remaining is greater than zero then insert into ready_queue.
        if (process->time_remaining > 0) {
        queue_t * ready_queue = header->ready_queue;
        return insert(ready_queue, process, 2);
     }
      
    } else if (process->time_remaining == 0){
      process->flags = (process->flags & ~(1 << 1)) | ((0 << 1) & (1 << 1));
      process->flags = process->flags | STATE_DEFUNCT;
      
      queue_t * defunct_queue = header->defunct_queue;
      return insert(defunct_queue, process, 10);
      
    } else if ((process->flags & STATE_DEFUNCT) == 0) {
        
      queue_t * defunct_queue = header->defunct_queue;
      return insert(defunct_queue, process, 10);
    }
    return -1; // Replace Me with Your Code!
}

/* Move the process with matching pid from Ready to Stopped.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_stop(header_t *header, int pid) {
    // correct input then proceed.
    if (header != NULL) {
    //Initializing
    queue_t *ready_queue = header->ready_queue;
    process_t *current = ready_queue->head;
    process_t *previous = NULL;
    process_t *process = NULL;
    
    if (process != NULL) {
        
    for (int i = 0; i < 5;i++) {
        
       process->flags  = (process->flags & ~(1 << i)) | ((0 << i) & (1 << i));
   }
      process->flags = process->flags | STATE_STOPPED;
      queue_t *stopped_queue = header->stopped_queue;
      
      return insert(stopped_queue, process, 6);
    } 
    
    while (current != NULL) {
        
      if (current->pid == pid) {
        process = current;
        
        if (previous == NULL) {
          ready_queue->head = current->next;
        } else {
          previous->next = current->next;
        }
        process->next = NULL;
        ready_queue->count -= 1;
        break;
      }
      //update current next
      previous = current;
      current = current->next;
    }
  }
       return -1; // for invalid input
}

/* Move the process with matching pid from Stopped to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int srtf_continue(header_t *header, int pid) {
    if (header != NULL) {
    queue_t *stopped_queue = header->stopped_queue;
    process_t *current = stopped_queue->head;
    process_t *previous = NULL;
    process_t *process = NULL;
    
    if (process != NULL) {
      for (int i=0; i< 5; i++) {
        process->flags  = (process->flags & ~(1 << i)) | ((0 << i) & (1 << i));
      }
      process->flags = process->flags | STATE_READY;
      queue_t *ready_queue = header->ready_queue;
      
      return insert(ready_queue, process, 6);
    }
    while (current != NULL) {
      if (current->pid == pid) {
        process = current;
        if (previous == NULL) {
          stopped_queue->head = current->next;
        } else {
          previous->next = current->next;
        }
        process->next = NULL;
        stopped_queue->count -= 1;
        break;
      }
      previous = current;
      current = current->next;
    }
  }

  return -1; // Replace Me with Your Code!
}

/* Remove the process with matching pid from Defunct.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int srtf_reap(header_t *header, int pid) {
    
    if (header != NULL) {
        
    queue_t *defunct_queue = header->defunct_queue;
    process_t *current = defunct_queue->head;
    process_t *previous = NULL;
    process_t *process = NULL;
    
    while (current != NULL) {
      if (current->pid == pid) {
        process = current;
        if (previous == NULL) {
          defunct_queue->head = current->next;
        } else {
          previous->next = current->next;
        }
        process->next = NULL;
        defunct_queue->count -= 1;
        break;
      }
      previous = current;
      current= current->next;
    }
    if (process != NULL) {
      for (int i=0; i < 5; i++) {
        process->flags  = (process->flags & ~(1 << i)) | ((0 << i) & (1 << i));
      }
      process->flags = process->flags | STATE_TERMINATED;
      //extracting exit_code from the flags.
      unsigned int exit_code = process->flags;
      exit_code = exit_code >> 6;

      process_t *head = process;
      process_t *current = NULL;
      
      while (head != NULL) {
        current = head;
        head = head->next;
	if (current->command != NULL) {
        	free(current->command);
		current->command = NULL;
	}
     if (current != NULL) {	
        	free(current);
		current = NULL;
	}
          
      }
      if (head != NULL) {
      	free(head);
        head = NULL;
      }
      return exit_code;
    }
  }

  return -1; // Replace Me with Your Code!
}

/* Create a new process_t with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the process_t on success or a NULL on any error.
 */
process_t *srtf_generate(char *command, int pid, int time_remaining, int is_sudo) {
    //creating new process 
   process_t *process = (process_t *) malloc(sizeof(process_t));
   
   if (process == NULL) {
    return NULL;
  }
  // create new process and copy 
  process->command = malloc (40* sizeof (char));
  strcpy(process->command, command);
  
//Set the fields for pid and time_remaining. 
//For time_last_run, call the following clock function to get the current time: clock_get_time(); 
//and use that value. All newly created processes will have their
// creation time as their initial time last run. 
// Set the initial state to STATE_CREATED.
  process->pid = pid;
  process->time_remaining = time_remaining;
  process->time_last_run = clock_get_time();
  process->next = NULL;
  
  process->flags = 0x0 | STATE_CREATED;
  // if sudo is non zero then bring PF_SUPERPRIV
  if (is_sudo == 1) {
    process->flags = PF_SUPERPRIV | STATE_CREATED;
  }

  return process; // Replace Me with Your Code!
}
 
/* Schedule the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
process_t *srtf_schedule(header_t *header) {
    // if valid input then proceed 
    if (header != NULL) {
        
    if (header->ready_queue != NULL) {
      int lowestTimeRemaining = 0;
      int lowestPid = 0;
      queue_t *ready_queue = header->ready_queue;
      process_t *process = NULL;
      process_t *current = ready_queue->head;
      // if the current is valid input, set LTR and pid as current
      if (current != NULL) {
        lowestTimeRemaining = current->time_remaining;
        lowestPid = current->pid;
      }
      
      while(current != NULL) {
        if(current->time_remaining <=lowestTimeRemaining) {
          lowestTimeRemaining = current->time_remaining;
          if (current->pid <= lowestPid) {
            lowestPid = current->pid;
            process = current;
          }
        }
        //updating current next
        current = current->next;
      }

      current = ready_queue->head;
      // starvation 
      while(current != NULL) {
        if ((clock_get_time() - current->time_last_run) >= TIME_STARVATION) {
          lowestPid = current->pid;
          process = current;
          break;
        }
        current = current->next;
      }
    // if process is valid then proceed
      if (process != NULL) {
          current = ready_queue->head;
          process_t *previous = NULL;
          process_t *stateSelected = NULL;
          
          while (current != NULL) {
              
            if (current->pid == lowestPid) {
              stateSelected = current;
              
              if (previous == NULL) {
                ready_queue->head = current->next;
              } else {
                  
                previous->next = current->next;
              }
              //decrement
              stateSelected->next = NULL;
              ready_queue->count -= 1;
              break;
            }
            previous = current;
            current = current->next;
         }
         return stateSelected;
      }
    }
  }
  return NULL; // Replace Me with Your Code!
}

/* Returns the number of items in a given queue_t
 * Follow the specification for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int srtf_count(queue_t *ll) {
    if (ll == NULL) {
        return -1;
    }
  return ll->count; // Replace Me with Your Code!
}

/* Frees all allocated memory in the header_tr */
void srtf_free(header_t *header) {
    // if valid input 
    if (header != NULL) {
        // checking if ready_queue is null 
    if (header->ready_queue != NULL) {
    // Memory check for ready_queue state
      if(header->ready_queue->head != NULL) {
        process_t *head = header->ready_queue->head;
        process_t *current;
        while (head != NULL) {
          current = head;
          head = head->next;
          if (current->command != NULL) {
          	free(current->command);
		current->command = NULL;
	  }
          if (current != NULL) {
          	free(current);
		current = NULL;
	  }
        }
      }
      if (header->ready_queue != NULL) {	
      	free(header->ready_queue);
	header->ready_queue = NULL;
      }
      
    }

    if (header->stopped_queue != NULL) {
    // Memoery check for stopped_queue state
      if(header->stopped_queue->head != NULL) {
        process_t *head = header->stopped_queue->head;
        process_t *current;
        while (head != NULL) {
          current = head;
          current = head->next;
	  if (current->command != NULL) {	
          	free(current->command);
		current->command = NULL;
	  }
          if (current != NULL) {
          	free(current);
		     current = NULL;
	  }
        }
      }
      
      if (header->stopped_queue != NULL) {
      	free(header->stopped_queue);
	header->stopped_queue = NULL;
      }
    }

    if (header->defunct_queue != NULL) {
    //Memory check for defunct_queue state.
      if(header->defunct_queue->head != NULL) {
        process_t *head = header->defunct_queue->head;
        process_t *current;
        while (head != NULL) {
          current = head;
          head = head->next;
          if (current->command != NULL) {
	  	free(current->command);
		current->command = NULL;
	  }
          if (current != NULL) {
          	free(current);
		     current = NULL;
	  }
        }
      }

      if (header->defunct_queue != NULL) {
      		free(header->defunct_queue);
		header->defunct_queue = NULL;
      }
      
    }
    // free memory at the Endpoint
    free(header);
    header = NULL;
  } 
}