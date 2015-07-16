#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../include/malloc_list.h"
#include "../include/free_list.h"
#include "../include/allouer.h"
#include "../include/afficher_erreur.h"
 pthread_mutex_t mutex1;          /* protege les variables partagees */
 pthread_cond_t  requesting;     /* signal que ecriture local est terminée */
 pthread_cond_t  fin_lecture;        /* */
#define FREE 0
#define T_READ 1
#define T_WRITE 2
#define FREE_READ 4
#define PORT 1502
struct rep {
        int val;
        int port;
        char ip[15];
        int taille;
};
//struct zone_alloc{
//           int port;
//           char ip[15];
//           int val;
//           int adresse;
//           int taille;
//           int compteur;
//           int request;          
//           char name[20];
//           struct zone_alloc *next;
//       };  
       struct demande{
    int type_acces;
    char name[20];
    };
void job(int sock){
     int errno;
     struct demande req;
     struct rep reponse;
     struct zone_alloc *p;  
         
     
      if (read(sock,&req, sizeof(req)) < 0){            /*lire la re requete*/
	        perror("read thread");
	        exit(1);
           } 
    p =get_zone(req.name);                              /*recuperer la variable de la liste chainée*/
    if(p==NULL){
        printf("erreur get zone\n");
        reponse.port=0;
        if(write(sock,&reponse,sizeof(reponse))<0){
                                perror("write");
                                exit(1);
        }        
    }
    else{
        /*remplir la reponse du client*/
        reponse.taille=p->taille;        
        strcpy(reponse.ip,get_addr());
        reponse.port=PORT;
        reponse.val=p->val;
        if(req.type_acces==T_READ){                                /*si la requete est lecture*/
            errno = pthread_mutex_lock(&mutex1);
            if (errno) afficher_erreur("echec de pthread_mutex1_lock1");
            while(p->request){                            /*tantque en etat de requste attend*/
                errno = pthread_cond_wait(&requesting,&mutex1);         
                 if (errno) afficher_erreur("echec de pthread_cond_wait1");
            }
                 if(write(sock,&reponse,sizeof(reponse))<0){     /*lorsque il sors de état requesting ->transaction*/
                                perror("write");
                                exit(1);
                        }
             p->compteur++;                                  /*incrementer le compteur de la lecture*/
             printf("compteur=%d\n",p->compteur);
             errno = pthread_mutex_unlock(&mutex1);
             if (errno) afficher_erreur("echec de pthread_mutex1unlock1");
            
       }
        else{
            if(req.type_acces==T_WRITE){
            errno =pthread_mutex_lock(&mutex1);
               if (errno) afficher_erreur("echec de pthread_mutex1_lock2");
            
                while(p->compteur){                           /*tantque la lecture n'est encore pas terminée*/                   
                   errno = pthread_cond_wait(&fin_lecture,&mutex1);
                   if (errno) afficher_erreur("echec de pthread_cond_wait2");
                }
                while(p->request){
                    errno = pthread_cond_wait(&requesting,&mutex1);
                   if (errno) afficher_erreur("echec de pthread_cond_wait2");
                }
               // printf("taille=%d\n",reponse.taille);         
                if(write(sock,&reponse,sizeof(reponse))<0){    
                                perror("write");
                                exit(1);
                }
                  
                printf("demande_client:envoie reponse\n");
                errno = pthread_mutex_unlock(&mutex1);
                          if (errno) afficher_erreur("echec de pthread_mutex1unlock2");
                    
                
            }
            else{
                if(req.type_acces==FREE_READ){          /*libiration le verrou de lecture*/
                  errno = pthread_mutex_lock(&mutex1);
                        if (errno) afficher_erreur("echec de pthread_mutex1_lock5");  
                  p->compteur--;
                  printf("compteur=%d\n",p->compteur);
                  if(p->compteur==0)
                      pthread_cond_signal(&fin_lecture);
                }
                
                else{                            /*requete pour libirer esapce free()*/
                errno = pthread_mutex_lock(&mutex1);
                        if (errno) afficher_erreur("echec de pthread_mutex1_lock3");
                while(p->compteur){             /*tant que il y a des lecture distant*/
                    errno = pthread_cond_wait(&fin_lecture,&mutex1);
                         if (errno) afficher_erreur("echec de pthread_cond_wait3");
                }
                    while(p->request){           /*tant que il y a ecriture local*/
                       errno = pthread_cond_wait(&requesting,&mutex1);
                         if (errno) afficher_erreur("echec de pthread_cond_wait4");
                    }
                         if(libirer(p->name))
                          printf("demande_client: erreur free\n");
                    
                errno = pthread_mutex_unlock(&mutex1);
                if (errno) afficher_erreur("echec de pthread_mutex1unlock3");
                }
            }
          }
        }
      
}

//void Write_fin (char name[20]){
//    int errno;
//       struct zone_alloc *p;
//        p =get_zone(name);                              /*recuperer la variable de la liste chainée*/
//    if(p==NULL)
//        printf("demande_client:erreur get zone\n");   
//    errno = pthread_mutex_lock(&mutex1);
//        if (errno) afficher_erreur("demande_client:echec de pthread_mutex1_lock4");
//    p->request=0;
//    pthread_cond_signal(&requesting);
//   errno= pthread_mutex_unlock(&mutex1);
//    if (errno) afficher_erreur("demande_client:echec de pthread_mutex1_unlock4");
//}
//  
void Write_fin (int addr){
    int errno;
       struct zone_alloc *p;
         p =get_zone_adr(addr);
         errno = pthread_mutex_lock(&mutex1);
         if (errno) afficher_erreur("echec de pthread_mutex1_lock4");
         p->request=0;
         pthread_cond_signal(&requesting);
         errno= pthread_mutex_unlock(&mutex1);
         if (errno) afficher_erreur("echec de pthread_mutex1_unlock4");
}
                   
                                           /*recuperer la variable de la liste chainée*/
   
