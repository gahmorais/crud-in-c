#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create();
void read(int i);
void update();
void delete ();
void showAll();
void opcao();
void fillWithZeros(char *stringToFill, int lenOfString, int totalLen);
FILE *openFile(char *file, char *op);
int getIndex();

#define DATABASE "USUARIO.TXT"
#define SZ_ID 4

int main() {
    int opt;
    while (opt != 7) {
        system("clear");
        printf("************Bem vindo ao sistema de cadastro de usuários!************\n");
        printf("**********************Digite a opção desejada!***********************\n\n");
        printf("1 - criar usuário\n");
        printf("2 - Buscar usuário\n");
        printf("3 - Atualizar usuário\n");
        printf("4 - Deletar usuário\n");
        printf("5 - Mostrar todos os dados\n");
        printf("6 - Sair\n");
        scanf("%d", &opt);
        opcao(opt);
    }

    return 0;
}
void opcao(int opt) {
    int id;
    putchar('\n');
    switch (opt) {
        case 1:
            printf("Criar usuário\n");
            create();
            break;
        case 2:
            printf("Buscar usuário\n");
            printf("Digite o ID do usuário\n");
            scanf("%d", &id);
            read(id);
            break;
        case 3:
            printf("Atualizar usuário\n");
            printf("Digite o ID do usuário\n");
            scanf("%d", &id);
            update(id);
            break;
        case 4:
            printf("Deletar usuário\n");
            printf("Digite o ID do usuário\n");
            scanf("%d", &id);
            delete (id);
            break;
        case 5:
            printf("Mostrando todos os dados\n");
            showAll();
            break;

        case 6:
            printf("Encerrando programa!\n\n");
            exit(0);
            break;
    }
    system("read -p \"Pressione enter para continuar\" saindo");
}

FILE *openFile(char *f, char *op) {
    FILE *file = fopen(f, op);
    if (!file) {
        printf("Erro ao abrir o arquivo!");
        exit(0);
    }

    return file;
}

int getIndex() {
    FILE *file = openFile(DATABASE, "r");
    int index = -2;
    int ch;
    char id[SZ_ID];
    fseek(file, 0, SEEK_END);
    if (ftell(file)) {
        while ((ch = fgetc(file)) != '\n') {
            fseek(file, index, SEEK_END);
            index--;
            if (!ftell(file)) {
                break;
            }
        }
        int i = 0;
        while ((ch = fgetc(file)) != ';') {
            id[i] = ch;
            i++;
        }
    } else {
        sprintf(id, "%s", "000");
    }

    return atoi(id);
}

void create() {
    FILE *file = openFile(DATABASE, "a+");
    int id = getIndex() + 1;
    char *idTemp = (char *)malloc(SZ_ID);
    char *nome = (char *)malloc(40 * sizeof(char));
    sprintf(idTemp, "%d", id);                           // Converte o inteiro para string;
    fillWithZeros(idTemp, strlen(idTemp), (SZ_ID - 1));  // Preenche o id com zeros a esquerda
    printf("Digite o nome do usuário: \n");
    getchar();  // Utilizando getchar() para consumir o \n que fiz no buffer
    fgets(nome, 30, stdin);
    fprintf(file, "%s;%s", idTemp, nome);
    printf("Usuário criado com sucesso!!\n\n");
    free(idTemp);
    fclose(file);
}

void read(int id) {
    FILE *file = openFile(DATABASE, "r");
    char *idTemp = malloc(SZ_ID);
    sprintf(idTemp, "%d", id);
    fillWithZeros(idTemp, strlen(idTemp), (SZ_ID - 1));
    int ch;
    char string[40];
    while (fgets(string, 40, file) != NULL) {
        if (strstr(string, idTemp)) {
            printf("\nUsuário encontrado: \n%s\n", string);
            return;
        }
    }
    free(idTemp);
    printf("Usuário não encontrado!!\n");
}

void update(int id) {
    char *tempFile = "TEMP.txt";
    FILE *file = openFile(DATABASE, "r");
    FILE *filetmp = openFile(tempFile, "w");
    char *idTemp = malloc(SZ_ID * sizeof(char));
    char *nome = malloc(40 * sizeof(char));
    sprintf(idTemp, "%d", id);
    fillWithZeros(idTemp, strlen(idTemp), (SZ_ID - 1));
    int ch;
    char string[40];
    while (fgets(string, 40, file) != NULL) {
        if (strstr(string, idTemp)) {
            printf("Digite o nome do usuário: \n");
            getchar(); //Consome o \n, caso esteja no buffer, para que a entrada de teclado funcione.
            fgets(nome, 40, stdin);
            sprintf(string, "%s;%s", idTemp, nome);
        }
        fprintf(filetmp, "%s", string);
    }
    fclose(filetmp);
    fclose(file);
    file = openFile(DATABASE, "w+");
    filetmp = openFile(tempFile, "r");

    while (fgets(string, 40, filetmp) != NULL) {
        fputs(string, file);
    }

    remove(tempFile);
    free(idTemp);
    free(nome);
    fclose(file);
    printf("Usuário Atualizado com sucesso!!\n\n");
}

void delete (int id) {
    int ch;
    char string[40];

    char *tempFile = "TEMP.txt";
    FILE *file = openFile(DATABASE, "r");
    FILE *filetmp = openFile(tempFile, "w");
    char *idTemp = malloc(SZ_ID * sizeof(char));

    sprintf(idTemp, "%d", id);
    fillWithZeros(idTemp, strlen(idTemp), (SZ_ID - 1));

    while (fgets(string, 40, file) != NULL) {
        if (strstr(string, idTemp)) {
            sprintf(string, "%s", "");
        }
        fprintf(filetmp, "%s", string);
    }

    fclose(filetmp);
    fclose(file);
    file = openFile(DATABASE, "w+");
    filetmp = openFile(tempFile, "r");

    while (fgets(string, 40, filetmp) != NULL) {
        fputs(string, file);
    }

    printf("Usuário Deletado com sucesso!!\n\n");
    remove(tempFile);
    free(idTemp);
    fclose(file);
}

void showAll() {
    FILE *file = openFile(DATABASE, "r");
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
    }
    fclose(file);
}

void fillWithZeros(char *stringToFill, int lenOfString, int totalLen) {
    int count;
    for (count = lenOfString; count >= 0; count--) {
        *(stringToFill + (count + (totalLen - lenOfString))) = *(stringToFill + count);
    }
    for (count = 0; count < totalLen - lenOfString; count++) {
        *(stringToFill + count) = '0';
    }
}