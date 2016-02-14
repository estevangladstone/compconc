import java.util.*;

// Array de duas dimensões para guardar as requisições
class Andar {

  private ArrayList<Requisicao> requisicoes;  // Requisicoes no andar
  private boolean status;                     // Se um elevador já irá atender esse andar  

  // Construtor
  public Andar(){
    requisicoes = new ArrayList<Requisicao>();
  }

  // Inclui uma requisição no andar
  public void adicionarRequisicao(Requisicao item) {
    requisicoes.add(item);
  }

  // Retira e retorna um subarray com até 'capacidade' requisições
  public Requisicao[] retirarRequisicoes(int quantidade){
    Requisicao[] sublista = new Requisicao[1];

    if(requisicoes.size() < quantidade){
      sublista = requisicoes.toArray(sublista);
      requisicoes.clear();
    }
    else{
      sublista = (requisicoes.subList(0, quantidade)).toArray(sublista);
      requisicoes.removeAll(new ArrayList<Requisicao>(requisicoes.subList(0, quantidade)));
    }

    return sublista;
  }

  // Retorna a quantidade de requisições no andar
  public int quantidadeRequisicoes(){
    return requisicoes.size();
  }

  // Retorna de o andar está livre ou não
  public boolean livre(){
    return status;
  }

  // Marca o andar como livre ou ocupado
  public void setStatus(boolean status){
    this.status = status;
  }

}

