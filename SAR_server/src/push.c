#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
struct fool{
        int val;
        struct fool *next;

       };
struct fool *push (struct fool *p_head, int value){
   /* allocation d'un nouveau structure */
   struct fool *p_new = malloc (sizeof *p_new);
    /* si tout s'est bien passe : */
   if (p_new != NULL)
   {
      /* mise a jour des champs : */
      /* donnees */
      p_new->val= value;
      
      /* chainage par defaut */
      
      p_new->next = NULL;

      /* chainage */
      if (p_head == NULL)               /* si la file est vide */    {
         
         p_head = p_new;
      }
      else
      {
         /* on cherche le dernier noeud */
         struct fool *p = p_head;
         while (p->next != NULL)
         {
            /* pointer sur le suivant */
            p = p->next;
         }
         /* modification du chainage */
         p->next = p_new;
         
      }
   }
   printf("element a ete bien stocke\n");
   return p_head;
}
