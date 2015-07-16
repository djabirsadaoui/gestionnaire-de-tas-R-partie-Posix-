#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
struct fool{
        int val;
        struct fool *next;

       };
struct fool *pop (struct fool *p_head){
            struct fool *a=NULL;
            if(p_head==NULL){                    /* si la file est vide */
                return a;
            }
            else 
                if(p_head->next==NULL){         /* s'il ya qu'une seule requete */
                    a=p_head;
                    p_head=NULL;
                    a->next=p_head;
                }
                else{
                    a=p_head;
                    p_head=p_head->next;
                    a->next=p_head;
                }
               return a;     
        }
