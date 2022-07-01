#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE "USUARIO.txt"
#define TEMPFILE "temp.txt"
#define SZ_ID 4
struct usuario {
    char nome[40];
    char id[4];
} classe_usuario;

void create();
void read(int i);
void update();
void delete ();
void showAll();
void opcao();
void fillWithZeros(char *stringToFill, int lenOfString, int totalLen);
void idFormat(int id, char *idTemp);
void saveData(FILE *file, struct usuario user);
void updateDatabase();
FILE *openFile(char *file, char *op);
int getIndex();

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
    idFormat(id, idTemp);
    printf("Digite o nome do usuário: \n");
    getchar();  // Utilizando getchar() para consumir o \n que estiver no buffer
    fgets(nome, 30, stdin);
    sprintf(classe_usuario.id, "%s", idTemp);
    sprintf(classe_usuario.nome, "%s", nome);
    saveData(file, classe_usuario);
    printf("Usuário criado com sucesso!!\n\n");
    free(idTemp);
    fclose(file);
}

void read(int id) {
    FILE *file   = openFile(DATABASE, "r");
    char *idTemp = malloc(SZ_ID);
    char *data   = malloc(40 * sizeof(char));
    idFormat(id, idTemp);
    while (fgets(data, 40, file)) {
        if (strstr(data, idTemp)) {
            printf("\nUsuário encontrado: \n%s\n", data);
            free(idTemp);
            free(data);
            return;
        }
    }
    free(idTemp);
    free(data);
    printf("Usuário não encontrado!!\n");
}
void update(int id) {
    FILE *file = openFile(DATABASE, "r");
    FILE *filetmp = openFile(TEMPFILE, "w");
    char *idTemp = malloc(SZ_ID * sizeof(char));
    char *nome = malloc(40 * sizeof(char));
    idFormat(id, idTemp);
    char string[40];
    while (fgets(string, 40, file)) {
        if (strstr(string, idTemp)) {
            printf("Digite o nome do usuário: \n");
            getchar();  // Consome o \n, caso esteja no buffer, para que a entrada de teclado funcione.
            fgets(nome, 40, stdin);
            sprintf(string, "%s;%s", idTemp, nome);
            printf("Usuário Atualizado com sucesso!!\n\n");
        }
        fputs(string, filetmp);
    }
    fclose(filetmp);
    fclose(file);
    free(idTemp);
    free(nome);
    updateDatabase();
}
void delete (int id) {
    FILE *file = openFile(DATABASE, "r");
    FILE *filetmp = openFile(TEMPFILE, "w");
    char *idTemp = malloc(SZ_ID * sizeof(char));
    char *nome = malloc(40 * sizeof(char));
    idFormat(id, idTemp);
    while (fgets(nome, 40, file)) {
        if (strstr(nome, idTemp)) {
            *nome = '\0';
            printf("Usuário Deletado com sucesso!!\n\n");
        }
        fputs(nome, filetmp);
    }

    fclose(filetmp);
    fclose(file);
    free(idTemp);
    free(nome);
    updateDatabase();
}
void showAll() {
    FILE *file = openFile(DATABASE, "r");
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
    }
    fclose(file);
}

void updateDatabase() {
    FILE *file = openFile(DATABASE, "w+");
    FILE *filetmp = openFile(TEMPFILE, "r");
    char string[40];

    while (fgets(string, 40, filetmp)) {
        fputs(string, file);
    }
    fclose(filetmp);
    fclose(file);
    remove(TEMPFILE);
}

void idFormat(int id, char *idStr) {
    sprintf(idStr, "%d", id);
    fillWithZeros(idStr, strlen(idStr), (SZ_ID - 1));
}

void saveData(FILE *file, struct usuario user) {
    fprintf(file, "%s;%s", user.id, user.nome);
};

void fillWithZeros(char *stringToFill, int lenOfString, int totalLen) {
    int count;
    for (count = lenOfString; count >= 0; count--) {
        *(stringToFill + (count + (totalLen - lenOfString))) = *(stringToFill + count);
    }
    for (count = 0; count < totalLen - lenOfString; count++) {
        *(stringToFill + count) = '0';
    }
}