#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<mqueue.h>
#include<sys/mman.h>
#include "../include/free_list.h"
/*
 * 
 */

struct zone_libre{
        int adresse;
        int taille;
        struct zone_libre *p_suivant;

       }; 
       
int initialiser(int adr,int size){
    push_freelist(adr,size);
return 0;    
}

int creer_segment(char name[],int taille){
     int *add;
    int *adr;
    adr=(int *)0x1000000;  
    int fd1;   
    if((fd1 = open (name,O_RDWR | O_CREAT,0600)) == -1) {
        perror("open");
        exit(1);
    } 
//    if (ftruncate(fd1,taille)==-1){
//        perror("fruncate");
//        exit(1);
//    }
    if((add=mmap(adr,taille,PROT_READ|PROT_WRITE,MAP_FIXED | MAP_SHARED,fd1,0))==MAP_FAILED){
        perror("mmap");
        exit(1);
    }   
    return add;
}
