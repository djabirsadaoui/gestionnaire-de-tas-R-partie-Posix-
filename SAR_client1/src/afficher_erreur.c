#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void afficher_erreur(const char* msg)
{
  fprintf(stderr, "%s (errno: %s)\n", msg, strerror(errno));
  exit(1);
}
