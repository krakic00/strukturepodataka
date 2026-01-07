#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>    

// Struktura za direktorij
typedef struct Dir {
    char name[50];          // ime direktorija
    struct Dir* parent;     // roditelj
    struct Dir* child;      // prvo dijete
    struct Dir* sibling;    // sljedeći direktorij u istoj razini
} Dir;

// Funkcija kreira direktorij i vraća pokazivač na njega
Dir* createDirectory(char* name, Dir* parent) {
    Dir* d = (Dir*)malloc(sizeof(Dir));  // alokacija
    strcpy(d->name, name);               // kopiranje imena
    d->parent = parent;                  // postavljanje roditelja
    d->child = NULL;                     // nema djece
    d->sibling = NULL;                   // nema braće
    return d;                            // vraćamo direktorij
}

// Funkcija koja dodaje dijete i vraća 1 ako je dodano
int addDirectory(Dir* parent, char* name) {
    Dir* newDir = createDirectory(name, parent); // kreiramo novi

    if (parent->child == NULL) {          // ako je prvi
        parent->child = newDir;
        return 1;                         // uspjeh
    }

    Dir* curr = parent->child;            // inače prolazimo ostale
    while (curr->sibling != NULL) {
        curr = curr->sibling;
    }

    curr->sibling = newDir;               // dodajemo kao brata
    return 1;                             // uspjeh
}

// Funkcija traži dijete i ako postoji vrati ga, inače NULL
Dir* enterDirectory(Dir* parent, char* name) {
    Dir* curr = parent->child;            // prvo dijete
    while (curr != NULL) {                // prolazak
        if (strcmp(curr->name, name) == 0)
            return curr;                  // našli smo
        curr = curr->sibling;
    }
    return NULL;                          // ne postoji
}

// Funkcija za povratak gore, vraća roditelja
Dir* goBack(Dir* current) {
    if (current->parent == NULL)          // ako smo na vrhu
        return current;                   // ne možemo gore
    return current->parent;               // vraćamo roditelja
}

// Funkcija vraća broj direktorija i ispisuje ih
int listDirectories(Dir* current) {
    Dir* cur = current->child;
    int count = 0;

    printf("\nSadrzaj direktorija %s:\n", current->name);

    if (cur == NULL) {                    // ako je prazno
        printf("  <prazno>\n");
        return 0;
    }

    while (cur != NULL) {                 // ispis svih
        printf("  %s\n", cur->name);
        cur = cur->sibling;
        count++;                          // broji koliko ih ima
    }

    return count;                         // vraća broj
}

int main() {
    Dir* root = createDirectory("C:", NULL); // stvaramo root
    Dir* current = root;                      // trenutačni direktorij

    int choice;          // izbor izbornika
    char name[50];       // ime direktorija za unos

    do {
        printf("\n--- Izbornik ---\n");
        printf("1 - md (kreiraj direktorij)\n");
        printf("2 - cd dir (udji u direktorij)\n");
        printf("3 - cd.. (povratak)\n");
        printf("4 - dir (ispis sadrzaja)\n");
        printf("5 - izlaz\n");
        printf("Odabir: ");

        scanf("%d", &choice);            // učitavamo izbor

        if (choice == 1) {
            printf("Unesi ime direktorija: ");
            scanf("%s", name);
            addDirectory(current, name); // dodaj direktorij

        }
        else if (choice == 2) {
            printf("Unesi ime direktorija: ");
            scanf("%s", name);

            Dir* next = enterDirectory(current, name);
            if (next == NULL)
                printf("Direktorij ne postoji!\n");
            else
                current = next;

        }
        else if (choice == 3) {
            Dir* p = goBack(current);
            if (p == current)
                printf("Vec si u korijenu!\n");
            else
                current = p;

        }
        else if (choice == 4) {
            listDirectories(current);    // ispis

        }
        else if (choice == 5) {
            printf("Izlaz...\n");

        }
        else {
            printf("Pogresan odabir!\n");
        }

        printf("\nTrenutni direktorij: %s\n", current->name);

    } while (choice != 5);

    return 0;
}
