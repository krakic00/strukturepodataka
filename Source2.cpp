#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Osoba {
    char ime[50];
    char prezime[50];
    int godinaRodenja;
    struct _Osoba* next;  // pokazivač na sledeći element u listi
} Osoba;

// 🔸 Funkcija za unos nove osobe i dodavanje na početak liste
Osoba* dodajNaPocetak(Osoba* head) {
    // 1️⃣ Dinamički alociramo novi čvor
    Osoba* novaOsoba = (Osoba*)malloc(sizeof(Osoba));
    if (novaOsoba == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return head; // vraćamo stari početak jer se ništa nije promenilo
    }

    // 2️⃣ Unos podataka od korisnika
    printf("Unesite ime: ");
    scanf("%s", novaOsoba->ime);
    printf("Unesite prezime: ");
    scanf("%s", novaOsoba->prezime);
    printf("Unesite godinu rodenja: ");
    scanf("%d", &novaOsoba->godinaRodenja);

    // 3️⃣ Novi čvor pokazuje na prethodni prvi element
    novaOsoba->next = head;

    // 4️⃣ Novi čvor sada postaje početak liste
    return novaOsoba;
}

// 🔸 Pomoćna funkcija za ispis liste
void ispisiListu(Osoba* head) {
    Osoba* temp = head;
    while (temp != NULL) {
        printf("%s %s, %d\n", temp->ime, temp->prezime, temp->godinaRodenja);
        temp = temp->next;
    }
}

int main() {
    Osoba* head = NULL; // početak liste je prazan

    // Dodajemo nekoliko osoba na početak
    head = dodajNaPocetak(head);
    head = dodajNaPocetak(head);
    head = dodajNaPocetak(head);

    printf("\nTrenutna lista osoba:\n");
    ispisiListu(head);

    return 0;
}
