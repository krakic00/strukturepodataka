#include <stdio.h>
#include <stdlib.h>

#define MAX 100  // maksimalan broj članova u polinomu

// struktura koja opisuje jedan član polinoma
typedef struct {
    int koef;  // koeficijent člana
    int exp;   // eksponent člana
} Clan;

// funkcija za čitanje polinoma iz datoteke
int ucitajPolinom(FILE* fp, Clan polinom[]) {
    int n = 0;  // broj učitanih članova
    while (fscanf(fp, "%d %d", &polinom[n].koef, &polinom[n].exp) == 2) {
        n++;  // povećavamo broj članova
        // prekid ako dođemo do kraja reda
        if (fgetc(fp) == '\n' || feof(fp)) break;
    }
    return n;  // vraćamo koliko smo članova učitali
}

// funkcija za ispis polinoma
void ispisiPolinom(Clan p[], int n) {
    for (int i = 0; i < n; i++) {
        if (p[i].koef == 0) continue; // preskačemo nule
        if (i > 0 && p[i].koef > 0) printf("+"); // znak plus ako treba
        printf("%dx^%d", p[i].koef, p[i].exp); // ispis jednog člana
    }
    printf("\n");
}

// funkcija za sortiranje članova prema eksponentima (silazno)
void sortiraj(Clan p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].exp < p[j].exp) { // ako je eksponent manji, zamijeni
                Clan temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

// funkcija za zbrajanje dvaju polinoma
int zbroji(Clan p1[], int n1, Clan p2[], int n2, Clan rez[]) {
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (p1[i].exp == p2[j].exp) { // isti eksponent – zbroji koeficijente
            rez[k].exp = p1[i].exp;
            rez[k].koef = p1[i].koef + p2[j].koef;
            i++; j++; k++;
        }
        else if (p1[i].exp > p2[j].exp) { // veći eksponent ide prvi
            rez[k++] = p1[i++];
        }
        else {
            rez[k++] = p2[j++];
        }
    }
    // preostali članovi ako ih ima
    while (i < n1) rez[k++] = p1[i++];
    while (j < n2) rez[k++] = p2[j++];
    return k; // vraćamo broj članova rezultata
}

// funkcija za množenje dvaju polinoma
int pomnozi(Clan p1[], int n1, Clan p2[], int n2, Clan rez[]) {
    int k = 0;
    // inicijalno postavimo sve na 0
    for (int i = 0; i < MAX; i++) {
        rez[i].koef = 0;
        rez[i].exp = 0;
    }
    // množimo svaki član prvog sa svakim članom drugog
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int noviExp = p1[i].exp + p2[j].exp; // zbrajamo eksponente
            int noviKoef = p1[i].koef * p2[j].koef; // množimo koeficijente
            // tražimo postoji li već taj eksponent u rezultatu
            int postoji = 0;
            for (int t = 0; t < k; t++) {
                if (rez[t].exp == noviExp) {
                    rez[t].koef += noviKoef; // zbrojimo koeficijente
                    postoji = 1;
                    break;
                }
            }
            if (!postoji) { // ako ne postoji, dodamo novi član
                rez[k].exp = noviExp;
                rez[k].koef = noviKoef;
                k++;
            }
        }
    }
    sortiraj(rez, k); // sortiramo rezultat po eksponentima
    return k;
}

int main() {
    FILE* fp = fopen("polinomi.txt", "r"); // otvaramo datoteku
    if (!fp) {
        printf("Greška pri otvaranju datoteke!\n");
        return 1;
    }

    Clan p1[MAX], p2[MAX], zbroj[MAX], umnozak[MAX];
    int n1, n2, nz, nm;

    n1 = ucitajPolinom(fp, p1);  // čitamo prvi polinom
    n2 = ucitajPolinom(fp, p2);  // čitamo drugi polinom
    fclose(fp); // zatvaramo datoteku

    sortiraj(p1, n1); // sortiramo prvi polinom
    sortiraj(p2, n2); // sortiramo drugi polinom

    printf("Prvi polinom: ");
    ispisiPolinom(p1, n1);
    printf("Drugi polinom: ");
    ispisiPolinom(p2, n2);

    nz = zbroji(p1, n1, p2, n2, zbroj);  // zbrajamo
    nm = pomnozi(p1, n1, p2, n2, umnozak); // množimo

    printf("\nZbroj: ");
    ispisiPolinom(zbroj, nz);

    printf("Umnožak: ");
    ispisiPolinom(umnozak, nm);

    return 0;
}
