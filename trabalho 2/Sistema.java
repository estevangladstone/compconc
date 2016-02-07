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
- Os Elevadores, Sistema e o Controlador devem escrever em arquivos de Log
- Implementar o método getRequestsOnLevel()
- Terminar a função getNearRequest() do monitor
- Elevador marcar andar como "seu"

------------------------------------------------------------------- */
import java.io.*;
import java.lang.String;
import java.util.*;

// Classe do Elevador implementado como Thread

class Elevador extends Thread{

  private Controlador controlador;      // Controlador do elevador 
  private Requisicao[] requisicoes;     // Vetor de requisições do elevador
  private int id;                       // Recurso compartilhado
  private int capacidade;               // Capacidade do elevador
  private int andar_atual;              // Andar atual do elevador no prédio

  // Construtor
  public Elevador(int tid, int capacidade, Controlador controlador) {
    this.id = tid;
    this.capacidade = capacidade;
    this.controlador = controlador;
  }

  public void run() {

    int destino;

    while((destino = controlador.getNearRequest(andar_atual)) != -1){
      
      // Vai até um andar para pegar pessoas
      this.goTo(destino);

      // Pega até C pessoas daquele andar
      requisicoes = controlador.getRequestsOnLevel(andar_atual, capacidade);

      /* Ordenar o vetor de requisições pode facilitar as coisas (ordenar como?)*/
      // Funcao ordenaRequests() ordenar de acordo com a forma que vai percorrer os andares (ou nao)

      while(requisicoes.length > 0){
        // Pega sempre o primeiro elemento, por ser uma "fila"
        destino = requisicoes[0].getDestino(); // pega o destino de uma requisição do vetor

        this.goTo(destino); // Vai até o andar da requisição

        requisicoes.remove(0); // Libera a requisição no andar de destino
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

  private int numero_andares,           // Número de andares
              numero_elevadores,        // Número de elevadores do prédio
              capacidade_elevador;      // Capacidade dos elevadores
  private boolean[] andar_livre;

  public boolean readInput(String[] args){

    String nome_arquivo = args[0];

    try{
      FileReader arquivo_de_entrada = new FileReader(nome_arquivo);
      BufferedReader buffer = new BufferedReader(arquivo_de_entrada);

      //Variable to hold the one line data
      String linha;
      boolean primeira_linha = true;
      int andar = 0;

      // Lê o arquivo e cria as variáveis correspondentes
      while ((linha = buffer.readLine()) != null){
        String[] partes = linha.split("\\s+");

        if(primeira_linha){
          if(partes.length != 3){  
            System.out.println("Erro: arquivo de entrada inválido! Dados insuficientes.");
            return false;
          }
          this.numero_andares = Integer.valueOf(partes[0]);
          this.numero_elevadores = Integer.valueOf(partes[1]);
          this.capacidade_elevador = Integer.valueOf(partes[2]);

          primeira_linha = false;
        }
        else{
          for(String pedido : partes){
            requisicoes.adicionarRequisicao(andar, Integer.valueOf(pedido));
            System.out.print(Integer.valueOf(pedido)+" ");
          }          
          System.out.print("\n");
          andar++;
        }
      }
      //Close the buffer reader
      buffer.close();
    }
    catch(Exception e){
      System.out.println("Erro durante leitura de dados de entrada: " + e.getMessage());
      return false;
    }

    return true;
  }

  // Retorna o andar com requisições mais perto do elevador
  public synchronized int getNearestRequest(int andar){
    // Caso haja requisições no andar atual do elevador
    int andar, andar_abaixo, andar_acima;

    if(!requisicoes.isEmpty(andar)){ // Problemas: se o andar já estiver ocupado??
      return andar;
    }
    else{
      andar_acima = andar + 1;
      andar_abaixo = andar - 1;
      while(andar_acima < andar_maximo || andar_abaixo > andar_minimo){
        if(requisicoes.innerArraySize(andar_acima) > requisicoes.innerArraySize(andar_abaixo))
          if(andar_livre[andar_acima]){
            andar_livre[andar_acima] = false;
            return andar_acima;
          }
        else
          if(andar_livre[andar_abaixo]){
            andar_livre[andar_abaixo] = false;
            return andar_abaixo;
          }
        
        if(andar_acima < andar_maximo){ andar_acima++; }
        if(andar_abaixo > andar_minimo){ andar_abaixo--; }
      }

      return -1; // Retorna -1 caso não tenha nenhum andar com requisições
    }

  }

  // retornar um vetor com as requests de tamanho até a CAPACIDADE do elevador
  public synchronized ArrayList<Requisicao> getRequestsOnLevel(int andar, int capacidade){
    return requisicoes.takeSubArray(andar, capacidade);
  }

  // Função principal da aplicação
  public static void main (String[] args) {

    if(args.length != 1){
      System.out.println("Erro: entrada inválida!\nTente: java <nome do programa> <nome do arquivo de dados>");
      return false;
    }

    Monitor monitor = new Monitor();

    monitor.readInput(args);

    // Reserva espaço para um vetor de threads
    Thread[] elevadores = new Thread[monitor.numero_elevadores];
    andar_livre = new boolean[monitor.numero_andares];

    // Cria as threads da aplicação
    for (int i = 0; i < elevadores.length; i++) {
      elevadores[i] = new Elevador(i, monitor.capacidade_elevador, monitor);
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

}

// Classe principal da aplicação
class Sistema {

  // Função principal da aplicação
  public static void main (String[] args) {

    if(args.length != 1){
      System.out.println("Erro: entrada inválida!\nTente: java <nome do programa> <nome do arquivo de dados>");
      return false;
    }

    Controlador controlador = new Controlador();

    controlador.readInput(args);

    // Reserva espaço para um vetor de threads
    Thread[] elevadores = new Thread[controlador.numero_elevadores];
    andar_livre = new boolean[controlador.numero_andares];

    // Cria as threads da aplicação
    for (int i = 0; i < elevadores.length; i++) {
      elevadores[i] = new Elevador(i, controlador.capacidade_elevador, controlador);
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

}


// Array de duas dimensões para guardar as requisições
class ArrayRequisicoes<T> extends ArrayList<ArrayList<T>> {

  // Inclui uma requisição no array do indice 'index'
  public void addToIndex(int index, T item) {
    while (index >= this.size()) {
        this.add(new ArrayList<T>());
    }
    this.get(index).add(item);
  }

  // Retira e retorna um subarray com até 'capacidade' requisições
  public ArrayList<T> retireSubArray(int index, int amount){
    ArrayList<T> sublist;

    if(this.get(index).size() < amount){
      sublist = new ArrayList<T>(this.get(index));
      this.get(index).removeAll(sublist);
    }
    else{
      sublist = new ArrayList<T>(this.get(index).subList(0, amount));
      this.get(index).removeAll(sublist);
    }

    return sublist;
  }

  public int innerArraySize(int index){
    return this.get(index).size();
  }

}

