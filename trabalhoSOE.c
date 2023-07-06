#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#define MAX_PROCESSES 10

typedef struct {
  int id;
  int priority;
  int time_creation;
  int time_left;
  int running;
} Process; //Nossa estrutura de processo
//Variaveis globais e ponteiros de funcao
typedef int Func(int x);
typedef int FuncPrio (int x, int y);
typedef int Fucn (int x, int y, int z);

Process processes[MAX_PROCESSES];
int num_processes = 0;
int current_process = 0;
int last_process = 0;
int control = 1;
int erro = 0;
int temporeal = 0;
int processos = 0;
int qlqr =0;
Process fila2[MAX_PROCESSES];
int aux = 0;

void create_process(int criacao, int tempo, int prio) { //Cria o processo
  if (((last_process + 1) % MAX_PROCESSES) != current_process) { //Se n tiver cheio
    if(temporeal >= criacao){ //Se o tempo real tá maior que o de criacao
    printf("Tempo atual do sistema: %d\n", temporeal);
    printf("\n\n-------Adicionando Processo---------\n");
    processes[last_process].id = control;
    printf("ID: %d\n", control);
    processes[last_process].time_creation = criacao;
    printf("Tempo de criacao: %d\n", criacao);
    processes[last_process].time_left = tempo;
    printf("Tempo de duracao: %d \n", tempo);
    processes[last_process].priority = prio;
    printf("Prioridade: %d \n", prio);
    for(int cont=0;cont<475000000;cont++){}
    processes[last_process].running = 0;
    printf("-----------------------------\n\n\n");
    int x = schedule(); //Bota ele no buffer e organiza
    last_process = (last_process + 1) % (MAX_PROCESSES);
    }
  } else {
    erro = 1;
  }
} //Vai printar o tempo e adicionar um novo processo, mosntrando a ID, tempo de criação, tempo de duração e prioridade.


void terminate_process(int id) { //Vai executar o processo, vai falar se encontrou e qual a duração
  int tempof;
  int identidade;
  int cont;
  FILE* arquivo_saida = fopen("saida.txt", "a");
  if (last_process != current_process) { //Ve se nao ta vazio
    for (int i = current_process; i != (last_process) % MAX_PROCESSES; i++) {
      if (processes[i].id == id) {
        tempof = processes[i].time_left;
        identidade = processes[i].id;
        printf("Processo encontrado\nID: %d\nTempo de duracao: %d\n", processes[current_process].id, processes[current_process].time_left); //Encontrou
        printf("--------------\n\n\n");
        for (int j = 0; j < tempof; j++) {
          printf("Executando o processo %d de %d segundos.\n", identidade, tempof); //Vai mostrar quanto tempo falta
          fprintf(arquivo_saida, "Executando o processo %d de %d segundos\n", identidade, tempof); //Printa no saida.txt
          for(cont=0;cont<475000000;cont++){} //Simular o tempo
        }
        int tempo = tempof;
        processes[i].id = 0; //Depois de executar, deixa a id, time_left e prio como 0
        processes[i].time_left = 0;
        processes[i].priority = 0;
        current_process = (current_process + 1) % (MAX_PROCESSES); //Aumenta o atual pro proximo
        erro = 0;
        temporeal = tempo + temporeal; //Aumenta o tempo
        printf("Tempo atual do sistema: %d\n", temporeal); //Mostra o tempo
        processos++; //Conta quantos processos já foram
        return;
      }
    }

    printf("Process not found\n");
    printf("--------------\n\n\n");


  }
}

void terminate_process_MFR(int id) { //Terminate process das Multiplas Filas Realimentadas, mesma logica
  int tempof;
  int identidade;
  int cont;
  int tempo;
  FILE* arquivo_saida = fopen("saida.txt", "a");
  if (last_process != current_process) { //Se ta vazio
    for (int i = current_process; i != (last_process) % MAX_PROCESSES; i++) {
      if (processes[i].id == id) {
        tempof = processes[i].time_left;
        identidade = processes[i].id;
        printf("Processo encontrado\nID: %d\nTempo de duracao: %d\n", processes[current_process].id, processes[current_process].time_left);
        printf("--------------\n\n\n");
        if (tempof >= 2) //Se tempo for maior que 2, vai ter que passar pra proxima fila
        {
            for (int j = 0; j < 2; j++) {
            printf("Executando o processo %d de %d segundos.\n", identidade, tempof);
            fprintf(arquivo_saida, "Executando o processo %d de %d segundos\n", identidade, tempof);
            temporeal++;
            for(cont=0;cont<475000000;cont++){}
        }
            tempof = tempof - 2; //Diminui os 2 segundos do tempo
            printf("Faltam %d segundos do processo %d\n", tempof, identidade);
            fprintf(arquivo_saida, "Faltam %d segundos do processo %d\n", tempof, identidade);
            fila2[aux].time_left = tempof;
            fila2[aux].id = identidade;
            fila2[aux].priority = processes[i].priority;
            processes[i].time_left = 0;
            processes[i].id = 0;
            processes[i].priority = 0;
            aux++;
            processos++;
            return;

        }
        if (tempof == 1) //Se ta em 1 so vai rodar 1x
        {
          for (int j = 0; j < 1; j++) {
          printf("Executando o processo %d de %d segundos.\n", identidade, tempof);
          fprintf(arquivo_saida, "Executando o processo %d de %d segundos\n", identidade, tempof);
          for(cont=0;cont<475000000;cont++){}
          tempo = 1;
          temporeal++;
          tempof = 0;
        }
        }
        if (tempof == 0) //Se zerou (acabou) ent zera ID, time left e priority
        {
            processes[i].id = 0;
            processes[i].time_left = 0;
            processes[i].priority = 0;
            processos++;
            erro = 0;
        }
        current_process = (current_process + 1) % (MAX_PROCESSES);
        printf("Tempo atual do sistema: %d\n", temporeal);
        return;
    }

    printf("Process not found\n");
    printf("--------------\n\n\n");
  }
}
  }

int schedule() { //Vai fazer a lógica de quem vai ser executado baseado no menor tempo, vai organizar o buffer circular pra colocar o menor tempo primeiro
  int i;
  int j;
  int helpe;
  for (i = current_process; i != last_process; i = (i + 1) % MAX_PROCESSES) {
    for (j = current_process; j != i; j = (j + 1) % MAX_PROCESSES) {
      if (processes[i].time_left < processes[j].time_left) {
        int temp = processes[i].time_left;
        int tempid = processes[i].id;
        int tempprio = processes[i].priority;
        processes[i].time_left = processes[j].time_left;
        processes[j].time_left = temp;
        processes[i].id = processes[j].id;
        processes[j].id = tempid;
        processes[i].priority = processes[j].priority;
        processes[j].priority = tempprio;
      }
  }
  }
  return processes[current_process].id;
}

void Kernel_SJF() //SHORTEST JOB FIRST
{
  int i, j;
  int help;
  int escolha;
  int tamanho_lista = 0;

  FILE *arquivo; //Abrir o arquivo
  arquivo =fopen("nome.txt", "r");
  int x[20], y[20], z[20]; //cria variáveis auxiliares
  printf("LISTA SEM ORDEM: 1 - TEMPO DE CRIACAO 2 - TEMPO DE DURACAO 3 - PRIORIDADE ESTATICA\n"); //Printar a lista sem ordem (do bloco de notas)

  for(i = 0; i < 20; i++)
  {
      fscanf(arquivo, "%d %d %d", &x[i], &y[i], &z[i]); //abre o arquivo e coloca nas 20 posições do vetor as 20 entradas
  }

  if (arquivo == NULL) //se der erro pra abrir o arquivo
  {
      printf("Erro ao abrir arquivo.\n");
      return 1;
  }
    fclose(arquivo);

  for(i = 0; i < 20; i++)
  {
      printf("%d %d %d\n", x[i], y[i], z[i]); //printa o que viu no arquivo
  }

  for (i = 0; i < 20; i++)
  {
      for (j = 0; j < 20; j++)
      {
        if (x[j] > x[i]) //logica pra mudar as posicoes
        {
            help = x[i];
            x[i] = x[j];
            x[j] = help;
            help = y[i];
            y[i] = y[j];
            y[j] = help;
            help = z[i];
            z[i] = z[j];
            z[j] = help;
        }
      }
  }
  printf("LISTA ORDENADA PELO TEMPO DE CRIACAO:\n"); //Ordena a lista baseada no tempo de criação
  for(i = 0; i < 20; i++)
  {
      printf("%d %d %d\n", x[i], y[i], z[i]); //printa ordenado
  }
    temporeal = 0;
    control = 1;
    processos = 0;
    while(processos < 20) //se tiver processo pra fazer ainda
        {
          if(temporeal>=x[control-1]) //so se o tempo de criacao for maior
          {
          create_process(x[control-1], y[control-1], z[control-1]);
          control++;
          }
          else
          {
          int a = schedule(); //Organiza
          estado_atual(); //Printa Buffer
          terminate_process(a); //Executa
          }
          if (current_process == last_process) //Se tiver vazio, vai contando o tempo e printando
          {
              temporeal++;
              for(i=0;i<475000000;i++){}
              printf("Tempo atual do sistema: %d\n", temporeal);
          }
        }
}

void estado_atual() { //Printa estado atual
  int w, z, y;
  if (last_process != current_process) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
      w = processes[i].id;
      z = processes[i].time_left;
      y = processes[i].priority;
      printf("\nPosicao: %d\n", i);
      printf("ID: %d\n", w);
      printf("Tempo de duracao: %d\n", z);
      printf("Prioridade estatica: %d\n\n", y);
      for(int cont=0;cont<475000000;cont++){}
    }
  }
}

void Kernel_MFR() //Kernel do MFR
{
  int i, j;
  int help;
  int a,b,c;
  int escolha;
  int vazio = 0;
  int tamanho_lista = 0;
  int auxiliar;

  FILE *arquivo;
  arquivo =fopen("nome.txt", "r");
  int x[20], y[20], z[20];
  printf("LISTA SEM ORDEM: 1 - TEMPO DE CRIACAO 2 - TEMPO DE DURACAO 3 - PRIORIDADE\n");

  for(i = 0; i < 20; i++)
  {
      fscanf(arquivo, "%d %d %d", &x[i], &y[i], &z[i]);
  }

  if (arquivo == NULL)
  {
      printf("Erro ao abrir arquivo.\n");
      return 1;
  }
    fclose(arquivo);

  for(i = 0; i < 20; i++)
  {
      printf("%d %d %d\n", x[i], y[i], z[i]);
  }

  for (i = 0; i < 20; i++)
  {
      for (j = 0; j < 20; j++)
      {
        if (x[j] > x[i])
        {
            help = x[i];
            x[i] = x[j];
            x[j] = help;
            help = y[i];
            y[i] = y[j];
            y[j] = help;
            help = z[i];
            z[i] = z[j];
            z[j] = help;
        }
      }
  }
  printf("LISTA ORDENADA PELO TEMPO DE CRIACAO:\n");
  for(i = 0; i < 20; i++)
  {
      printf("%d %d %d\n", x[i], y[i], z[i]);
  }
    temporeal = 0;
    control = 1;
    processos = 0;
    while(processos < 20)
        {
            if(temporeal>=x[control-1])
          {
          create_process(x[control-1], y[control-1], z[control-1]);
          control++;
          }
          else
          {
          int a = schedule();
          estado_atual();
          terminate_process_MFR(a);
          }
          if (current_process == last_process)
          {
              temporeal++;
              for(i=0;i<475000000;i++){}
              printf("Tempo atual do sistema: %d\n", temporeal);
          }
        }
        //ate aqui mesma logica do kernel SJF
    printf("Continuando a fila 2:\n"); //passa para a fila 2 e executa o que faltou
    for(i=0;i<20;i++)
    {
        if (fila2[i].time_left != 0){
        a = fila2[i].id;
        b = fila2[i].time_left;
        c = fila2[i].priority;
        create_process(a,b,c);
        int d = schedule();
        terminate_process(d);
        }
    }

}

int main() {
  int escolha; //para o switch
  printf("Nosso codigo e baseado em: \n-SHORTEST JOB FIRST (QUANTO MENOR O TEMPO DE EXECUCAO, MAIOR A PRIORIDADE)\n-MULTIPLAS FILAS COM REALIMENTACAO\n");
  while(escolha != 3){ //explicacao e loop que enquanto n digitar 3 n sai
  temporeal = 0;
  control = 1;
  processos = 0;
  printf("1 - SHORTEST JOB FIRST\n2 - MULTIPLAS FILAS COM REALIMENTACAO\n3 - SAIR DO PROGRAMA\n"); //explica as possibilidade
  scanf("%d", &escolha);
  switch(escolha)
  {
    case 1:
        Kernel_SJF(); //case 1 vai para o SJF
        break;

    case 2:
        Kernel_MFR(); //case 2 vai para o MFR
        break;

    case 3:
        break; //case 3 para sair do switch
  }
  }
  return 0;
}
