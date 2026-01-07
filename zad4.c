#include <stdio.h>    // za ulaz/izlaz
#include <stdlib.h>   // za malloc, free, exit itd.

// struktura koja predstavlja jedan član polinoma (čvor liste)
typedef struct Clan {
    int koef;              // koeficijent
    int exp;               // eksponent
    struct Clan* next;     // pokazivač na sljedeći čvor
} Clan;

// funkcija za stvaranje novog čvora
Clan* noviClan(int koef, int exp) {
    Clan* novi = (Clan*)malloc(sizeof(Clan)); // alociramo memoriju
    novi->koef = koef;                        // postavljamo koeficijent
    novi->exp = exp;                          // postavljamo eksponent
    novi->next = NULL;                        // inicijalno nema sljedećeg
    return novi;                              // vraćamo pokazivač na novi čvor
}

// funkcija za dodavanje člana u polinom na kraj liste
void dodajNaKraj(Clan** glava, int koef, int exp) {
    Clan* novi = noviClan(koef, exp);         // stvori novi čvor
    if (*glava == NULL) {                     // ako je lista prazna
        *glava = novi;                        // novi postaje glava
        return;
    }
    Clan* temp = *glava;                      // inače idemo do kraja liste
    while (temp->next) temp = temp->next;
    temp->next = novi;                        // dodaj novi čvor na kraj
}

// funkcija za ispis polinoma
void ispisiPolinom(Clan* p) {
    while (p) {                               // prolazimo kroz sve čvorove
        if (p->koef == 0) { p = p->next; continue; } // preskačemo nule
        if (p != NULL && p != p->next && p->next && p->next->koef > 0)
            printf("%dx^%d+", p->koef, p->exp);
        else
            printf("%dx^%d", p->koef, p->exp);
        if (p->next && p->next->koef < 0) printf(""); // minus se ispisuje automatski
        p = p->next;
    }
    printf("\n");
}

// funkcija za umetanje člana u polinom (sortirano po eksponentima silazno)
void umetniSortirano(Clan** glava, int koef, int exp) {
    Clan* novi = noviClan(koef, exp);
    // ako je lista prazna ili novi ima veći eksponent od prvog
    if (*glava == NULL || (*glava)->exp < exp) {
        novi->next = *glava;
        *glava = novi;
        return;
    }
    Clan* temp = *glava;
    // tražimo mjesto za umetanje (po silaznom redoslijedu)
    while (temp->next && temp->next->exp > exp) temp = temp->next;

    // ako postoji isti eksponent, samo zbrojimo koeficijente
    if (temp->next && temp->next->exp == exp) {
        temp->next->koef += koef;
        free(novi); // novi nije potreban
        return;
    }

    // umetanje između čvorova
    novi->next = temp->next;
    temp->next = novi;
}

// funkcija za čitanje jednog polinoma iz datoteke
Clan* ucitajPolinom(FILE* fp) {
    Clan* glava = NULL;
    int koef, exp;
    // čitamo parove (koef, exp) do kraja retka
    while (fscanf(fp, "%d %d", &koef, &exp) == 2) {
        umetniSortirano(&glava, koef, exp);
        int c = fgetc(fp);
        if (c == '\n' || c == EOF) break; // kraj retka
    }
    return glava;
}

// funkcija za zbrajanje dvaju polinoma
Clan* zbroji(Clan* p1, Clan* p2) {
    Clan* rezultat = NULL;
    while (p1 && p2) {
        if (p1->exp == p2->exp) {                       // isti eksponent
            int noviKoef = p1->koef + p2->koef;
            if (noviKoef != 0)
                umetniSortirano(&rezultat, noviKoef, p1->exp);
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->exp > p2->exp) {                   // veći eksponent
            umetniSortirano(&rezultat, p1->koef, p1->exp);
            p1 = p1->next;
        }
        else {
            umetniSortirano(&rezultat, p2->koef, p2->exp);
            p2 = p2->next;
        }
    }
    // dodaj preostale članove
    while (p1) { umetniSortirano(&rezultat, p1->koef, p1->exp); p1 = p1->next; }
    while (p2) { umetniSortirano(&rezultat, p2->koef, p2->exp); p2 = p2->next; }
    return rezultat;
}

// funkcija za množenje dvaju polinoma
Clan* pomnozi(Clan* p1, Clan* p2) {
    Clan* rezultat = NULL;
    for (Clan* i = p1; i != NULL; i = i->next) {
        for (Clan* j = p2; j != NULL; j = j->next) {
            int noviKoef = i->koef * j->koef;          // množimo koeficijente
            int noviExp = i->exp + j->exp;             // zbrajamo eksponente
            umetniSortirano(&rezultat, noviKoef, noviExp); // dodaj u rezultat
        }
    }
    return rezultat;
}

// funkcija za oslobađanje memorije
void oslobodi(Clan* p) {
    while (p) {
        Clan* temp = p;
        p = p->next;
        free(temp);
    }
}

int main() {
    FILE* fp = fopen("polinomi.txt", "r");      // otvaramo datoteku
    if (!fp) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    // čitanje dva polinoma
    Clan* p1 = ucitajPolinom(fp);
    Clan* p2 = ucitajPolinom(fp);
    fclose(fp);

    printf("Prvi polinom: ");
    ispisiPolinom(p1);

    printf("Drugi polinom: ");
    ispisiPolinom(p2);

    // računanje zbroja i umnoška
    Clan* zbrojPol = zbroji(p1, p2);
    Clan* umnozakPol = pomnozi(p1, p2);

    printf("\nZbroj: ");
    ispisiPolinom(zbrojPol);

    printf("Umnožak: ");
    ispisiPolinom(umnozakPol);

    // oslobađanje memorije
    oslobodi(p1);
    oslobodi(p2);
    oslobodi(zbrojPol);
    oslobodi(umnozakPol);

    return 0;
}
