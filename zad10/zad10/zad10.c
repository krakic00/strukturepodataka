#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Grad {
    char ime[50];
    int stanovnici;

    struct Grad* left;   // za stablo
    struct Grad* right;  // za stablo
    struct Grad* next;   // za listu
} Grad;


typedef struct Drzava {
    char ime[50];
    Grad* gradovi;          // stablo gradova
    struct Drzava* next;
} Drzava;

typedef struct DrzavaBST {
    char ime[50];
    Grad* gradovi;          // lista gradova
    struct DrzavaBST* left;
    struct DrzavaBST* right;
} DrzavaBST;

//uspoređujemo gradove prvo po stanovnicima pa po imenima
int usporediGradove(Grad* g1, Grad* g2) {
    if (g1->stanovnici != g2->stanovnici)
        return g1->stanovnici - g2->stanovnici;

    return strcmp(g1->ime, g2->ime);
}

//umećen gradove u stablo
Grad* insertGradBST(Grad* root, Grad* novi) {
    if (root == NULL)
        return novi;

    if (usporediGradove(novi, root) < 0)
        root->left = insertGradBST(root->left, novi);
    else
        root->right = insertGradBST(root->right, novi);

    return root;
}

//umećen države u sortiranu listu
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

//stavljanje gradova u stablo
Grad* ucitajGradoveBST(char* imeDat) {
    FILE* f = fopen(imeDat, "r");
    if (!f) return NULL;

    Grad* root = NULL;
    char ime[50];
    int br;

    while (fscanf(f, " %[^,],%d", ime, &br) == 2) {
        Grad* g = (Grad*)malloc(sizeof(Grad));
        strcpy(g->ime, ime);
        g->stanovnici = br;
        g->left = g->right = g->next = NULL;

        root = insertGradBST(root, g);
    }

    fclose(f);
    return root;
}


Grad* insertGradLista(Grad* head, Grad* novi) {
    if (head == NULL || usporediGradove(novi, head) < 0) {
        novi->next = head;
        return novi;
    }

    Grad* temp = head;
    while (temp->next != NULL && usporediGradove(novi, temp->next) > 0)
        temp = temp->next;

    novi->next = temp->next;
    temp->next = novi;

    return head;
}
//umećen gradove
DrzavaBST* insertDrzavaBST(DrzavaBST* root, DrzavaBST* nova) {
    if (root == NULL)
        return nova;

    if (strcmp(nova->ime, root->ime) < 0)
        root->left = insertDrzavaBST(root->left, nova);
    else
        root->right = insertDrzavaBST(root->right, nova);

    return root;
}

//učitavan gradove u listu
Grad* ucitajGradoveLista(char* imeDat) {
    FILE* f = fopen(imeDat, "r");
    if (!f) return NULL;

    Grad* head = NULL;
    char ime[50];
    int br;

    while (fscanf(f, " %[^,],%d", ime, &br) == 2) {
        Grad* g = (Grad*)malloc(sizeof(Grad));
        strcpy(g->ime, ime);
        g->stanovnici = br;
        g->left = g->right = g->next = NULL;

        head = insertGradLista(head, g);
    }

    fclose(f);
    return head;
}

//pretražujen gradove 
void pretragaGradovaBST(Grad* root, int min) {
    if (root == NULL)
        return;

    pretragaGradovaBST(root->left, min);

    if (root->stanovnici > min)
        printf("   %s (%d)\n", root->ime, root->stanovnici);

    pretragaGradovaBST(root->right, min);
}


void ispisListaDrzava(Drzava* head, int min) {
    while (head != NULL) {
        printf("\nDrzava: %s\n", head->ime);
        pretragaGradovaBST(head->gradovi, min);
        head = head->next;
    }
}


   
int main() {
    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    Drzava* listaDrzava = NULL;
    DrzavaBST* stabloDrzava = NULL;

    char imeDrzave[50];
    char datGradova[50];

    while (fscanf(f, "%s %s", imeDrzave, datGradova) == 2) {

        /* dio A */
        Drzava* d = (Drzava*)malloc(sizeof(Drzava));
        strcpy(d->ime, imeDrzave);
        d->gradovi = ucitajGradoveBST(datGradova);
        d->next = NULL;
        listaDrzava = insertDrzavaLista(listaDrzava, d);

        /* dio B */
        DrzavaBST* db = (DrzavaBST*)malloc(sizeof(DrzavaBST));
        strcpy(db->ime, imeDrzave);
        db->gradovi = ucitajGradoveLista(datGradova);
        db->left = db->right = NULL;
        stabloDrzava = insertDrzavaBST(stabloDrzava, db);
    }

    fclose(f);

    int min;
    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &min);

    ispisListaDrzava(listaDrzava, min);

    return 0;
}
