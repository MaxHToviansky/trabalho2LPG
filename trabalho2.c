#include <stdio.h>
#include <stdlib.h>

// FILE *f = fopen("yourfile.txt", "wt"); <- yourfile tem de estar na mesma pasta.
// fprintf(f, "varortext%d", val); <- escreve no arquivo f.
// fclose(f); <- encerra e registra o arquivo no disco, sem ele o arquivo pode voltar em branco.
// w -> write, t -> text, r -> read

struct Data
{
    int date[3];
};

struct Horario
{
    int time[2];
};

struct Evento
{
    struct Data dia;
    struct Horario inicio;
    struct Horario fim;
    char descricao[50], local[50];
};

int cmpData(struct Data *a, struct Data *b)
{
    // LHS (Left Hand Side), RHS (Right Hand Side)
    // LHS > RHS -> 1
    // LHS < RHS -> -1
    // LHS == RHS -> 0
    for (int i = 2; i >= 0; i--)
    {
        if (a->date[i] > b->date[i])
        {
            return 1;
        }
        else if (a->date[i] < b->date[i])
        {
            return -1;
        }
    }
    return 0;
}

int cmpHora(struct Horario *a, struct Horario *b)
{
    for (int i = 0; i < 2; i++)
    {
        if (a->time[i] > b->time[i])
        {
            return 1;
        }
        else if (a->time[i] < b->time[i])
        {
            return -1;
        }
    }
    return 0;
}

int cmpEvento(struct Evento *a, struct Evento *b)
{
    // perguntar rui, por enquanto &() basta
    int dateRes = cmpData(&a->dia, &b->dia);

    if (dateRes != 0)
        return dateRes;

    return cmpHora(&(a->inicio), &(b->inicio));
}

int shift(struct Evento *list, int pos, int n)
{
    for (int i = n - 2; i >= pos; i--)
        list[i + 1] = list[i];
}

int insEvent(struct Evento *list, struct Evento val, int n)
{
    int l = 0, r = n - 1;
    if (n > 0)
    {
        while (l <= r)
        {
            int mid = (l + r) / 2;
            int cmp = cmpEvento(&(list[mid]), &val);
            if (cmp == 1)
            {
                l = mid + 1;
            }
            else if (cmp == -1)
            {
                r = mid;
            }
            else
            {
                return 1; // 1 -> conflito inicio
            }
        }
        // l é a posição do novo evento

        if (cmpHora(&list[l - 1].fim, &list[l].inicio) == 1)
        {
            return 2; // 2 -> conflito interseção
        }
    }
    realloc(list, n + 1);
    shift(list, l, n + 1);
    list[l] = val;
    return 0; // inserção feita com sucesso
}

void showEventos(struct Evento *list, int n)
{
    for (int i = 0; i < n; i++)
    {
        int *v = list->dia.date;
        int *d = list->inicio.time;
        int *e = list->fim.time;
        printf("Data:\n");
        printf("%d/%d/%d (DD/MM/YYYY)\n", v[0], v[1], v[2]);
        printf("Hora De Inicio:\n");
        printf("%d:%d\n", d[0], d[1]);
        printf("Hora Do Fim:\n");
        printf("%d:%d\n", e[0], e[1]);
        printf("Descrição:\n");
        printf("%s", list->descricao);
        printf("Local:\n");
        printf("%s", list->local);
    }
}

int readDate(struct Data *dia){
    int a,b,c;
    scanf("%d %d %d",&a,&b,&c);
    if(a>9999 || b > 12 || a > 31){
        printf("Data invalida!\n");
        return 1;
    }
    dia->date[0] = a;
    dia->date[1] = b;
    dia->date[2] = c;
    return 0;
}

int readTime(struct Horario *hora){
    int a,b;
    scanf("%d %d",&a,&b);
    if(a>24 || b>59){
        printf("Horario invalido!\n");
        return 1;
    }
    hora->time[0] = a;
    hora->time[1] = b;
    return 0;
}

int generateEvent(struct Evento *input){
    printf("Informe a data de inicio do evento.(DD/MM/AAAA)\n");
    if(readDate(&input->dia)){
        // cleanup(); TODO
        return 1;
    }
    printf("Informe o horario do inicio do evento. (HH:MM)\n");
    if(readTime(&input->inicio)){
        // cleanup(); TODO
        return 1;
    }
    printf("Informe o horario do fim do evento. (HH:MM)\n");
    if(readTime(&input->fim)){
        // cleanup(); TODO
        return 1;
    }
    // comparação inicio fim TODO
    printf("Informe a descrição do evento.\n");
    fgets(&input->descricao,49,stdin);
    printf("Informe o Local do evento.\n");
    fgets(&input->local,49,stdin);
}

int main()
{
    struct Evento *agenda;
    int n = 0;
    // Menu
    int run = 1;
    char status;
    while (run)
    {
        scanf("%c", &status);
        switch (status)
        {
        case 'i':
            struct Evento val;
            generateEvent(&val);
            break;
        case 'e':
            run = 0;
            break;
        default:
            break;
        }
    }
    return 0;
}