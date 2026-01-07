#include <stdio.h>      
#include <stdlib.h>     
#include <ctype.h>      
#include <string.h>     

// struktura jednog čvora u vezanoj listi (koristimo ju kao stog)
typedef struct Cvor {
    double vrijednost;      // svaki čvor čuva jedan broj (vrijednost)
    struct Cvor* next;      // pokazivač na sljedeći element u stogu
} Cvor;

// funkcija koja stvara novi čvor i vraća pokazivač na njega
Cvor* noviCvor(double vrijednost) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));  // alociram memoriju za novi čvor
    if (!novi) {                               // provjera da li je alokacija uspjela
        printf("Greska pri alokaciji memorije!\n");
        exit(1);                               // ako nije, izlaz iz programa
    }
    novi->vrijednost = vrijednost;              // spremam broj u čvor
    novi->next = NULL;                          // postavljam pokazivač na NULL
    return novi;                                // vraćam novi čvor
}

// funkcija za stavljanje elementa na vrh stoga (PUSH)
void push(Cvor** vrh, double vrijednost) {
    Cvor* novi = noviCvor(vrijednost);          // stvaram novi čvor
    novi->next = *vrh;                          // novi čvor pokazuje na dosadašnji vrh
    *vrh = novi;                                // novi čvor postaje novi vrh
}

// funkcija za skidanje elementa sa stoga (POP)
double pop(Cvor** vrh) {
    if (*vrh == NULL) {                         // provjera da li je stog prazan
        printf("Greska: stog je prazan!\n");
        exit(1);                                // ako je prazan, prekidam program
    }
    Cvor* temp = *vrh;                          // privremeno spremam vrh
    double vrijednost = temp->vrijednost;       // spremam vrijednost s vrha
    *vrh = temp->next;                          // pomičem vrh na sljedeći element
    free(temp);                                 // oslobađam memoriju
    return vrijednost;                          // vraćam skinutu vrijednost
}

// glavni dio programa
int main() {
    FILE* dat = fopen("zad5.txt", "r");         // otvaram datoteku za čitanje
    if (!dat) {                                 // provjera da li je uspješno otvorena
        printf("Greska pri otvaranju datoteke!\n");
        return 1;                               // prekidam program ako nije
    }

    Cvor* stog = NULL;                          // inicijalno je stog prazan
    char token[100];                            // ovdje spremam svaki pročitani token

    // čitam datoteku dok ima tokena (riječi ili brojeva)
    while (fscanf(dat, "%s", token) == 1) {
        // ako token počinje znamenkom ili je negativan broj (npr. -3)
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stog, atof(token));           // pretvaram string u broj i stavljam na stog
        }
        else {
            // inače je token operator (+, -, *, /)
            double b = pop(&stog);              // skidam drugi operand
            double a = pop(&stog);              // skidam prvi operand
            double rezultat;                    // ovdje spremam rezultat operacije

            // ovisno o operatoru radim odgovarajuću računsku operaciju
            switch (token[0]) {
            case '+': rezultat = a + b; break;   // zbrajanje
            case '-': rezultat = a - b; break;   // oduzimanje
            case '*': rezultat = a * b; break;   // množenje
            case '/':                            // dijeljenje
                if (b == 0) {                    // provjera da ne dijelim s nulom
                    printf("Greska: dijeljenje s nulom!\n");
                    fclose(dat);
                    return 1;                    // ako je 0, prekidam program
                }
                rezultat = a / b;
                break;
            default:                             // ako naiđem na nešto neočekivano
                printf("Nepoznat operator: %s\n", token);
                fclose(dat);
                return 1;
            }

            push(&stog, rezultat);                   // rezultat stavljam natrag na stog
        }
    }

    fclose(dat);                                    // zatvaram datoteku

    // ako nakon čitanja izraza stog nije točno jedan element, nešto nije u redu
    if (stog == NULL || stog->next != NULL) {
        printf("Greska u izrazu!\n");
        return 1;
    }

    // ispisujem konačni rezultat (ostaje samo jedan broj na stogu)
    printf("Rezultat izraza je: %.2f\n", pop(&stog));
    return 0;                                       // kraj programa
}
