#define _XOPEN_SOURCE 700
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h> /*pour les types de sockets*/
#include<sys/socket.h>
#include<sys/resource.h>
#include<netinet/in.h>
#include<error.h>
#include<netdb.h>
#include <arpa/inet.h>
#define FREE_READ 4
//#define  PORT_CLIENT 8081

struct sockaddr_in clnt;
struct sockaddr_in client2;
struct sockaddr_in client3;

struct client_info_client{
    int type_acces;
    char name[20];
};
struct reponse_client{
       int val;
        int port;
        char ip[15];
        int taille;
    };
struct client_info_server{
    int type_acces;
    char name[20];
    char ip[15];
    int port;
    int taille;
};
struct reponse_server {
    char ip[15];
    int port;
    int verrou;
    int taille;
};



int req_malloc(struct client_info_server d,int s){         /*fait une requete malloc au serveur et renvoie @*/
 
     int sock_com1;
     int reponse; 
    int taille_soc = sizeof(clnt);    
    if((sock_com1=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    /*remplir la zone mÃ©moire de client par 0*/
    memset((void*)&clnt,0,sizeof(clnt));
    /*creation le nom de socket*/
    clnt.sin_addr.s_addr=inet_addr("127.0.0.1");
    clnt.sin_family =AF_INET;
    clnt.sin_port=htons(s);
    
     if(connect(sock_com1,(struct sockaddr*)&clnt,taille_soc)<0){
       perror("connect");
       exit(1);
        }    
    if(write(sock_com1,&d,sizeof(d))<0){
        perror("send");
        exit(1);
    }  
   
   
    if(read(sock_com1,&reponse,sizeof(reponse))<0){
        perror("read malloc");
        exit(0);
    }
   
    shutdown(sock_com1,2);
    close(sock_com1); 
        return reponse;  
} 


struct reponse_client req_client(struct client_info_client info,int s){ /*fait requete au client et renvoie la valeur et @ de client*/
    int taille_soc = sizeof(client2); 
    struct reponse_client reponse;
    int sock_com2;

    if((sock_com2=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    /*remplir la zone mÃ©moire de client par 0*/
    memset((void*)&client2,0,sizeof(client2));
    /*creation le nom de socket*/
    client2.sin_addr.s_addr=inet_addr("127.0.0.1");
    client2.sin_family =AF_INET;
    client2.sin_port=htons(s); 
    
     if(connect(sock_com2,(struct sockaddr*)&client2,taille_soc)<0){
       perror("connect");
          exit(1);    
        }    
    if(write(sock_com2,&info,sizeof(info))<0){
        perror("send");
                exit(1);
    }  
    if(info.type_acces==FREE_READ){
        shutdown(sock_com2,2);
        close(sock_com2); 
        return reponse;  
    }
    if(read(sock_com2,&reponse,sizeof(reponse))<0){
        perror("read client");
        exit(0);
    }   
    shutdown(sock_com2,2);
    close(sock_com2); 
    return reponse;  
} 

  struct reponse_server req_rerver(struct client_info_server info,int s){           /*fait une sequete au server et renvoie @ de variable*/
   struct reponse_server reponse;   
   int sock_com1;
     
    int taille_soc = sizeof(client3);    
    if((sock_com1=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket");
        exit(1);
    }
    /*remplir la zone mÃ©moire de client par 0*/
    memset((void*)&client3,0,sizeof(client3));
    /*creation le nom de socket*/
    client3.sin_addr.s_addr=inet_addr("127.0.0.1");
    client3.sin_family =AF_INET;
    client3.sin_port=htons(s);
    
     if(connect(sock_com1,(struct sockaddr*)&client3,taille_soc)<0){
       perror("connect server");
       exit(1);
        }    
    if(write(sock_com1,&info,sizeof(info))<0){
        perror("send");
        exit(1);
    }  
   
   
    if(read(sock_com1,&reponse,sizeof(reponse))<0){
        perror("read malloc");
        exit(0);
    }
   
    shutdown(sock_com1,2);
    close(sock_com1); 
        return reponse;  
} 
