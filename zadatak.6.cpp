#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za jedan artikl na računu
typedef struct Artikl {
    char naziv[100];
    int kolicina;
    float cijena;
    struct Artikl* next;   // pokazivač na sljedeći artikl (povezana lista)
} Artikl;

// Struktura za jedan račun
typedef struct Racun {
    char datum[11];        // datum računa
    Artikl* artikli;       // početak liste artikala
    struct Racun* next;    // sljedeći račun u listi
} Racun;

// Funkcija umeće novi artikl u sortiranu listu artikala (sortirano po nazivu)
Artikl* umetniArtiklSortirano(Artikl* head, Artikl* novi) {
    // Ako je lista prazna ili novi artikl ide na početak
    if (!head || strcmp(novi->naziv, head->naziv) < 0) {
        novi->next = head;
        return novi;
    }

    // Inače prolazimo listu i tražimo mjesto gdje umetnuti artikl
    Artikl* p = head;
    while (p->next && strcmp(novi->naziv, p->next->naziv) > 0)
        p = p->next;

    // Umetanje artikla
    novi->next = p->next;
    p->next = novi;
    return head;
}

// Funkcija umeće novi račun u sortiranu listu računa (sortirano po datumu)
Racun* umetniRacunSortirano(Racun* head, Racun* novi) {
    if (!head || strcmp(novi->datum, head->datum) < 0) {
        novi->next = head;
        return novi;
    }

    Racun* p = head;
    while (p->next && strcmp(novi->datum, p->next->datum) > 0)
        p = p->next;

    novi->next = p->next;
    p->next = novi;
    return head;
}

// Funkcija učitava jedan račun iz datoteke
Racun* ucitajRacun(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    // Alokacija novog računa
    Racun* r = malloc(sizeof(Racun));
    r->artikli = NULL;
    r->next = NULL;

    // Čitanje datuma
    fgets(r->datum, 11, f);
    fgetc(f); // čitanje znaka \n

    // Varijable za čitanje artikala
    char naziv[100];
    int kol;
    float cij;

    // Čitanje artikala dok god format odgovara
    while (fscanf(f, "%99[^,], %d, %f\n", naziv, &kol, &cij) == 3) {
        Artikl* a = malloc(sizeof(Artikl));
        strcpy(a->naziv, naziv);
        a->kolicina = kol;
        a->cijena = cij;
        a->next = NULL;

        // Umetanje artikla u sortiranu listu artikala
        r->artikli = umetniArtiklSortirano(r->artikli, a);
    }

    fclose(f);
    return r;
}

// Funkcija učitava sve račune čija se imena nalaze u popisnoj datoteci
Racun* ucitajSveRacune(const char* popis) {
    FILE* f = fopen(popis, "r");
    if (!f) return NULL;

    Racun* lista = NULL;
    char nazivDatoteke[100];

    // Svaka linija popisa predstavlja naziv jedne datoteke s računom
    while (fgets(nazivDatoteke, 100, f)) {
        nazivDatoteke[strcspn(nazivDatoteke, "\n")] = '\0'; // ukloni \n

        Racun* r = ucitajRacun(nazivDatoteke);
        if (r)
            lista = umetniRacunSortirano(lista, r);
    }

    fclose(f);
    return lista;
}

// Funkcija ispisuje koliko je jedan artikl kupljen i koliko je potrošeno
// u zadanom rasponu datuma
void upit(Racun* lista, const char* artikl, const char* od, const char* do_) {
    float ukupno = 0;
    int kolicina = 0;

    // Prolaz kroz listu računa
    for (Racun* r = lista; r; r = r->next) {

        // Provjera je li račun unutar zadanog raspona datuma
        if (strcmp(r->datum, od) >= 0 && strcmp(r->datum, do_) <= 0) {

            // Pretraživanje artikala u računu
            Artikl* a = r->artikli;
            while (a) {
                if (strcmp(a->naziv, artikl) == 0) {
                    ukupno += a->cijena * a->kolicina;
                    kolicina += a->kolicina;
                }
                a = a->next;
            }
        }
    }

    printf("Ukupno potroseno: %.2f, ukupno kupljeno: %d\n", ukupno, kolicina);
}

int main() {
    // Učitavanje svih računa iz popisne datoteke
    Racun* racuni = ucitajSveRacune("racuni.txt");

    char trazeni[100], d1[11], d2[11];

    // Unos korisnika
    printf("Artikl: "); scanf("%99s", trazeni);
    printf("Od datuma (YYYY-MM-DD): "); scanf("%10s", d1);
    printf("Do datuma (YYYY-MM-DD): "); scanf("%10s", d2);

    // Pokretanje upita
    upit(racuni, trazeni, d1, d2);
    return 0;
}
