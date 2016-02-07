import java.io.*;
import java.lang.String;
import java.util.*;

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

class Teste {

  private int numero_andares,           // Número de andares
              numero_elevadores,        // Número de elevadores do prédio
              capacidade_elevador;      // Capacidade dos elevadores

  private ArrayRequisicoes<Integer> requisicoes;

  public static void main(String args[]){
  
    if(args.length < 1)
      System.out.println("Sem argumentos");

    Teste teste = new Teste();
    teste.requisicoes = new ArrayRequisicoes<Integer>();

    if(!teste.leEntrada(args)){
      System.out.println("oie");
    }

    System.out.println(teste.requisicoes);

  }

  public boolean leEntrada(String[] args){
    if(args.length != 1){
      System.out.println("Erro: entrada inválida!\nTente: java <nome do programa> <nome do arquivo de dados>");
      return false;
    }

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

}