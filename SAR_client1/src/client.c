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
#include "../include/afficher_erreur.h" 
#include "../include/allouer.h" 
#include "../include/push.h"
#include "../include/free_list.h"
#include "../include/create_sockcontrol.h"
#include "../include/initialiser.h"
#include "../include/allouer.h"
#include "../include/malloc_list.h" 
#include "../include/demande_client.h"
#include "../include/t_fonction.h"
#include "../include/req_connexion.h"
#include "../include/t_fonction.h"


#define NB 2
#define PORT 1500
 
 int nb_endormies;      /* nombre de threads endormies */
 int clnt;            /* connection recue en attente de traitement */
 pthread_mutex_t mutex_client;          /* protege les variables partagees */
 pthread_cond_t  nb_changed;     /* signal que 'nb' a ete modifie */
 pthread_cond_t  client_changed; /* signal que 'client' a ete modifie */
 struct fool{
        int val;
        struct fool *next;
       };

struct fool *first;     /*represente le premier element dans la file d'attente*/
//void routine(int sok);          /*la routine excutée par le thread */
void consomme_req(struct fool *p);      /*retirer une requete de la file d'attente*/
void* traiter_requete(void* arg);       /*la gestion des threads*/
void attendre_connexion();              /*attendere les requetes des clients */
int main() {
  pthread_t id[NB];
  int i;  
  int adr;
  nb_endormies = NB;            /*  initialise le pool de thread */  
  clnt = -1;
  int adresse;
  int taille=1024;
  adresse=creer_segment("M2",taille);        /*creer un segment de memoire */
  initialiser(adresse,taille);               /*initialiser free_list*/
  adr=t_malloc(40,"a");
  printf("valeur1=%d\n",adr);
  errno = pthread_mutex_init(&mutex_client, NULL);
        if (errno) afficher_erreur("echec de pthread_mutex_init");
  errno = pthread_cond_init(&client_changed, NULL);
        if (errno) afficher_erreur("echec de pthread_cond_init");
  for (i=0; i<NB; i++)  {
    errno = pthread_create(&id[i], NULL, traiter_requete, NULL);
         if (errno) afficher_erreur("echec de pthread_create");
  } 
  signal(SIGPIPE, SIG_IGN);      /* supprime SIGPIPE */  
  attendre_connexion();         /* lance le serveur */
        
  return 0;  
}

/*la routine excutépar le thread*/

void consomme_req(struct fool *p){
     
     job(p->val);/*effectueer le traitement de la premiere requte dans la file  */
     first= p->next; /*changer le premier element dans la file d'attente*/    
 } 
  void* traiter_requete(void* arg){
  struct fool *p;   
  int soc; 
  while (1) {              
    errno = pthread_mutex_lock(&mutex_client);
                if (errno) afficher_erreur("echec de pthread_mutex_lock1");
    while(clnt==-1){          /* attend une nouvelle requete à traiter */ 
        errno = pthread_cond_wait(&client_changed, &mutex_client);  
                if (errno)afficher_erreur("echec de pthread_cond_wait");
    }
    soc=clnt;                 /*prendre la requete recu*/
    printf("valeur client=%d\n",clnt);
    clnt=-1;
    nb_endormies--;          /*décrimenter le nombre des thread sans travail*/
//    printf("le nbr thread decrimente=%d\n",nb_endormies);
    errno = pthread_mutex_unlock(&mutex_client);
            if (errno) afficher_erreur("echec de pthread_mutex_unlock2");
//    printf("%d debut de connection\n", pthread_self());    
    job(soc);            /* executer la routine de la requete */
//            sleep(10);
    pthread_mutex_lock(&mutex_client); 
   
    while(first!=NULL){                  /*s'il y a des requetes dans la fille d'attente*/
         p=pop(first);                   /*recupérer le premier element dans la file d'attente */
         consomme_req(p);                /*effectuer sa traitement */
         pthread_mutex_unlock(&mutex_client);
          printf("moi %d j'ai termine mon travail\n",pthread_self());
    }
    pthread_mutex_unlock(&mutex_client);
    errno=pthread_mutex_lock(&mutex_client);
        if (errno) afficher_erreur("echec lock 3");
           
    nb_endormies++;                     /*incrementer le nombre des thread sans travail*/
//    printf("le nbr de thread=%d\n",nb_endormies);
    errno= pthread_mutex_unlock(&mutex_client);
        if (errno) afficher_erreur("echec unlock 3");
  }
}

  void attendre_connexion(){
int sock_contr=cree_socket_ecoute(PORT);   
   printf("Serveur actif sur le port  %d socket %d\n",PORT,sock_contr);
  while (1) {
    int x;
    x = accept(sock_contr,NULL, 0);   
    errno = pthread_mutex_lock(&mutex_client);
        if (errno) afficher_erreur("echec de pthread_mutex_lock5");
    if(nb_endormies){           /*s'il ya des threads libres*/
        clnt = x;
        printf("la valeur client=%d\n",clnt);
                if (clnt==-1) {
                        if (errno==EINTR || errno==ECONNABORTED) continue; /* non fatal */
                                 afficher_erreur("echec de accept");
                } 
         errno = pthread_cond_signal(&client_changed);
                if (errno) afficher_erreur("echec de pthread_cond_signal");
         pthread_mutex_unlock(&mutex_client);
    }    
    else{                 /*si tous les threads occupés ajouter la requete dans la file*/
   first =push(first,x); 
   pthread_mutex_unlock(&mutex_client);
    }
    
  }
}

