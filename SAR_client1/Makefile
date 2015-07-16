all: bin/client1 			
	
obj/pop.o: src/pop.c
	gcc -c -o  obj/pop.o  src/pop.c -Wall -ansi
	
obj/afficher_erreur.o: src/afficher_erreur.c
	gcc -c -o obj/afficher_erreur.o  src/afficher_erreur.c -Wall -ansi
	
obj/push.o: src/push.c
	gcc  -c -o  obj/push.o src/push.c -Wall -ansi
	
obj/malloc_list.o : src/malloc_list.c 
	gcc -c -o obj/malloc_list.o src/malloc_list.c  -g -Wall 
	
obj/free_list.o : src/free_list.c
	gcc -c -o obj/free_list.o src/free_list.c  -g -Wall 
	
obj/initialiser.o : src/initialiser.c  include/free_list.h  
	gcc -c -o obj/initialiser.o src/initialiser.c  -g -Wall 

obj/allouer.o : src/allouer.c include/malloc_list.h include/free_list.h 
		gcc -c -o obj/allouer.o src/allouer.c  -g -Wall
	
	
obj/client2.o : src/client2.c
	gcc -c -o obj/client2.o src/client2.c  -g -Wall 
bin/client2 : obj/client2.o
	gcc  -o bin/client2 obj/client2.o -g -Wall
	   
	
	

	
obj/t_fonction.o : src/t_fonction.c  include/req_connexion.h  include/create_sockcontrol.h include/malloc_list.h include/free_list.h  include/allouer.h
	gcc -c -o obj/t_fonction.o src/t_fonction.c  -g -Wall 
	
obj/create_sockcontrol.o : src/create_sockcontrol.c include/afficher_erreur.h
	gcc -c -o obj/create_sockcontrol.o src/create_sockcontrol.c  -g -Wall
 


obj/req_connexion.o : src/req_connexion.c 
	gcc -c -o obj/req_connexion.o src/req_connexion.c  -g -Wall 
	
obj/demande_client.o : src/demande_client.c include/allouer.h include/malloc_list.h include/free_list.h include/afficher_erreur.h
	gcc -c -o obj/demande_client.o src/demande_client.c  -g -Wall 	
	
	
	
obj/client1.o : src/client1.c include/create_sockcontrol.h include/allouer.h include/t_fonction.h include/afficher_erreur.h   include/pop.h include/req_connexion.h  include/push.h include/initialiser.h include/free_list.h  include/malloc_list.h include/demande_client.h
	gcc -c -o obj/client1.o src/client1.c  -g -Wall 

bin/client1 : obj/client1.o obj/create_sockcontrol.o obj/allouer.o obj/t_fonction.o obj/afficher_erreur.o obj/pop.o obj/req_connexion.o  obj/push.o obj/initialiser.o obj/free_list.o  obj/malloc_list.o   obj/demande_client.o
	gcc  -o bin/client1 obj/client1.o obj/create_sockcontrol.o  obj/allouer.o obj/t_fonction.o obj/afficher_erreur.o obj/pop.o obj/req_connexion.o obj/push.o obj/initialiser.o obj/free_list.o  obj/malloc_list.o   obj/demande_client.o -g -Wall -lpthread 
	
	
				 
	
