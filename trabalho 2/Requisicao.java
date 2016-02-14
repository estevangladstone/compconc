
// Classe das Requisição feita por um usuário do elevador
class Requisicao {
  
  private int id;
  private int destino;

  public Requisicao(int id, int destino)
  {
    this.id = id;
    this.destino = destino;
  }

  public int getDestino()
  {
    return this.destino;
  }

  public int getId()
  {
    return this.id;
  }

}