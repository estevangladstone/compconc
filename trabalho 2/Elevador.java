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

  private Monitor monitor;              // Monitor do elevador 
  private Requisicao[] requisicoes;     // Vetor de requisições do elevador
  private int id;                       // Recurso compartilhado
  private int capacidade;               // Capacidade do elevador
  private int andar_atual;              // Andar atual do elevador no prédio
  private int andar_destino;            // Onde vai deixar O ULTIMO passageiro
  private int passageiros;              // Numero de passageiros no momento

  // Construtor
  public Elevador(int tid, int capacidade, int andar_inicial, Monitor monitor) {
    this.id = tid;
    this.capacidade = capacidade;
    this.andar_atual = andar_inicial;
    this.monitor = monitor;
  }

  public void run() {

    int destino;

    destino = monitor.getNearestRequest(andar_atual);
    while(destino != -1){
      
      // Vai até um andar para pegar pessoas
      System.out.println("E["+id+"] Buscando request: Indo para "+destino);
      this.goTo(destino);

      // Pega até C pessoas daquele andar
      requisicoes = monitor.getRequestsOnLevel(andar_atual, capacidade);
      System.out.println("E["+id+"] pegou "+requisicoes.length+" requisicoes");

      /* Ordenar o vetor de requisições pode facilitar as coisas (ordenar como?)*/
      // Funcao ordenaRequests() ordenar de acordo com a forma que vai percorrer os andares (ou nao)

      for(int i = 0; i < requisicoes.length; i++){
        // Pega sempre o primeiro elemento, por ser uma "fila"
        destino = requisicoes[i].getDestino(); // pega o destino de uma requisição do vetor

        System.out.println("E["+id+"] Indo para "+destino);
        this.goTo(destino); // Vai até o andar da requisição

        System.out.println("E["+id+"] liberou a requisição "+requisicoes[i].getId()+" no andar "+requisicoes[i].getDestino()+"="+andar_atual);
      }
      System.out.println("E["+id+"] Terminou uma leva no andar "+andar_atual);

      System.out.println("E["+id+"] pegando próximo destino");
      destino = monitor.getNearestRequest(andar_atual);
      System.out.println("E["+id+"] Próximo detino = "+destino);
    }
    System.out.println("E["+id+"] Saiu");
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

  private Andar[] andares;

  public boolean readInput(String input){

    String nome_arquivo = input;

    try{
      FileReader arquivo_de_entrada = new FileReader(nome_arquivo);
      BufferedReader buffer = new BufferedReader(arquivo_de_entrada);

      //Variable to hold the one line data
      String linha;
      boolean primeira_linha = true,
              segunda_linha = false;
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
          andares = new Andar[numero_andares];

          this.numero_elevadores = Integer.valueOf(partes[1]);
          this.capacidade_elevador = Integer.valueOf(partes[2]);
          System.out.println(numero_andares);
          System.out.println(numero_elevadores);
          System.out.println(capacidade_elevador);

          primeira_linha = false;
          segunda_linha = true;
        }
        else if(segunda_linha){
          if(partes.length != this.numero_elevadores){  
            System.out.println("Erro: arquivo de entrada inválido! Número de elevadores não bate com os andares inciais.");
            return false;
          }

          andar_elevador = new int[numero_elevadores];
          for(int i = 0; i < numero_elevadores; i++){
            andar_elevador[i] = Integer.valueOf(partes[i]);
            System.out.println("andar elevador "+i+" : "+andar_elevador[i]);
          }

          segunda_linha = false;
        }
        else{
          if(partes.length < 1 || partes.length-1 != Integer.valueOf(partes[0])){
            System.out.println("Erro: arquivo de entrada inválido! Número de requisicoes no andar inválido");
            return false;
          }
          andares[andar] = new Andar();
          for(int i = 1; i <= Integer.valueOf(partes[0]); i++){
            int pedido = Integer.valueOf(partes[i]);
            // System.out.println("oioi");
            if(pedido < 0 || pedido > numero_andares-1){
              System.out.println("Erro: arquivo de entrada inválido! Requisição com andar inválido");
              return false;    
            }
            Requisicao nova_requisicao = new Requisicao(rid++, Integer.valueOf(partes[i]));
            andares[andar].adicionarRequisicao(nova_requisicao);
          }
          andares[andar].setStatus(true);
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

    if(andares[andar].quantidadeRequisicoes() > 0 && andares[andar].livre()){
      System.out.println("Mesmo andar "+andar);
      return andar;
    }
    else{
      andar_acima = andar;
      andar_abaixo = andar;

      do{
        if(andar_acima < numero_andares-1){ andar_acima++; }
        if(andar_abaixo > 0){ andar_abaixo--; }

        if(andares[andar_acima].quantidadeRequisicoes() > andares[andar_abaixo].quantidadeRequisicoes()){
          // System.out.println("aqui 10");        
          if(andares[andar_acima].livre()){
            // System.out.println("aqui 20");
            andares[andar_acima].setStatus(false);
            return andar_acima;
          }
        }
        else{
          // System.out.println("aqui 1");
          if(andares[andar_abaixo].livre() && andares[andar_abaixo].quantidadeRequisicoes() > 0){
            // System.out.println("aqui 2");
            andares[andar_abaixo].setStatus(false);
            return andar_abaixo;
          }
        }

        // System.out.println("Preso");
      } while(andar_acima != numero_andares-1 || andar_abaixo != 0);

      return -1; // Retorna -1 caso não tenha nenhum andar com requisições
    }
  }

  // retornar um vetor com as requests de tamanho até a CAPACIDADE do elevador
  public synchronized Requisicao[] getRequestsOnLevel(int andar, int capacidade){
    andares[andar].setStatus(true);
    return andares[andar].retirarRequisicoes(capacidade);
  }

  // Função principal da aplicação
  public static void main (String[] args) {

    if(args.length != 1){
      System.out.println("Erro: entrada inválida!\nTente: java <nome do programa> <nome do arquivo de dados>");
      return;
    }

    Monitor monitor = new Monitor();

    System.out.println("Lendo entradas");
    
    if(!monitor.readInput(args[0])){ return; }

    // Reserva espaço para um vetor de threads
    Thread[] elevadores = new Thread[monitor.numero_elevadores];

    System.out.println("Criando Threads");
    // Cria as threads da aplicação
    for (int i = 0; i < elevadores.length; i++) {
      elevadores[i] = new Elevador(i, monitor.capacidade_elevador, monitor.andar_elevador[i], monitor);
    }

    System.out.println("Iniciando elevadores");
    // Inicia os elevadores
    for (int i = 0; i < elevadores.length; i++) {
       elevadores[i].start();
    }

    System.out.println("Join nas Threads");
    // Espera pelo termino de todos os elevadores
    for (int i = 0; i < elevadores.length; i++) {
      try {
        elevadores[i].join();
      }
      catch (InterruptedException e) {
        System.out.println("Interrupted");
        return; // Por enquanto ignoramos a Exception
      }
    }
    for(int i=0;i<monitor.numero_andares;i++){
      System.out.println(monitor.andares[i].quantidadeRequisicoes());
    }
    System.out.println("Acabou...");
  }

}
