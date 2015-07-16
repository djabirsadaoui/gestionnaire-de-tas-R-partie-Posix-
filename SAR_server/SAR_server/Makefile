all:    bin/Server_final 
					 
	
obj/pop.o: src/pop.c
	gcc  -o  obj/pop.o -c src/pop.c -Wall -ansi
	
obj/afficher_erreur.o: src/afficher_erreur.c
	gcc  -o obj/afficher_erreur.o -c src/afficher_erreur.c -Wall -ansi
	
obj/push.o: src/push.c
	gcc  -o  obj/push.o -c src/push.c -Wall -ansi
	
obj/malloc_list.o : src/malloc_list.c 
	gcc -c -o obj/malloc_list.o src/malloc_list.c  -g -Wall 
	
obj/free_list.o : src/free_list.c
	gcc -c -o obj/free_list.o src/free_list.c  -g -Wall 
	
obj/initialiser.o : src/initialiser.c  include/free_list.h  
	gcc -c -o obj/initialiser.o src/initialiser.c  -g -Wall 

obj/allouer.o : src/allouer.c include/malloc_list.h include/free_list.h 
		gcc -c -o obj/allouer.o src/allouer.c  -g -Wall
	
obj/create_sockcontrol.o : src/create_sockcontrol.c include/afficher_erreur.h
	gcc -c -o obj/create_sockcontrol.o src/create_sockcontrol.c  -g -Wall
 
obj/routine.o : src/routine.c include/afficher_erreur.h include/malloc_list.h include/free_list.h include/allouer.h
	gcc -c -o obj/routine.o src/routine.c  -g -Wall



obj/Server_final.o : src/Server_final.c include/create_sockcontrol.h include/afficher_erreur.h include/routine.h include/pop.h include/push.h include/initialiser.h  include/free_list.h include/malloc_list.h include/allouer.h
	gcc -c -o obj/Server_final.o src/Server_final.c  -g -Wall  -lpthread 

bin/Server_final : obj/Server_final.o obj/create_sockcontrol.o obj/allouer.o obj/routine.o obj/afficher_erreur.o obj/pop.o obj/push.o obj/initialiser.o obj/free_list.o   obj/malloc_list.o 
	gcc -o bin/Server_final obj/Server_final.o obj/create_sockcontrol.o obj/routine.o obj/allouer.o obj/afficher_erreur.o obj/pop.o obj/push.o obj/initialiser.o obj/free_list.o  obj/malloc_list.o  -g -Wall  -lpthread 
	