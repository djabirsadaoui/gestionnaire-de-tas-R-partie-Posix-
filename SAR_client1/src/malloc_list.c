#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct zone_alloc{
           int port;
           char ip[15];
           int val;
           int adresse;
           int taille;
           int compteur;
           int request;          
           char name[20];
           struct zone_alloc *next;
       };  
struct zone_libre{
        int adresse;
        int taille;
        struct zone_libre *p_suivant;
       }; 

       /*changement de structure alloc et free list*/
       
       struct zone_alloc *p_last,*pp_first;      /* 2 pointeurs pour vers la fin et le debut*/       
      int *push_alloclist(struct zone_alloc *a){
       struct zone_alloc *p_new = malloc (sizeof *p_new);
         
         if (p_new != NULL){                      /*initialisation de la zone*/        
             p_new=a;
         }
         if(p_last==NULL){              /*si la liste de malloc est vide*/
             p_last=p_new;
             pp_first=p_last;
         }
         else{                            /*s'il existe déja des zone*/
             p_last->next=p_new;                /*pointer vers cette zone*/
             p_last=p_new;                      /* last devient la nouvelle zone ajoutée à laliste*/
         }          
         return 0;
     }
     struct zone_libre *pop_alloclist(char name[]){
         struct zone_libre *a=malloc(sizeof(a));              
         struct zone_alloc *p;           /*pointeur pour parcourir la liste*/
         struct zone_alloc *prec;        /* poiteur precede tjr p*/
         int found=1;
         p=pp_first;
         if(p==NULL){                   /*si la liste est vide*/
             a=NULL;
             return a;
         }
         else{                                          /*s'il existe des zones dans la liste*/            
         while(found && p!=NULL ){                      /*tant que il n'a pas trouvé la zone et n'a pas parcouri toute la liste*/
           if((strcmp(name,p->name))==0){             /* tester s'ils ont le même nom */
             if((p==pp_first)){                            /*si la zone trouvée c'est celle de header*/
                 a->adresse=p->adresse;
                 a->taille=p->taille;
                 a->p_suivant=NULL;
                 found=0;
                 if(p->next==NULL){
                    pp_first=NULL;
                    p_last=NULL;  
                 }
                 else{
                     pp_first=pp_first->next;
                 }
                
             }
             else{                                      
                 if((prec==pp_first) && (p->next==NULL)){                     /*si la zone trouvée est située just après le header*/
                         a->adresse=p->adresse;
                         a->taille=p->taille;
                         a->p_suivant=NULL;
                         found=0;
                         pp_first->next=NULL;
                         p_last=pp_first;
                 }
                 else{                                  
                     if(p->next==NULL){                 /*si la zone trouvée est située a la fin de la liste*/
                         a->adresse=p->adresse;
                         a->taille=p->taille;
                         a->p_suivant=NULL;
                         found=0;
                         prec->next=NULL;
                         p_last=prec;                        
                     }
                     else{
                         a->adresse=p->adresse;
                         a->taille=p->taille;
                         a->p_suivant=NULL;
                         found=0;
                         prec->next=p->next;                         
                     }
                 }
             }
           }
           else{                                        /*s'ils ont pas le même nom*/                            
               prec=p;
               p=p->next;
           }
         }
     }
         if(found){                                    /* s'il n'a pas trouvé le nom demandé */
          a=NULL;
          return a;
         }
                                           
             return a;          
}
     int existe(char name[20]){
         struct zone_alloc *p;
         while(p!=NULL){
         if(strcmp(p->name,name))
             p=p->next;
         else
             return 0;
         }
         return -1;
     }
      
     
     struct zone_alloc *get_zone(char name[]){
         struct zone_alloc *p;        
         p=pp_first;
         while(p!=NULL){
             if((strcmp(name,p->name))==0)
              return p;
             else 
                 p=p->next;  
     }
         
             p=NULL;
             return p;
         }
     
   struct zone_alloc *get_zone_adr(int adr){
    
       struct zone_alloc *p;
         p =pp_first;
                while (p!=NULL){
                    if(p->adresse==adr){
                       return p;  
                    }
                    else{
                        p=p->next;
                    }
             }                              /*recuperer la variable de la liste chainée*/
   
}
   
