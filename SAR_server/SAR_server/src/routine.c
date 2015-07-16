#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include "../include/malloc_list.h"
#include "../include/free_list.h"
#include "../include/allouer.h"
#define FREE 0
#define T_READ 1
#define T_WRITE 2
#define T_MALLOC 3
struct rep {
    char ip[15];
    int port;
    int verrou;
    int taille;         /*pour creer l'esapce chez le client*/
};
struct demande{
    int type_acces;
    char name[20];
    char ip[15];
    int port;
    int taille;
};
//struct zone_alloc{
//           int port;   
//           char ip[20];          
//           int type_acces;
//           int adresse;
//           int taille;
//           int verrou;
//           int libre;
//           char name[20];
//           struct zone_alloc *next;
//       };  

void routine(int sok){     
    int addr;
    struct demande req;
    struct rep reponse;
    struct zone_alloc *p;        
      if (read(sok,&req, sizeof(req)) < 0){
	        perror("routine:read_sock");
	        exit(1);
           }
    if(req.type_acces==T_MALLOC){        
      if((existe(req.name))==0){     /* si le nom existe déja renvoie -1*/
             printf("routine:ce nom existe deja\n");              
             addr=-1;
         if(write(sok,&addr,sizeof(addr))<0){
              perror("routine:write-sock");
              exit(1);
         }                       
      }
      else{                      /* si le nom n'existe pas avant int name_exite(char name[20]) */         
             
             addr=allouer(req.name,req.taille);  /*renvoie la adresse*/            
             if(write(sok,&addr,sizeof(addr))<0){
                   perror("routine:write_sock");
                   exit(1);
             }        
            
      }
    }
   else{
    p =get_zone(req.name);
    if(p==NULL){
        printf("routine: erreur get zone\n");
        reponse.port=0;
            if(write(sok,&reponse,sizeof(reponse))<0){
                                perror("routine:write_sock");
                                exit(1);
                        }
        
    }
    else{
        if((p->libre)==0){              /* s'il y a pas de requete free*/
            if((req.type_acces)==T_READ){ 
                
                /*fournir le port et @ip de dernier verssion*/
                 strcpy(reponse.ip,p->ip);
                 reponse.port=p->port;
                 reponse.verrou=p->verrou;
                 reponse.taille=p->taille;
                 /*envoyer la reponse*/
                 if(write(sok,&reponse,sizeof(reponse))<0){
                        perror("routine:write_sock");
                        exit(1);
                 }
            }
            else{
                if(req.type_acces==T_WRITE){ 
                    
                        /*fournir le port et @ip de dernier verssion*/
                        strcpy(reponse.ip,p->ip);
                        if(p->verrou==0)
                            reponse.port=req.port;
                        else
                        reponse.port=p->port;
                        reponse.verrou=p->verrou ;       /* s'il verrou egal =0 peut s'effectue directement write*/
                        reponse.taille=p->taille;
                        /*update le port et @ip de dernier verssion*/
                        strcpy(p->ip,req.ip);
                        p->port=req.port;  
                        /*envoyer la reponse*/
                        if(write(sok,&reponse,sizeof(reponse))<0){
                                perror("routine:write_sock");
                                exit(1);
                        }
                        if(p->verrou==0)
                        p->verrou=1;
                    
                }                                 
            else{
                   if(req.type_acces==FREE){  /*si requete est libirer espace mettre a jour le drapeu libre*/
                                p->libre = 1;   
                                strcpy(reponse.ip,p->ip);
                                reponse.port=p->port; 
                                reponse.taille=p->taille;
                        }
                    }
                    
                }                    
          }        
        else{           /*si déja il y a demande libirer espace renvoie le port =0*/
            reponse.port=0;
            if(write(sok,&reponse,sizeof(reponse))<0){
                                perror("routine:write_sock");
                                exit(1);
                        }           
        }
    }       
    }  
   
  }
     
    
    
