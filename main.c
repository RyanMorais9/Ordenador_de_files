#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubbleSort(int *vetor, int n){ //BubbleSort O(M^2)
    int i, j, aux;
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < n - i - 1; j++){
            if(vetor[j] > vetor[j - 1]){
                aux = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
            }
        }
    }
}

void ordenar_externo(int M){
    FILE *entrada = fopen("C:/Users/HOME/Documents/ED II/T1/dados.txt", "r");
    if(entrada == NULL) {
        printf("Erro: arquivo dados.txt nao encontrado");
        exit(1);
    }
    int *buffer = (int*)malloc(M * sizeof(int));
    char filename[20];
    int num_arquivos = 0;
    int i;
// Gerando blocos ordenados
int count = 0;
// O loop principal é o próprio fscanf
while (fscanf(entrada, "%d", &buffer[count]) == 1) { // O(N)
    // Após ler o número, consumimos o próximo caractere apenas se for ';' ou " "
    fgetc(entrada); 
    
    printf("Lido: %d (Posicao no bloco: %d)\n", buffer[count], count);
    count++;

    // Se encheu o buffer M ou se o arquivo acabou
    if (count == M) {
        bubbleSort(buffer, count); 
        sprintf(filename, "temp%d.dat", num_arquivos++); //O(N)
        FILE *temp = fopen(filename, "w");
        for (i = 0; i < count; i++) {
            fprintf(temp, "%d%s", buffer[i], (i == count - 1 ? "" : ";")); //Função ternaria que coloca " " se for o ultimo valor se nao ";x"
        }
        fclose(temp);
        printf("--- Bloco %d criado e salvo ---\n", num_arquivos - 1);
        count = 0; // Reseta para o próximo bloco
    }
}

// Trata o resto: Se o total de números não for múltiplo de M
if (count > 0) {
    bubbleSort(buffer, count); 
    sprintf(filename, "temp%d.dat", num_arquivos++);
    FILE *temp = fopen(filename, "w");
    for (i = 0; i < count; i++) {
        fprintf(temp, "%d%s", buffer[i], (i == count - 1 ? "" : ";"));
    }
    fclose(temp);
    printf("--- Bloco final %d criado ---\n", num_arquivos - 1);
}

    FILE **temps = malloc(num_arquivos * sizeof(FILE*));
    int *valores = malloc(num_arquivos * sizeof(int)); 
    int *ativo = malloc(num_arquivos * sizeof(int)); //Garante que o arquivo pare de ser lido se nao tiver mais dados

    for(i = 0; i < num_arquivos; i++){ //MergeSort
        sprintf(filename, "temp%d.dat", i);
        temps[i] = fopen(filename, "r");
        if(fscanf(temps[i], "%d%*c", &valores[i]) == 1){
            ativo[i] = 1;
        } else{
            ativo[i]= 0;
        }   
    }

    FILE *saida = fopen("C:/Users/HOME/Documents/ED II/T1/dados_ordenados.txt", "w");
    int first_write = 1;
    //O(N * K)
    while (1){
        int menor_idx = -1; //Serve para guardar o menor valor atual
        for(i = 0; i < num_arquivos; i++){
            if(ativo[i]){
                if(menor_idx == -1 || valores[i] < valores[menor_idx]){
                    menor_idx = i;
                }
            }
        }
        if(menor_idx == -1) break; //Se não houver mais nada nos arquivos "Break"
        
            if(!first_write) fprintf(saida, ";"); // Evitar erro com ";" sobrando, coloca so depois do primeiro numero
            fprintf(saida, "%d", valores[menor_idx]);
            first_write = 0;

            if(fscanf(temps[menor_idx], "%d%*c", &valores[menor_idx]) != 1){ //Tenta ler o proximo numero do mesmo file
                ativo[menor_idx] = 0; //Se nao houver, ele na vai mais ler este file
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

    printf("dados_ordenado.txt\n");
    
}

int main() {
    int M;

    do {
        printf("Digite a quantidade de memoria (M >= 1): ");
        if (scanf("%d", &M) != 1) {
            while (getchar() != '\n'); //Limpa o buffer em caso de uma letra ser digitada
            M = 0;
            printf("Erro: Digite um numero valido.\n");
            continue;
        }

        if (M < 1) {
            printf("Erro: O valor de M deve ser pelo menos 1.\n");
        }
    } while (M < 1);
    ordenar_externo(M);
    
    return 0;
}