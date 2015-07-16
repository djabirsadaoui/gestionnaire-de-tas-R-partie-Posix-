#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct zone_libre{
        int adresse;
        int taille;
        struct zone_libre *p_suivant;

       }; 
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
       
      
volatile struct zone_libre *p_first;



int *push_freelist (int adr,int size){
int found=1;
struct zone_libre *p_new = malloc (sizeof *p_new);          /* allocation d'un nouvelle zone */
if (p_new != NULL){                                         /*initialiser cette zone*/        
      p_new->adresse= adr;                                  /* adresse */     
      p_new->taille=size;        /*taille*/ 
      p_new->p_suivant = NULL;
   }
      
      if (p_first == NULL)                                  /* si la file est vide */             
         p_first = p_new;     
      else{                                               /* on cherche l'emplacement de ajout de la zone selon ordre croissance de adresse */         
         struct zone_libre *p = p_first;
         struct zone_libre *a=NULL;
         while (found && p!=NULL){
             if(p->adresse > p_new->adresse){            /* si @ de la zone est inferieur @ de header*/
                  if(p==p_first){                         
                      if((p_new->adresse+p_new->taille)== (p->adresse)){
                          p_new->taille=p_new->taille+p->taille; /*add taille ancien header à taille nouveau header */
                          p_new->p_suivant=p->p_suivant;
                          p_first=p_new;
                          found=0;
                      }
                      else{
                      p_new->p_suivant=p;
                      p_first=p_new;
                      found=0;
                        }                 
                 } 
                 else{
                      if(((p_new->adresse)==(a->adresse+a->taille))&&((p_new->adresse+p_new->taille)==(p->adresse))){
                          a->taille=a->taille+p_new->taille+p->taille;                   /*fusion avec voisin gauche et droite*/
                          a->p_suivant=p->p_suivant; 
                          found=0;
                      }
                      else{
                          if((p_new->adresse)==(a->adresse+a->taille)){                  /* fusion avec voisin gauche*/
                              a->taille=a->taille+p_new->taille;
                              a->p_suivant=p;
                              found=0;
                          }
                          else{
                              if((p_new->adresse+p_new->taille)==(p->adresse)){         /*fusion avec voisin droite*/
                                  p_new->taille=p_new->taille+p->taille;
                                  p_new->p_suivant=p->p_suivant;
                                  a->p_suivant=p_new;
                                  found=0;                       
                              }
                              else{                                                      /* pas de fusion*/
                                 p_new->p_suivant=p;
                                 a->p_suivant=p_new;
                                 found=0;
                              }
                          }
                      }
                      
                  }
             }                      
             else{                                                       /* si @ n'est pas inferieur */
              a=p;                                                         /*je guarde l'element précedent */
              p = p->p_suivant;
             }
    }  
         if(found){     /*si tous les elemens de la file inferieurs */
             if((p_new->adresse)==(a->adresse+a->taille)){      /* fusion gauche avec le dernier element*/
                 a->taille=a->taille+p_new->taille;
//                 a->p_suivant=NULL;
             }
             else{
                 a->p_suivant=p_new;    /* pas de fusion*/
             }
             
         }      
    }
    printf("adresse =%d , taille=%d\n",p_first->adresse,p_first->taille);
    return 0;
   }



/*changement de structure alloc et free */



struct zone_alloc  *pop_freelist(char name[20],int taille){
    int found=1;
    struct zone_libre *p,*a;
    struct zone_alloc *p_new=malloc(sizeof(p_new)) ;
    /* initialiser la zone */  
    if (p_new != NULL){                
     p_new->taille=taille;              /* adresse */ 
     p_new->next=NULL;                  /*taille*/   
     strcpy(p_new->name,name);            /*name*/          
      }
   
    if(p_first==NULL) {                  /* si free_list est vide*/
         p_new=NULL;
         return p_new;
         printf("pop_free: le tas est plein\n");
     }
     else{
     if(p_first!=NULL){                              /* s'il ya espace libre*/
         p=p_first;
         while(p!=NULL){
            if((p->taille) >= taille){          /* s'il y un element avec une taille suffisante*/
                if((p->taille-taille)==0){      /* si espace trouvé egale exactement à la taille cherché*/
                    if(p==p_first){
                        p_first=(p->p_suivant);
                        p_new->adresse=p->adresse;
                        found=0;
                        break;
                    }
                    else{
                        p_new->adresse=p->adresse;
                        a->p_suivant=p->p_suivant; 
                        found=0;
                        break;
                    }
                }
                else{
                    p_new->adresse=p->adresse;
                    p->taille=p->taille-taille;
                    p->adresse=p->adresse+taille;
                    found=0;
                    break;
                }
            }
            else{
                a=p;
                p=p->p_suivant;
            } 
         }
         
     }
     }
     
     if(found==1){
         p_new=NULL;
         return p_new;
     }
       
//     printf("nom :%s et taille=%d et adresse=%d et compteur=%d\n",p_new->name,p_new->taille,p_new->adresse,p_new->cmt);     
     return p_new;
    
}
