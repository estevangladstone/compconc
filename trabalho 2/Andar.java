import java.util.*;

// Array de duas dimensões para guardar as requisições
class Andar {

  private int						numero_do_andar;
  private ArrayList<Requisicao> 	requisicoes;  	// Requisicoes no andar

  // Construtor
  public Andar(){
    requisicoes = new ArrayList<Requisicao>();
  }
  
  public void setAndar(int n){
	numero_do_andar = n;
  }

  public int getAndar(){
	return numero_do_andar;
  }
  
  // Inclui uma requisição no andar
  public void adicionarRequisicao(Requisicao item) {
    requisicoes.add(item);
  }

  // Retira e retorna um subarray com até 'capacidade' requisições, otimizando ja (vai soh subir ou soh descer,prioridade primeiro pra quantidade e dps pra quem vai mais longe)
  public ArrayList<Requisicao> retirarRequisicoes(int quantidade){
    ArrayList<Requisicao> sublista = new ArrayList<Requisicao>();

	int vy = 0 ; 											// Vai ser -1 se o elevador decidir descer e +1 se decidir subir.
	int querendo_subir = 0 , querendo_descer = 0;			//Quantidade de gente querendo subir ou descer;
	int soma_delta_subindo = 0 , soma_delta_descendo = 0; 	//Valor das somas das distancias que sera percorrida subindo.
	
	for(Requisicao req : requisicoes){
		if(req.getDestino() > numero_do_andar){
			querendo_subir++;
			soma_delta_subindo += ( req.getDestino() - numero_do_andar );
		}
		else if (req.getDestino() < numero_do_andar){
			querendo_descer--;
			soma_delta_descendo += ( numero_do_andar - req.getDestino() );
		}
	}

	if(querendo_subir > querendo_descer)vy=1;
	if(querendo_subir < querendo_descer)vy=-1;
	if(querendo_subir == querendo_descer){
		if(soma_delta_descendo>soma_delta_subindo){
			vy=1;
		}else{
			vy=-1;
		}
	}
    
	while( sublista.size()<quantidade || quantidadeRequisicoes(vy) >0 ){
		Requisicao req = null; //A proxima requisicao a adicionar.
		
		for(Requisicao it : requisicoes){
			if( vy == 1 && it.getDestino() > numero_do_andar ){
				req = it;
				break;
			}
			if( vy == -1 && it.getDestino() < numero_do_andar ){
				req = it;
				break;
			}
		}
		if( req != null){
			sublista.add(req);
			requisicoes.remove(req);
		}else{
			break;
		}
	}
	
	return sublista;
  }

  // Retorna a quantidade de requisições no andar. Se v for 1, retorna as quantidades de requisicoes querendo subir, se for -1, descendo.
  public int quantidadeRequisicoes( int v ){
	int n = 0;
    if(v==0){
		return requisicoes.size();
	}
	for(Requisicao req : requisicoes){
		if(req.getDestino() > numero_do_andar && v == +1)n++;
		if(req.getDestino() < numero_do_andar && v == -1)n--;
	}
	return n;
  }

}

