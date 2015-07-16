#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include "../include/free_list.h"
#include "../include/malloc_list.h"




struct zone_libre{
        int adresse;
        int taille;
        struct zone_libre *p_suivant;

       }; 
       int allouer(char name[20],int taille){
           struct zone_alloc *zone;           
           zone=pop_freelist(name,taille);          
           if(zone==NULL){
             printf("allouer: memoire inssifusante\n");
             return-1;             
           }   
           
           push_alloclist(zone);
           
           return zone->adresse;           
       }


       int libirer(char name[]){
           struct zone_libre *zone;
           zone=pop_alloclist(name);
           if(zone==NULL)
               return -1;
           push_freelist(zone->adresse,zone->taille);
           return 0;           
       }
