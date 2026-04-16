#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compara(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
}

void ordenar_externo(int M){
    FILE *entrada = fopen("C://Users//HOME//Documents//ED II//T1//dados.txt", "rt");
    if(entrada == NULL) {
        printf("Erro: arquivo dados.txt nao encontrado");
        exit(1);
    }
    int *buffer = (int*)malloc(M * sizeof(int));
    char filename[20];
    int num_arquivos = 0;
    int i;
//Gerando blocos ordenados
    while(!feof(entrada)){
        int count = 0;
        while(count < M && fscanf(entrada, "%d%*c", &buffer[count]) == 1){
            count++;
        }

        if(count > 0){
            qsort(buffer, count, sizeof(int), compara);//QuickSort

            sprintf(filename, "temp%d.dat", num_arquivos++);
            FILE *temp = fopen(filename, "w");
            for(i = 0; i < count; i++){
                fprintf(temp, "%d%s", buffer[i], (i == count - 1?"" : ";"));
            }
            fclose(temp);
        }
    }
    fclose(entrada);

    FILE **temps = malloc(num_arquivos * sizeof(FILE*));
    int *valores = malloc(num_arquivos * sizeof(int));
    int *ativo = malloc(num_arquivos * sizeof(int));

    for(i = 0; i < num_arquivos; i++){
        sprintf(filename, "temp%d.dat", i);
        temps[i] = fopen(filename, "r");
        if(fscanf(temps[i], "%d%*c", &valores[i]) == 1){
            ativo[i] = 1;
        } else{
            ativo[i]= 0;
        }   
    }

    FILE *saida = fopen("dados_ordenados.txt", "w");
    int first_write = 1;

    while (1){
        int menor_idx = -1;
        for(i = 0; i < num_arquivos; i++){
            if(ativo[i]){
                if(menor_idx == -1 || valores[i] < valores[menor_idx]){
                    menor_idx = i;
                }
            }
        }
        if(menor_idx == -1) break; //Se não houver mais nada nos arquivos "Break"
        
            if(!first_write) fprintf(saida, ";");
            fprintf(saida, "%d", valores[menor_idx]);
            first_write = 0;

            if(fscanf(temps[menor_idx], "%d%*c", &valores[menor_idx]) != 1){
                ativo[menor_idx] = 0;            
            }
    }

    // Fechando
    for (i = 0; i < num_arquivos; i++) {
        fclose(temps[i]);
        sprintf(filename, "temp%d.dat", i);
        remove(filename); // Deleta temporários
    }
    fclose(saida);
    free(buffer); free(temps); free(valores); free(ativo);

    printf("resultado_ordenado.txt\n");
    
}

int main(){
    int M;
    if(scanf("%d", &M)==1){
        ordenar_externo(M);
    }
    return 0;
}