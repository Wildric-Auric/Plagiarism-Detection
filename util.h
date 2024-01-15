#ifndef _UTIL_HEADER_FILE_
#define _UTIL_HEADER_FILE_

struct alignement {
    char* x;
    char* y;
};


char* readtextfile(char* filename);

int Imax(int a, int b);

int Imin2(int a, int b);

int Imin(int a, int b, int c);

void retourne(char* c);

void afficheSeparateurHorizontal(int nbcar);

void affiche(char* texte1, char* texte2, int nbcar);

void affiche2(char* texte1, char* texte2, int nbcar);
#endif