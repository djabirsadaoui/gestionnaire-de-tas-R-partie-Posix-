#define _XOPEN_SOURCE 700
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "../include/create_sockcontrol.h"
#include "../include/afficher_erreur.h"
#include "../include/pop.h"
#include "../include/push.h"
#include "../include/free_list.h"
#include "../include/initialiser.h"
#include "../include/allouer.h"
#include "../include/malloc_list.h"
#include "../include/routine.h"


#define N 4
#define PORT 8081

  int nbr_endormies;      /* nombre de threads endormies */
  int client;            /* connection recue en attente de traitement */
 pthread_mutex_t mutex;          /* protege les variables partagees */
 pthread_cond_t  nb_changed;     /* signal que 'nb' a ete modifie */
 pthread_cond_t  client_changed; /* signal que 'client' a ete modifie */
 struct fool{
        int val;
        struct fool *next;
       };

struct fool *first;     /*represente le premier element dans la file d'attente*/

void consomme_req(struct fool *p);      /*retirer une requete de la file d'attente*/
void* traiter_requete(void* arg);       /*la gestion des threads*/
void attendre_connexion();              /*attendere les requetes des clients */


int main()
{
  pthread_t id[N];
  int i;   
  nbr_endormies = N;            /*  initialise le pool de thread */  
  printf("nbr thread initial=%d\n",nbr_endormies);
  client = -1;
//  printf("valeur client=%p\n",&client);
  int adresse;
  int taille=1000;
  adresse=creer_segment("sada",taille);        /*creer un segment de memoire */
  initialiser(adresse,taille);               /*initialiser free_list*/
  errno = pthread_mutex_init(&mutex, NULL);
        if (errno) afficher_erreur("echec de pthread_mutex_init");
  errno = pthread_cond_init(&client_changed, NULL);
        if (errno) afficher_erreur("echec de pthread_cond_init");
  for (i=0; i<N; i++)  {
    errno = pthread_create(&id[i], NULL, traiter_requete, NULL);
         if (errno) afficher_erreur("echec de pthread_create");
  } 
  signal(SIGPIPE, SIG_IGN);      /* supprime SIGPIPE */  
  attendre_connexion();         /* lance le serveur */
        
  return 0;  
}

/*la routine excutépar le thread*/

void consomme_req(struct fool *p){
     
     routine(p->val);/*effectueer le traitement de la premiere requte dans la file  */
     first= p->next; /*changer le premier element dans la file d'attente*/    
 } 
  void* traiter_requete(void* arg){
  struct fool *p;   
  int soc; 
  while (1) {              
    errno = pthread_mutex_lock(&mutex);
                if (errno) afficher_erreur("echec de pthread_mutex_lock1");
    while(client==-1){          /* attend une nouvelle requete à traiter */ 
        errno = pthread_cond_wait(&client_changed, &mutex);  
                if (errno)afficher_erreur("echec de pthread_cond_wait");
    }
    soc=client;                 /*prendre la requete recu*/
    client=-1;
    nbr_endormies--;          /*décrimenter le nombre des thread sans travail*/
    routine(soc);
//    printf("nbr thread=%d\n",nbr_endormies);
    
    errno = pthread_mutex_unlock(&mutex);
            if (errno) afficher_erreur("echec de pthread_mutex_unlock2");
    
                /* executer la routine de la requete */

    pthread_mutex_lock(&mutex); 
   
    while(first!=NULL){                  /*s'il y a des requetes dans la fille d'attente*/
         p=pop(first);                   /*recupérer le premier element dans la file d'attente */
         consomme_req(p);                /*effectuer sa traitement */
         pthread_mutex_unlock(&mutex);
          printf("moi %d j'ai termine mon travail\n",(int)pthread_self());
    }
    pthread_mutex_unlock(&mutex);
    errno=pthread_mutex_lock(&mutex);
        if (errno) afficher_erreur("echec lock 3");
           
    nbr_endormies++;                     /*incrementer le nombre des thread sans travail*/
//        printf("nbr thread=%d\n",nbr_endormies);
    errno= pthread_mutex_unlock(&mutex);
        if (errno) afficher_erreur("echec unlock 3");
  }
}

  void attendre_connexion(){
   int sock_contr=cree_socket_ecoute(PORT);   
   printf("Serveur actif sur le port  %d socket %d\n",PORT,sock_contr);
  while (1) {
    int x;
    x = accept(sock_contr,NULL, 0);   
    errno = pthread_mutex_lock(&mutex);
        if (errno) afficher_erreur("echec de pthread_mutex_lock5");
    if(nbr_endormies){           /*s'il ya des threads libres*/
        client = x;  
//        printf("req_client=%d\n",client);
                if (client==-1) {
                        if (errno==EINTR || errno==ECONNABORTED) continue; /* non fatal */
                                 afficher_erreur("echec de accept");
                } 
         errno = pthread_cond_signal(&client_changed);
                if (errno) afficher_erreur("echec de pthread_cond_signal");
         pthread_mutex_unlock(&mutex);
    }    
    else{                 /*si tous les threads occupés ajouter la requete dans la file*/
   first =push(first,x);
   printf("thread occupé\n");
   pthread_mutex_unlock(&mutex);
    }
    
  }
}
