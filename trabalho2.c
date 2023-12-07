#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FILE *f = fopen("yourfile.txt", "wt"); <- yourfile tem de estar na mesma pasta.
// fprintf(f, "varortext%d", val); <- escreve no arquivo f.
// fclose(f); <- encerra e registra o arquivo no disco, sem ele o arquivo pode voltar em branco.
// w -> write, t -> text, r -> read
// Eu adoraria fazer um binary search polimorfico...

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

// LHS (Left Hand Side), RHS (Right Hand Side)
// LHS > RHS -> 1
// LHS < RHS -> -1
// LHS == RHS -> 0
int cmpData(struct Data *a, struct Data *b)
{
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

// LHS > RHS -> 1
// LHS < RHS -> -1
// LHS == RHS -> 0
int cmpEvento(struct Evento *a, struct Evento *b)
{
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

int insEvent(struct Evento **list, struct Evento val, int n)
{
    int l = 0, r = n - 1;
    if (n > 0)
    {
        while (l <= r)
        {
            int mid = (l + r) / 2;
            int comp = cmpEvento(&(*list)[mid], &val);
            if (comp == -1)
            {
                l = mid + 1;
            }
            else if (comp == 1)
            {
                r = mid;
                if (r == l)
                    break;
            }
            else
            {
                return 1;
            }
        }
        // l é a posição do novo evento
        if (l - 1 >= 0 && cmpData(&(*list)[l - 1].dia, &val.dia) == 0 && cmpHora(&(*list)[l - 1].fim, &val.inicio) == 1)
        {
            return 2; // 2 -> conflito interseção
        }

        void *p = realloc(*list, (n + 1) * sizeof(struct Evento));
        if (p == NULL)
            return 3;
        *list = p;
        shift(*list, l, n + 1);
    }
    (*list)[l] = val;
    return 0; // inserção feita com sucesso
}
void showEvent(struct Evento a)
{
    int *v = a.dia.date;
    int *d = a.inicio.time;
    int *e = a.fim.time;
    printf("Data:\n");
    printf("%d/%d/%d (DD/MM/YYYY)\n", v[0], v[1], v[2]);
    printf("Hora De Inicio:\n");
    printf("%d:%d\n", d[0], d[1]);
    printf("Hora Do Fim:\n");
    printf("%d:%d\n", e[0], e[1]);
    printf("Descricao:\n");
    printf("%s\n", a.descricao);
    printf("Local:\n");
    printf("%s\n", a.local);
}

void showEventos(struct Evento *list, int n, int l, int r)
{
    if (l < 0 || l > r)
        return;
    if (n < 1)
    {
        printf("Nao ha eventos\n");
        return;
    }
    for (int i = l; i < n && i <= r; i++)
    {
        showEvent(list[i]);
        printf("\n");
    }
    return;
}

int readDate(struct Data *dia)
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    if (a > 9999 || a < 0 || b > 12 || b < 1 || a > 31 || a < 1)
    {
        printf("Data invalida!\n");
        return 1;
    }
    dia->date[0] = a;
    dia->date[1] = b;
    dia->date[2] = c;
    return 0;
}

int readTime(struct Horario *hora)
{
    int a, b;
    scanf("%d %d", &a, &b);
    if (a > 23 || a < 0 || b > 59 || b < 0)
    {
        printf("Horario invalido!\n");
        return 1;
    }
    hora->time[0] = a;
    hora->time[1] = b;
    return 0;
}

int generateEvent(struct Evento *input)
{
    printf("Informe a data de inicio do evento.(DD/MM/AAAA)\n");
    if (readDate(&input->dia))
    {

        return 1;
    }
    printf("Informe o horario do inicio do evento. (HH:MM)\n");
    if (readTime(&input->inicio))
    {
        return 1;
    }
    printf("Informe o horario do fim do evento. (HH:MM)\n");
    if (readTime(&input->fim))
    {
        return 1;
    }
    if (cmpHora(&input->inicio, &input->fim) == 1)
    {
        return 1;
    }
    char s[50], v[50];
    printf("Informe a descricao do evento.\n");
    scanf("%s['\n']", input->descricao);
    printf("Informe o Local do evento.\n");
    scanf("%s['\n']", input->local);
    return 0;
}

void queryDate(struct Data a, struct Evento *list, int n)
{
    if (n == 0)
    {
        printf("Nenhum valor encontrado.\n");
        return 0;
    }
    int s = -1, e = -1;
    for (int i = 0; i < n; i++)
    {
        if (cmpData(&a, &list[i].dia) == 0)
        {
            if (s == -1)
            {
                s = i;
                e = i;
            }
            else
            {
                e++;
            }
        }
    }
    if (s != -1)
        showEventos(list, n, s, e);
    else
        printf("Nenhum valor encontrado.\n");
}

void queryDescription(char v[50], struct Evento *list, int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
        if (strcmp(v, list[i].descricao) == 0)
        {
            count++;
            showEvent(list[i]);
        }
    if (count == 0)
        printf("Nenhum evento encontrado.\n");
}

void bshift(struct Evento *list, int pos, int n){
    
}

int deleteEvent(struct Data val, struct Evento **list, int n)
{
    int l = 0, r = n-1;
    while (l <= r)
    {
        int mid = (l + r) / 2;
        int comp = cmpData(&(*list)[mid].dia,&val);
        if (comp == -1)
        {
            l = mid + 1;
        }
        else if (comp == 1)
        {
            r = mid;
            if (r == l)
                return 1;
                break;
        }
        else
        {   
            
            break;
        }
    }
}

int main()
{
    struct Evento *agenda;
    int n = 0;
    agenda = malloc(1 * sizeof(struct Evento));
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
            int error;
            if (generateEvent(&val))
            {
                printf("Dados Invalidos!\n");
                break;
            };
            error = insEvent(&agenda, val, n);
            switch (error)
            {
            case 1:
                printf("Evento nesse horario ja existe!\n");
                break;
            case 2:
                printf("Evento Sobrepoe outro!");
                break;
            case 3:
                printf("Falha na alocacao de memoria!\n");
                break;
            case 0:
                printf("Sucesso!\n");
                n++;
                break;
            }
            break;
        case 's':
            showEventos(agenda, n, 0, n);
            break;
        case 'e':
            run = 0;
            break;
        case 'd':
            struct Data input;
            printf("Informe a data que deseja procurar.\n");
            readDate(&input);
            queryDate(input, agenda, n);
            break;
        default:
            break;
        }
    }
    return 0;
}