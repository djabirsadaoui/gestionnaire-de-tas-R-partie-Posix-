struct zone_alloc{
           int port;   
           char ip[20];          
           int type_acces;
           int adresse;
           int taille;
           int verrou;
           int libre;
           char name[20];
           struct zone_alloc *next;
       };
int *push_freelist (int adr,int size);
struct zone_alloc  *pop_freelist(char name[20],int taille);