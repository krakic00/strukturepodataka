#define _CRT_SECURE_NO_WARNINGS          
#include <stdio.h>                      
#include <stdlib.h>                     
#include <string.h>                    

#define HASH_SIZE 11                    

// struktura za grad (čvor BST-a)
typedef struct Grad {
    char ime[50];                       // ime grada
    int stanovnici;                     // broj stanovnika
    struct Grad* left;                  // lijevo dijete
    struct Grad* right;                 // desno dijete
} Grad;

// struktura za državu (element liste u hash tablici)
typedef struct Drzava {
    char ime[50];                       // ime države
    Grad* gradovi;                     // korijen BST-a gradova
    struct Drzava* next;               // sljedeća država u listi
} Drzava;

// deklaracije funkcija
int hash(char* ime);
int usporediGradove(Grad* g1, Grad* g2);
Grad* insertGradBST(Grad* root, Grad* novi);
Grad* ucitajGradove(char* imeDat);
void pretraziGradove(Grad* root, int min);
void freeGradBST(Grad* root);

Drzava* insertDrzavaLista(Drzava* head, Drzava* nova);
void inicijalizirajHash(Drzava* hashTablica[]);
void ucitajDrzave(Drzava* hashTablica[], char* imeDatoteke);
void ispisiHash(Drzava* hashTablica[], int min);
void freeHash(Drzava* hashTablica[]);

// hash funkcija – zbraja ASCII vrijednosti prvih 5 slova
int hash(char* ime) {
    int suma = 0;                      // inicijalizacija sume

    for (int i = 0; i < 5 && ime[i] != '\0'; i++)
        suma += (int)ime[i];           // zbrajanje ASCII vrijednosti

    return suma % HASH_SIZE;           // indeks u hash tablici
}

// usporedba gradova: prvo po broju stanovnika, zatim po imenu
int usporediGradove(Grad* g1, Grad* g2) {
    if (g1->stanovnici != g2->stanovnici)
        return g1->stanovnici - g2->stanovnici;

    return strcmp(g1->ime, g2->ime);   // ako je isti broj stanovnika
}

// umetanje grada u BST
Grad* insertGradBST(Grad* root, Grad* novi) {
    if (root == NULL)
        return novi;                   // ako je stablo prazno

    if (usporediGradove(novi, root) < 0)
        root->left = insertGradBST(root->left, novi);   // ide lijevo
    else
        root->right = insertGradBST(root->right, novi); // ide desno

    return root;                       // vraća (nepromijenjeni) korijen
}

// učitavanje gradova iz datoteke i stvaranje BST-a
Grad* ucitajGradove(char* imeDat) {
    FILE* f = fopen(imeDat, "r");      // otvaranje datoteke
    if (!f)
        return NULL;                  // ako ne postoji datoteka

    Grad* root = NULL;                // početno prazno stablo
    char ime[50];
    int br;

    // čitanje linija oblika: ImeGrada, broj
    while (fscanf(f, " %[^,], %d", ime, &br) == 2) {
        Grad* g = (Grad*)malloc(sizeof(Grad));
        if (!g) break;                // provjera malloca

        strcpy(g->ime, ime);           // kopiranje imena
        g->stanovnici = br;            // broj stanovnika
        g->left = g->right = NULL;     // inicijalizacija djece

        root = insertGradBST(root, g); // umetanje u BST
    }

    fclose(f);                         // zatvaranje datoteke
    return root;                       // vraćanje korijena stabla
}

// ispis gradova koji imaju više stanovnika od min
void pretraziGradove(Grad* root, int min) {
    if (root == NULL)
        return;

    pretraziGradove(root->left, min);  // lijevo podstablo

    if (root->stanovnici > min)
        printf("   %s (%d)\n", root->ime, root->stanovnici);

    pretraziGradove(root->right, min); // desno podstablo
}

// oslobađanje memorije BST-a
void freeGradBST(Grad* root) {
    if (root == NULL)
        return;

    freeGradBST(root->left);
    freeGradBST(root->right);
    free(root);
}

// inicijalizacija hash tablice
void inicijalizirajHash(Drzava* hashTablica[]) {
    for (int i = 0; i < HASH_SIZE; i++)
        hashTablica[i] = NULL;
}

// umetanje države u sortiranu povezanu listu
Drzava* insertDrzavaLista(Drzava* head, Drzava* nova) {
    if (head == NULL || strcmp(nova->ime, head->ime) < 0) {
        nova->next = head;
        return nova;
    }

    Drzava* temp = head;
    while (temp->next != NULL && strcmp(nova->ime, temp->next->ime) > 0)
        temp = temp->next;

    nova->next = temp->next;
    temp->next = nova;

    return head;
}

// učitavanje država iz datoteke
void ucitajDrzave(Drzava* hashTablica[], char* imeDatoteke) {
    FILE* f = fopen(imeDatoteke, "r");
    if (!f) {
        printf("Greska pri otvaranju drzave.txt\n");
        return;
    }

    char imeDrzave[50];
    char datGradova[50];

    // svaka linija: Drzava datoteka_gradova
    while (fscanf(f, "%s %s", imeDrzave, datGradova) == 2) {
        Drzava* d = (Drzava*)malloc(sizeof(Drzava));
        if (!d) break;

        strcpy(d->ime, imeDrzave);
        d->gradovi = ucitajGradove(datGradova);
        d->next = NULL;

        int index = hash(imeDrzave);   // računanje hash indeksa
        hashTablica[index] = insertDrzavaLista(hashTablica[index], d);
    }

    fclose(f);
}

// ispis cijele hash tablice
void ispisiHash(Drzava* hashTablica[], int min) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Drzava* temp = hashTablica[i];

        if (temp != NULL)
            printf("\nHASH [%d]\n", i);

        while (temp != NULL) {
            printf("Drzava: %s\n", temp->ime);
            pretraziGradove(temp->gradovi, min);
            temp = temp->next;
        }
    }
}

// oslobađanje cijele hash tablice
void freeHash(Drzava* hashTablica[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        while (hashTablica[i] != NULL) {
            Drzava* temp = hashTablica[i];
            hashTablica[i] = hashTablica[i]->next;

            freeGradBST(temp->gradovi);
            free(temp);
        }
    }
}

int main() {
    Drzava* hashTablica[HASH_SIZE];     // hash tablica država
    int min;                            // minimalan broj stanovnika

    inicijalizirajHash(hashTablica);    // inicijalizacija
    ucitajDrzave(hashTablica, "drzave.txt");

    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &min);

    ispisiHash(hashTablica, min);       // ispis rezultata
    freeHash(hashTablica);              // oslobađanje memorije

    return 0;
}
