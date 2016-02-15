
// Classe das Requisição feita por um usuário do elevador
class Requisicao {
  private static int open_id = 0;
  private int id;
  private int destino;
  private int origem;

  public Requisicao(int origem, int destino)
  {
    this.id = open_id++;
    this.destino = destino;
    this.origem = origem;
  }

  public int getDestino()
  {
    return this.destino;
  }

  public int getId()
  {
    return this.id;
  }

  public int getOrigem()
  {
    return this.origem;
  }


}