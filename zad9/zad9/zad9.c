#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struktura cvora binarnog stabla
typedef struct Cvor {
    int vrijednost;
    struct Cvor* lijevo;
    struct Cvor* desno;
} Cvor;

// stvara novi cvor i vraca pokazivac
Cvor* noviCvor(int x) {
    Cvor* n = (Cvor*)malloc(sizeof(Cvor));
    n->vrijednost = x;
    n->lijevo = NULL;
    n->desno = NULL;
    return n;
}

// dodaje element u stablo po razinama
Cvor* insert(Cvor* korijen, int x) {

    if (korijen == NULL)
        return noviCvor(x);

    Cvor* red[100];
    int poc = 0, kraj = 0;

    red[kraj++] = korijen;

    while (poc < kraj) {
        Cvor* tren = red[poc++];

        if (tren->lijevo == NULL) {
            tren->lijevo = noviCvor(x);
            break;
        }
        else {
            red[kraj++] = tren->lijevo;
        }

        if (tren->desno == NULL) {
            tren->desno = noviCvor(x);
            break;
        }
        else {
            red[kraj++] = tren->desno;
        }
    }

    return korijen;
}

// zamjenjuje vrijednosti cvorova zbrojem potomaka
int replace(Cvor* korijen) {

    if (korijen == NULL)
        return 0;

    int lijevo = replace(korijen->lijevo);
    int desno = replace(korijen->desno);

    int stara = korijen->vrijednost;
    korijen->vrijednost = lijevo + desno;

    return stara + korijen->vrijednost;
}

// inorder prolazak i ispis u datoteku
int inorder(Cvor* korijen, FILE* f) {

    if (korijen == NULL)
        return 0;

    inorder(korijen->lijevo, f);
    fprintf(f, "%d ", korijen->vrijednost);
    inorder(korijen->desno, f);

    return 1;
}

// generira stablo sa slucajnim brojevima
Cvor* generirajStablo(int n) {

    Cvor* korijen = NULL;
    int i;

    for (i = 0; i < n; i++) {
        int x = rand() % 81 + 10;
        korijen = insert(korijen, x);
    }

    return korijen;
}

int main() {

    FILE* f;
    Cvor* korijen;

    srand(time(NULL));              // inicijalizacija rand()

    korijen = generirajStablo(9);   // stvaranje stabla

    f = fopen("izlaz.txt", "w");    // otvaranje datoteke

    inorder(korijen, f);            // ispis nakon insert

    replace(korijen);               // zamjena vrijednosti

    fprintf(f, "\n");

    inorder(korijen, f);            // ispis nakon replace

    fclose(f);                      // zatvaranje datoteke

    return 0;
}

