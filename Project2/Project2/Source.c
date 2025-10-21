#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struktura za osobu
typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godinaRodenja;
    struct Osoba* next;  // pokazivač na sljedeću osobu u listi
} Osoba;

// funkcija za stvaranje nove osobe
Osoba* kreirajOsobu(const char* ime, const char* prezime, int godina) {
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
    if (!nova) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    strcpy(nova->ime, ime);
    strcpy(nova->prezime, prezime);
    nova->godinaRodenja = godina;
    nova->next = NULL;
    return nova;
}

// A. dodavanje nove osobe na početak liste
void dodajNaPocetak(Osoba** head, const char* ime, const char* prezime, int godina) {
    Osoba* nova = kreirajOsobu(ime, prezime, godina);
    if (nova == NULL) return;
    nova->next = *head;  // nova pokazuje na staru glavu
    *head = nova;        // nova postaje nova glava liste
}

// B. ispis cijele liste
void ispisiListu(Osoba* head) {
    Osoba* temp = head;
    if (temp == NULL) {
        printf("Lista je prazna!\n");
        return;
    }
    printf("\n%-15s %-15s %-15s\n", "Ime", "Prezime", "Godina rodenja");
    printf("---------------------------------------------\n");
    while (temp != NULL) {
        printf("%-15s %-15s %-15d\n", temp->ime, temp->prezime, temp->godinaRodenja);
        temp = temp->next;
    }
}

// C. dodavanje nove osobe na kraj liste
void dodajNaKraj(Osoba** head, const char* ime, const char* prezime, int godina) {
    Osoba* nova = kreirajOsobu(ime, prezime, godina);
    if (nova == NULL) return;

    if (*head == NULL) {   // ako je lista prazna
        *head = nova;
        return;
    }

    Osoba* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = nova;  // zadnji element pokazuje na novu osobu
}

// D. pronalazak osobe po prezimenu
Osoba* pronadiOsobu(Osoba* head, const char* prezime) {
    Osoba* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->prezime, prezime) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL; // ako nije pronađena
}

// E. brisanje osobe iz liste po prezimenu
void obrisiOsobu(Osoba** head, const char* prezime) {
    Osoba* temp = *head;
    Osoba* prethodna = NULL;

    while (temp != NULL && strcmp(temp->prezime, prezime) != 0) {
        prethodna = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", prezime);
        return;
    }

    if (prethodna == NULL) {
        // brišem prvi element
        *head = temp->next;
    }
    else {
        prethodna->next = temp->next;
    }

    free(temp);
    printf("Osoba '%s' uspjesno obrisana.\n", prezime);
}

int main() {
    Osoba* head = NULL; // pokazivač na početak liste

    // testni unos 
    dodajNaPocetak(&head, "Ana", "Anić", 1998);
    dodajNaPocetak(&head, "Marko", "Marković", 2000);
    dodajNaKraj(&head, "Iva", "Ivić", 1997);

    printf("Lista nakon dodavanja:\n");
    ispisiListu(head);

    // pronalazak osobe
    Osoba* trazena = pronadiOsobu(head, "Anić");
    if (trazena != NULL)
        printf("\nPronadena osoba: %s %s, %d\n", trazena->ime, trazena->prezime, trazena->godinaRodenja);
    else
        printf("\nOsoba nije pronadena.\n");

    // brisanje osobe
    obrisiOsobu(&head, "Marković");

    printf("\nLista nakon brisanja:\n");
    ispisiListu(head);

    // oslobađanje cijele liste na kraju
    while (head != NULL) {
        Osoba* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
