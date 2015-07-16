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
int *push_alloclist(struct zone_alloc *a);
struct zone_libre *pop_alloclist(char tab[]);
int existe(char name[20]);
struct zone_alloc *get_zone(char name[]);
struct zone_alloc *get_zone_adr(int adr);