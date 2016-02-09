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
import java.io.*;
import java.lang.String;
import java.util.*;

// Classe do Elevador implementado como Thread

class Elevador extends Thread{

  private Monitor monitor;                        // Monitor do elevador 
  private ArrayList<Requisicao> requisicoes;      // Vetor de requisições do elevador
  private int id;                                 // Recurso compartilhado
  private int capacidade;                         // Capacidade do elevador
  private int andar_atual;                        // Andar atual do elevador no prédio

  // Construtor
  public Elevador(int tid, int capacidade, int andar_inicial, Monitor monitor) {
    this.id = tid;
    this.capacidade = capacidade;
    this.andar_atual = andar_inicial;
    this.monitor = monitor;
  }

  public void run() {

    int destino;

    while((destino = monitor.getNearestRequest(andar_atual)) != -1){
      
      // Vai até um andar para pegar pessoas
      this.goTo(destino);

      // Pega até C pessoas daquele andar
      requisicoes = monitor.getRequestsOnLevel(andar_atual, capacidade);

      /* Ordenar o vetor de requisições pode facilitar as coisas (ordenar como?)*/
      // Funcao ordenaRequests() ordenar de acordo com a forma que vai percorrer os andares (ou nao)

      while(requisicoes.size() > 0){
        // Pega sempre o primeiro elemento, por ser uma "fila"
        destino = requisicoes.get(0).getDestino(); // pega o destino de uma requisição do vetor

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

// Classe do Monitor dos elevadores
class Monitor {

  private int numero_andares,           // Número de andares
              numero_elevadores,        // Número de elevadores do prédio
              capacidade_elevador;      // Capacidade dos elevadores
  private int[] andar_elevador;
  
  private boolean[] andar_livre;

  private ArrayRequisicoes<Requisicao> requisicoes = new ArrayRequisicoes<Requisicao>();

  public boolean readInput(String[] args){

    String nome_arquivo = args[0];

    try{
      FileReader arquivo_de_entrada = new FileReader(nome_arquivo);
      BufferedReader buffer = new BufferedReader(arquivo_de_entrada);

      //Variable to hold the one line data
      String linha;
      boolean primeira_linha = true,
              segunda_linha = true;
      int andar = 0, rid = 0;

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
          // System.out.println(numero_andares);
          // System.out.println(numero_elevadores);
          // System.out.println(capacidade_elevador);

          primeira_linha = false;
        }
        else if(segunda_linha){
          if(partes.length != this.numero_elevadores){  
            System.out.println("Erro: arquivo de entrada inválido! Número de elevadores não bate com os andares inciais.");
            return false;
          }

          andar_elevador = new int[this.numero_elevadores];
          for(int i = 0; i < this.numero_elevadores; i++){
            this.andar_elevador[i] = Integer.valueOf(partes[i]);
            System.out.println("andar elevador "+i+" : "+andar_elevador[i]);
          }

          segunda_linha = false;
        }
        else{
          for(String pedido : partes){
            Requisicao nova_requisicao = new Requisicao(rid++, Integer.valueOf(pedido));
            requisicoes.addToIndex(andar, nova_requisicao);
          }          
          andar++;
          if(andar > numero_andares){ return false; }
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
    int andar_abaixo, andar_acima;

    if(!requisicoes.get(andar).isEmpty() && andar_livre[andar]){
      return andar;
    }
    else{
      andar_acima = andar + 1;
      andar_abaixo = andar - 1;
      while(andar_acima < numero_andares || andar_abaixo > 0){
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
        
        if(andar_acima < numero_andares){ andar_acima++; }
        if(andar_abaixo > 0){ andar_abaixo--; }
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
      return;
    }

    Monitor monitor = new Monitor();

    System.out.println("Lendo entradas");
    
    if(!monitor.readInput(args)){ return; }
    System.out.println(monitor.requisicoes.takeSubArray(1, 3).get(0).getId());
    // monitor.printArray();

    // System.out.println("Tamanho do Array: "+monitor.requisicoes.size());

    // // Reserva espaço para um vetor de threads
    // Thread[] elevadores = new Thread[monitor.numero_elevadores];
    // monitor.andar_livre = new boolean[monitor.numero_andares];

    // System.out.println("Criando Threads");
    // // Cria as threads da aplicação
    // for (int i = 0; i < elevadores.length; i++) {
    //   elevadores[i] = new Elevador(i, monitor.capacidade_elevador, monitor.andar_elevador[i], monitor);
    // }

    // System.out.println("Iniciando elevadores");
    // // Inicia os elevadores
    // for (int i=0; i<elevadores.length; i++) {
    //    elevadores[i].start();
    // }

    // System.out.println("Join nas Threads");
    // // Espera pelo termino de todos os elevadores
    // for (int i = 0; i < elevadores.length; i++) {
    //   try {
    //     elevadores[i].join();
    //   }
    //   catch (InterruptedException e) {
    //     return; // Por enquanto ignoramos a Exception
    //   }
    // }
  }

  // private void printArray(){
  //   System.out.println("Tamanho : "+requisicoes.size());
  //   for(ArrayList inner : requisicoes){
  //     System.out.println("Tamanho interno : "+inner.size());
  //     // System.out.println(inner);
  //     for(int i=0;i<inner.size();i++){
  //       Requisicao req = inner.get(i);
  //       System.out.println(req.getDestino());
        
  //       // Requisicao req = inner.get(i);
  //       // System.out.println("R["+req.getId()+"] : "+req.getDestino());
  //     }
  //   }
  // }

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

  public T getSub(){
    return this.get(2).get(0);
  }

  // Retira e retorna um subarray com até 'capacidade' requisições
  public ArrayList<T> takeSubArray(int index, int amount){
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
    // System.out.println(index);
    return this.get(index).size();
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

  public int getId(){
    return this.id;
  }
}

