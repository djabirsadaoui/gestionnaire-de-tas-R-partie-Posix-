struct client_info_client{
    int type_acces;
    char name[20];
};
struct client_info_server{
    int type_acces;
    char name[20];
    char ip[15];
    int port;
    int taille;
};

int req_malloc(struct client_info_server d,int s);
struct reponse_server req_rerver(struct client_info_server info,int s);
struct reponse_client req_client(struct client_info_client info,int s);