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
    for (int i = 0; i < 3; i++)
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

int cmpEvento(struct Evento *a, struct Evento *b){
    // perguntar rui, por enquanto &() basta
    int dateRes = cmpData(&(a->dia),&(b->dia));
    if(dateRes == 0){
        int hourRes = cmpHora(&(a->inicio), &(b->inicio));
        return hourRes;
    }else
        return dateRes;
}

int insEvent(struct Evento *list, struct Evento val, int n)
{
    int l = 0, r = n - 1;
    while (l < r)
    {
        int mid = (l + r) / 2;
        int cmp = cmpEvento(&(list[mid]),&val);
        if (cmp == 1)
        {
            l = mid+1;
        }else if(cmp == -1){
            r = mid;
        }else{
            return 1; // 1 -> conflito inicio
        }
    }
    // TODO verificar interseção e inserção
}

void showEventos(struct Evento *list, int n){
    for (int i = 0; i < n; i++)
    {
        int *v = list->dia.date;
        printf("Data:\n");
        printf("%d/%d/%d (DD/MM/YYYY)\n", v[0],v[1],v[2]);
        

    }
    
}

int main()
{
    struct Evento *agenda;
    // TODO input do File
    return 0;
}