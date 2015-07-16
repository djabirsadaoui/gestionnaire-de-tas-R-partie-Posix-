#define _XOPEN_SOURCE 700
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include "../include/req_connexion.h"
#include "../include/create_sockcontrol.h"
#include "../include/allouer.h"
#include "../include/malloc_list.h"
#include "../include/free_list.h"
#define FREE 0
#define T_READ 1
#define T_WRITE 2
#define T_MALLOC 3
#define FREE_READ 4
#define PORT 1502

struct reponse_client{
       int val;
        int port;
        char ip[15];
        int taille;
    };
struct reponse_server {
    char ip[15];
    int port;
    int verrou;
    int taille;                 /*pour creer espace c'est localement*/
};



struct reponse_server rep_server;
struct reponse_client rep_client;
struct client_info_server req1;
struct client_info_client req2;


int t_read(char name[20]){
     struct zone_alloc *f;
      
      /*remplir*//*req1 pour server,req2 pour client*/
      strcpy(req1.name,name);
      req1.type_acces=T_READ;
      req1.port=PORT;
      strcpy(req1.ip,get_addr());                /*recupérer la adresse de la machine*/      
      rep_server = req_rerver(req1,8081);        /*envoie la requête de lecture au serveur*/  
      printf("t_fonction:envoie requete de lecture au server\n");
      if(rep_server.port==0){                    /*si espace va selibere ou il y a pas ecriture sur cette var*/
                return -1;                        
      }
      else{  
             /*remplir la requete du client*/
              strcpy(req2.name,name);
              req2.type_acces=T_READ;
              rep_client = req_client(req2 ,rep_server.port);      /*envoie la requete au site qui a la dernier ver*/
              printf("t_fonction:envoie requete de lecture au dernier demandeur\n");
              f=get_zone(name);
              if(f==NULL){                                    /*si cet espace n'est pas allouer avant */
                 if(allouer(name,rep_client.taille)==-1)     /*allouer espace*/
                     printf("t_fonction:erreur d'allocation local\n");
                 f=get_zone(name);
                 strcpy(f->ip,rep_client.ip);                 /*enregister @ du site pour renvoie de release */
                 f->port=rep_client.port;
              }
              else{
                 strcpy(f->ip,rep_client.ip);
                 f->port=rep_client.port;  
              }              
      }
      return rep_client.val ;
}




int t_malloc(int taille,char name[20]){
    /*remplir*//*req1 pour server,req2 pour client*/
      strcpy(req1.name,name);
      req1.type_acces=T_MALLOC;
      req1.port=PORT;
      strcpy(req1.ip,get_addr());
      req1.taille=taille;
      int s=req_malloc(req1,8081);
      printf("t_fonction:envoie requete malloc au server\n");
      if(s==-1){
          return s;                     /*en cas d'erreur renvoie -1*/
      }
      else{
          if (allouer(req1.name,taille)==-1)
              printf("t_fonction:erreur d'allocation local\n");
          return s;              
      }
     
}

int t_write(char name[20]){             /*elle renvoie -1 s'il y a erreur*/
    
    struct zone_alloc *f;
    /*remplir requete*//*req1 pour server,req2 pour client*/
      strcpy(req1.name,name);
      req1.type_acces=T_WRITE;
      req1.port=PORT;
      strcpy(req1.ip,get_addr());               /*recupérer la adresse de la machine*/
      
      rep_server = req_rerver(req1,8081);        /*envoie la requte de ecriture au serveur*/ 
      printf("t_fonction:envoie requete ecriture au serveur\n");
      
      if(rep_server.port==0){                   /*si espace va selibere ou il y a pas ecriture sur cette var*/
                return -1;
      }
      else{
          f=get_zone(req1.name);
             if(f==NULL){               /*s'il espace n'est pas existé avant */
                 if(allouer(req1.name,rep_server.taille)==-1)
                 printf("t_fonction:erreur allocation local\n");
                 f=get_zone(req1.name);         /*changer la structure de alloc au niveau client*/
                 f->request=1;
                 f->compteur=0;
             }
             else{                              /*s'il existe déjà avant*/
                 f->request=1;
                 f->compteur=0;
            }
          
          if(rep_server.verrou){             /*si je suis pas le premier demandeur*/                                            
              strcpy(req2.name,name);
              req2.type_acces=T_WRITE;
              rep_client = req_client(req2 ,rep_server.port);           /*envoie la requete au site qui a la dernier ver*/
//              printf("t_fonction:envoie requete au dernier demandeur\n");
//              f=get_zone(req1.name);
//             if(f==NULL){                       /*s'il espace n'est pas existé avant */
//                 printf("la taille de valeur=%d\n",req1.taille);
//                 if(allouer(req1.name,rep_client.taille)==-1)
//                 printf("t_fonction:erreur allocation local\n");
//                 f=get_zone(req1.name);         /*changer la structure de alloc au niveau client*/
//                 f->request=1;                  /* mettre le verrou de ecriture*/ 
//                 f->compteur=0;
//             }
//             else                               /*s'il existe déjà avant*/
//                 f->request=1;
//                 f->compteur=0;
              
          }
   }
      return (f->adresse);
}

int t_release(char name[20]){
    struct zone_alloc *f;
    f=get_zone(name);
    /*remplir la requete du client*/
    strcpy(req2.name,name);
    req2.type_acces=FREE_READ;
    rep_client = req_client(req2,f->port);       /*envoie la requete au site qui a la dernier ver*/
    return 0;
}
    
    
    

