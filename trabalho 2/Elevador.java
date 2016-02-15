import java.io.*;
import java.lang.String;
import java.util.*;

// Classe do Elevador implementado como Thread

class Elevador extends Thread{

  private Monitor           monitor;        // Monitor do elevador 
  private ArrayList<Requisicao>   requisicoes;    // Vetor de requisicoes do elevador. Notar que as requisicoes antes de chegar no elevador pra pega-las.
  private boolean         vazio;      /* Para saber se esta vazio - e consequentemente indo coletar requisicoes. */
  private int             id;             // Recurso compartilhado
  private int             capacidade;     // Capacidade do elevador
  private int             andar_atual;    // Andar atual do elevador no predio
  private int             andar_destino;  // Onde vai deixar O ULTIMO passageiro. Se for -1 eh pq nao tem destino.
  private int             passageiros;  // Numero de passageiros no momento
  private PrintWriter         escritor;   // Responsavel pelo arquivo de saida do elevador
  
  // Construtor
  public Elevador(int tid, int capacidade, int andar_inicial, Monitor monitor) {
    this.id = tid;
    this.capacidade = capacidade;
    this.andar_atual = andar_inicial;
    this.andar_destino = -1;
    this.monitor = monitor;
    this.passageiros = 0;
    this.vazio = true;
    String nome_do_arquivo = new String("log_elevador" + tid + ".txt");
    try{
      escritor = new PrintWriter( nome_do_arquivo , "UTF-8");
    }catch(Exception e){};
  }

  public void run() {

    searchForRequisitions();
    while(andar_destino != -1){

    //Se aproxima do andar destino
    moveOn();
    
    //Caso tenha gente no elevador , checa pra ver se nao precisa deixar alguem descer:
    if( !vazio && requisicoes.size() > 0 ) checaAndar();
  
    
    //Caso tenha chegado ao destino:
    if(andar_atual == andar_destino){
    
      //Caso tenha chegado vazio, eh pra pegar as requisicoes do andar atual;
      if(vazio){
        vazio = false;
        
        int novo_destino = andar_atual;  //Qual o andar mais longe para o qual eu vou?
        escritor.println("Entrando no elevador as requisicoes:");
        for( Requisicao req : requisicoes ){
          escritor.println("ID " + req.getId() + ", de destino " + req.getDestino() );
          if( abs(req.getDestino() - andar_atual) > abs(novo_destino - andar_atual) )
            novo_destino = req.getDestino();
        }
        andar_destino = novo_destino;
        escritor.println("Novo destino pro elevador:" + andar_destino ); 
      }
      
      //Caso contrario, eh pq acabou de deixar o ultimo passageiro descer.
      else{
        escritor.println("Nao ha mais passageiros. Estacionado no andar " + andar_atual + ".");
        vazio = true;
        //Sera que ainda tem requisicoes para buscar?
        searchForRequisitions();
        if(requisicoes.size() == 0){
          andar_destino = -1;
          escritor.println("Nao ha mais requisicoes pendentes. Encerrando essa thread");
        }else{
          andar_destino = requisicoes.get(0).getOrigem();
        }
      }
      
    }
    }
  escritor.close();
  }

  
  //Checa se no andar atual tem gente querendo descer. Se tiver, o faz.
  private void checaAndar(){  
    for(Requisicao req : requisicoes){
      if( req.getDestino() == andar_atual ){
        //Vai com deus.
        requisicoes.remove(req);
        escritor.println("Descendo no andar " + andar_atual + " a requisicao de ID " + req.getId() + " .");
      }
    }
  }
  
  // Move um andar em direcao ao destino.
  private void moveOn(){
    if(andar_destino > andar_atual){
      escritor.println("Subindo do andar " + andar_atual + " para o andar " + (andar_atual + 1) + "." );
      andar_atual++;
    }
    else if (andar_destino < andar_atual){
      escritor.println("Descendo do andar " + andar_atual + " para o andar " + (andar_atual - 1) + "." );
      andar_atual--;
    }
  }

  //Procura por novas requisicoes e reserva elas.
  private void searchForRequisitions(){
    requisicoes = monitor.reserveNearestRequests(andar_atual, capacidade);
    if(requisicoes.size()>0){
      escritor.println( "Reservada as requisicoes para este elevador. Indo ao andar " + requisicoes.get(0).getOrigem() + " busca-las." );
    }
  }
  
  //Funcao pra valor absoluto...
  int abs(int n){
    if(n<0)return -n;
    return n;
  }

  
}
