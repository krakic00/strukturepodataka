#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>              
#include <stdlib.h>


// struktura čvora binarnog stabla pretraživanja
typedef struct Cvor {
    int vrijednost;             // vrijednost koja se sprema u čvor
    struct Cvor* lijevo;        // pokazivač na livo dijete
    struct Cvor* desno;         // pokazivač na desno dijete
} Cvor;

// funkcija za stvaranje novog čvora
Cvor* stvoriCvor(int x) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor)); // alokacija memorije
    if (novi == NULL)                          // provjera uspješnosti
        return NULL;                           // ako ne uspije, vrati NULL
    novi->vrijednost = x;                      // postavljanje vrijednosti
    novi->lijevo = NULL;                       // nema livo dijete
    novi->desno = NULL;                        // nema desno dijete

    return novi;                               // vraćanje čvora
}

// umetanje elementa u stablo
Cvor* umetni(Cvor* korijen, int x) {
    if (korijen == NULL)                       // ako je stablo prazno
        return stvoriCvor(x);                  // stvori novi čvor

    if (x < korijen->vrijednost)               // ako je manji
        korijen->lijevo = umetni(korijen->lijevo, x); // idi lijevo
    else if (x > korijen->vrijednost)          // ako je veći
        korijen->desno = umetni(korijen->desno, x);   // idi desno

    return korijen;                            // vrati korijen
}

// inorder ispis – vraća broj ispisanih elemenata
int inorder(Cvor* korijen) {
    int brojac = 0;                            // brojač elemenata
    if (korijen != NULL) {                     // ako čvor postoji
        brojac += inorder(korijen->lijevo);   // livo podstablo
        printf("%d ", korijen->vrijednost);   // ispis
        brojac++;                              // povećaj brojač
        brojac += inorder(korijen->desno);    // desno podstablo
    }
    return brojac;                             // vrati broj elemenata
}

// preorder ispis – vraća broj ispisanih elemenata
int preorder(Cvor* korijen) {
    int brojac = 0;                            // brojač
    if (korijen != NULL) {
        printf("%d ", korijen->vrijednost);   // ispis
        brojac++;                              // povećaj brojač
        brojac += preorder(korijen->lijevo);  // lijevo
        brojac += preorder(korijen->desno);   // desno
    }
    return brojac;                             // vrati broj
}

// postorder ispis – vraća broj ispisanih elemenata
int postorder(Cvor* korijen) {
    int brojac = 0;                            // brojač
    if (korijen != NULL) {
        brojac += postorder(korijen->lijevo); // lijevo
        brojac += postorder(korijen->desno);  // desno
        printf("%d ", korijen->vrijednost);   // ispis
        brojac++;                              // povećaj brojač
    }
    return brojac;                             // vrati broj
}

// pronalaženje minimalnog čvora
Cvor* minCvor(Cvor* korijen) {
    Cvor* trenutni = korijen;                  // početni čvor
    while (trenutni != NULL && trenutni->lijevo != NULL)
        trenutni = trenutni->lijevo;           // idi skroz lijevo
    return trenutni;                           // vrati najmanji
}

// brisanje elementa iz stabla
Cvor* obrisi(Cvor* korijen, int x) {
    if (korijen == NULL)                       // ako je prazno
        return NULL;                           // nema brisanja

    if (x < korijen->vrijednost)               // ako je manji
        korijen->lijevo = obrisi(korijen->lijevo, x);
    else if (x > korijen->vrijednost)          // ako je veći
        korijen->desno = obrisi(korijen->desno, x);
    else {                                     // pronađen čvor
        if (korijen->lijevo == NULL) {         // nema livo dijete
            Cvor* temp = korijen->desno;       // spremi desno
            free(korijen);                     // oslobodi memoriju
            return temp;                       // vrati novo stablo
        }
        if (korijen->desno == NULL) {          // nema desno dijete
            Cvor* temp = korijen->lijevo;      // spremi lijevo
            free(korijen);                     // oslobodi memoriju
            return temp;                       // vrati novo stablo
        }
        Cvor* temp = minCvor(korijen->desno);  // najmanji u desnom
        korijen->vrijednost = temp->vrijednost; // kopiranje
        korijen->desno = obrisi(korijen->desno, temp->vrijednost);
    }
    return korijen;                            // vrati korijen
}

// pretraživanje – vraća 1 ako postoji, 0 ako ne postoji
int pretrazi(Cvor* korijen, int x) {
    if (korijen == NULL)                       // ako je prazno
        return 0;                              // ne postoji
    if (korijen->vrijednost == x)              // ako je pronađen
        return 1;                              // postoji
    if (x < korijen->vrijednost)               // ako je manji
        return pretrazi(korijen->lijevo, x);   // livo
    else
        return pretrazi(korijen->desno, x);    // desno
}

// funkcija za izbornik – vraća korisnikov izbor
int izbornik() {
    int izbor;
    printf("\n1 - Umetni element\n");
    printf("2 - Inorder ispis\n");
    printf("3 - Preorder ispis\n");
    printf("4 - Postorder ispis\n");
    printf("5 - Obrisi element\n");
    printf("6 - Pretrazi element\n");
    printf("0 - Izlaz\n");
    printf("Izbor: ");
    scanf("%d", &izbor);                      // unos
    return izbor;                              // vrati izbor
}

// glavna funkcija – samo poziva druge funkcije
int main() {
    Cvor* korijen = NULL;                      // prazno stablo
    int izbor, x;                              // pomoćne varijable

    do {
        izbor = izbornik();                    // poziv funkcije izbornika

        if (izbor == 1) {
            scanf("%d", &x);                  // unos vrijednosti
            korijen = umetni(korijen, x);      // umetanje
        }
        else if (izbor == 2) {
            inorder(korijen);                  // inorder ispis
        }
        else if (izbor == 3) {
            preorder(korijen);                 // preorder ispis
        }
        else if (izbor == 4) {
            postorder(korijen);                // postorder ispis
        }
        else if (izbor == 5) {
            scanf("%d", &x);                  // unos vrijednosti
            korijen = obrisi(korijen, x);      // brisanje
        }
        else if (izbor == 6) {
            scanf("%d", &x);                  // unos vrijednosti
            if (pretrazi(korijen, x))           // pretraga
                printf("Postoji\n");
            else
                printf("Ne postoji\n");
        }

    } while (izbor != 0);                      // ponavljaj

    return 0;                               
}
