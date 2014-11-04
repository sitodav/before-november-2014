package connettori;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class ConnettoreControlloLogUserDb extends ConnettoreBaseDati{
	//questa classe serve per eseguire operazioni sulla base dati connettendoci come utente della base dati, con user e pass
	//presi in input (user e pass di utente BASI DI DATI)
	
	//questa classe eredita i metodi connect e disconnect dalla classe madre
	
	public boolean checkUserPw(String usr,String pw,String ruolo){ //questo metodo controlla che l'utente definito da usr//pw nella base dati
		//esista e che il ruolo da lui scelto corrisponda a quello assegnatogli nella BD. 
		//In tal caso ritorna true.
		
		boolean res=false;
		
		//tentiamo quindi prima un'apertura di connessione usando come user della bd quelli presi in input
		connetti(usr,pw); 
		if(connected){//è true se la connessione alla base dati, con user/pw presi in input, è andata a buon fine (quindi se il flag ereditato è a true)
			disconnetti(); 
			//a questo punto dobbiamo connetterci con l'account di dba e vedere se il ruolo che l'utente ha fornito, fa match
			connetti("dagull","dagull");
			if(connected){ //è false solo se ci sono stati problemi con la base dati
				try {
					//controllo se l'user (che risulta esistere nella BD ed avere la pw fornita) ha anche come ruolo quello fornito
					PreparedStatement pst=conn1.prepareStatement("select granted_role from dba_role_privs where grantee=?");
					pst.setString(1, usr.toUpperCase());
					ResultSet rs=pst.executeQuery();
					while(rs.next()){
						if(ruolo.equalsIgnoreCase(rs.getString(1))){ //il ruolo allora fa match !
							res=true;
							break;
						}
					}
					
				} catch (SQLException e) {
					//e.printStackTrace();
					System.out.println("Errore sulla creazione del prepared statement/esecuzione query");
				}
				if(!res) //se qui res è ancora false, vuol dire che pur esistendo quell'utente con quella pass, il ruolo fornito non fa match
					System.out.println("User e Pw validi a livello del DB, ma ruolo non trovato!");
				//arrivati qui sicuramente siamo connessi con l'user dagull/dagull
				disconnetti();
			}
		}
		else{
			//arrivati qui vuol dire che l'user/pass forniti, non risultano essere un utente (con quella pw) a livello del DB
			System.out.println("User/Pass non trovati per utente a livello BD");
		}
		
		return res;
	}

}
