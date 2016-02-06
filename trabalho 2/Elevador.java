/* Disciplina: Computacao Concorrente */

/* Prof.: Silvana Rossetto */

/* Alunos:  Estevan Gladstone 
            Romeu Pires
*/
            
/* Trabalho 2 */

/* Codigo: Implementar um controlador de elevadores */
/* -------------------------------------------------------------------

TODO:
- Vetor de requisições construido pela main a partir do arquivo
- Os Elevadores e o Monitor devem escrever em arquivos de Log
- Implementar o método getRequestsOnLevel()
- Terminar a função getNearRequest() do monitor
- Elevador marcar andar como "seu"

------------------------------------------------------------------- */

import java.util.Arrays;
import java.util.Scanner;

import org.apache.commons.lang3.ArrayUtils;

// Classe do Elevador implementado como Thread

class Elevador extends Thread{

  private Monitor monitor;              // Monitor do elevador 
  private Requisicao[] requisicoes;     // Vetor de requisições do elevador
  private int id;                       // Recurso compartilhado
  private int capacidade;               // Capacidade do elevador
  private int andar_atual;              // Andar atual do elevador no prédio

  // Construtor
  public Elevador(int tid, int capacidade, Monitor monitor) {
    this.id = tid;
    this.capacidade = capacidade;
    this.monitor = monitor;
  }

  public void run() {

    /* Marcar andar como 'já é meu!!' */
    while(int destino = monitor.getNearRequest(andar_atual) != -1){ // TEM Q SER SYNCHRONIZED
      
      // Vai até um andar para pegar pessoas
      this.goTo(destino);

      // Pega até C pessoas daquele andar
      requisicoes = monitor.getRequestsOnLevel(capacidade, andar_atual); // TEM Q SER SYNCHRONIZED

      /* Ordenar o vetor de requisições pode facilitar as coisas (ordenar como?)*/
      // Funcao ordenaRequests() ordenar de acordo com a forma que vai percorrer os andares (ou nao)

      while(requisicoes.length > 0){
        // Pega sempre o primeiro elemento, por ser uma "fila"
        destino = requisicoes[0].getDestino(); // pega o destino de uma requisição do vetor

        this.goTo(destino); // Vai até o andar da requisição

        requisicoes = ArrayUtils.remove(requisicoes, 0); // Libera a requisição no andar de destino
      }

    }

  }

  // Faz o "deslocamento" do elevador para o andar de destino
  private void goTo(int destino){
    while(andar_atual != destino){
      if(andar_atual < destino)
        andar_atual++;
      else
        andar_atual--;
      // andar_atual < destino ? andar_atual++ : andar_atual--; 
    }
  }

}

// Classe das Requisição feita por um usuário do elevador
class Requisicao {
  private int id;
  private int destino;

  public Requisicao(int id, int destino){
    this.id = id;
    this.destino = destino;
  }

  public int getDestino(){
    return this.destino;
  }
}

// Classe do Monitor dos elevadores
class Monitor {

  // Função principal da aplicação
  public static void main (String[] args) {
    // Reserva espaço para um vetor de threads
    Thread[] elevadores = new Thread[N];  // Vetor referenciando as threads dos elevadores
    int c = 10;                           // Capacidade dos elevadores

    // Cria as threads da aplicação
    for (int i = 0; i < elevadores.length; i++) {
      elevadores[i] = new Elevador(i, c, );
    }

    // Inicia os elevadores
    for (int i=0; i<elevadores.length; i++) {
       elevadores[i].start();
    }

    // Espera pelo termino de todos os elevadores
    for (int i = 0; i < elevadores.length; i++) {
       try {
        elevadores[i].join();
       }
       catch (InterruptedException e) {
        return; // Por enquanto ignoramos a Exception
       }
    }
  }

  // Retorna o andar com requisições mais perto do elevador
  public synchronized int getNearRequest(int andar){
  
    // Caso haja requisições no andar atual do elevador
    if(requisicoes[andar] > 0){
      return andar;
    }
    else{
      x = andar_atual + 1;
      y = andar_atual - 1;
      while(x < andar_maximo || y > andar_minimo){
        /* Tem que checar se o andar já não é de algum outro elevador também */
        if(requisicoes_no[x] > requisicoes_no[y])
          return x;
        else
          return y;
        
        if(x < andar_maximo){ x++; }
        if(y > andar_minimo){ y--; }
      }

      return -1; // Retorna -1 caso não tenha nenhum andar com requisições
    }

  }

  // retornar um vetor com as requests de tamanho até a CAPACIDADE do elevador
  public synchronized Requisicao getRequestsOnLevel(int capacidade, int andar){ 
    // requisicoes é uma matriz andares x requisicoes (ver se tem melhor forma de armazenar)
    Requisicao[] pedido;

    if(requisicoes[andar].length > capacidade){
      pedido = ArrayUtils.subarray(requisicoes[andar], 0, capacidade - 1);
      requisicoes[andar] = ArrayUtils.subarray(requisicoes[andar], capacidade, requisicoes[andar].length - 1);
    }
    else{
      pedido = ArrayUtils.subarray(requisicoes[andar], 0, requisicoes[andar].length - 1);
      requisicoes[andar] = NULL; // "????"
    }

    return pedido;
  }

}
