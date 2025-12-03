#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikl {
    char naziv[100];
    int kolicina;
    float cijena;
    struct Artikl* next;
} Artikl;

typedef struct Racun {
    char datum[11];
    Artikl* artikli;
    struct Racun* next;
} Racun;

Artikl* umetniArtiklSortirano(Artikl* head, Artikl* novi) {
    if (!head || strcmp(novi->naziv, head->naziv) < 0) {
        novi->next = head;
        return novi;
    }
    Artikl* p = head;
    while (p->next && strcmp(novi->naziv, p->next->naziv) > 0)
        p = p->next;
    novi->next = p->next;
    p->next = novi;
    return head;
}

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

Racun* ucitajRacun(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    Racun* r = malloc(sizeof(Racun));
    r->artikli = NULL;
    r->next = NULL;

    fgets(r->datum, 11, f);
    fgetc(f);

    char naziv[100];
    int kol;
    float cij;

    while (fscanf(f, "%99[^,], %d, %f\n", naziv, &kol, &cij) == 3) {
        Artikl* a = malloc(sizeof(Artikl));
        strcpy(a->naziv, naziv);
        a->kolicina = kol;
        a->cijena = cij;
        a->next = NULL;
        r->artikli = umetniArtiklSortirano(r->artikli, a);
    }

    fclose(f);
    return r;
}

Racun* ucitajSveRacune(const char* popis) {
    FILE* f = fopen(popis, "r");
    if (!f) return NULL;

    Racun* lista = NULL;
    char nazivDatoteke[100];

    while (fgets(nazivDatoteke, 100, f)) {
        nazivDatoteke[strcspn(nazivDatoteke, "\n")] = '\0';
        Racun* r = ucitajRacun(nazivDatoteke);
        if (r) lista = umetniRacunSortirano(lista, r);
    }

    fclose(f);
    return lista;
}

void upit(Racun* lista, const char* artikl, const char* od, const char* do_) {
    float ukupno = 0;
    int kolicina = 0;

    for (Racun* r = lista; r; r = r->next) {
        if (strcmp(r->datum, od) >= 0 && strcmp(r->datum, do_) <= 0) {
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
    Racun* racuni = ucitajSveRacune("racuni.txt");

    char trazeni[100], d1[11], d2[11];
    printf("Artikl: "); scanf("%99s", trazeni);
    printf("Od datuma (YYYY-MM-DD): "); scanf("%10s", d1);
    printf("Do datuma (YYYY-MM-DD): "); scanf("%10s", d2);

    upit(racuni, trazeni, d1, d2);
    return 0;
}
