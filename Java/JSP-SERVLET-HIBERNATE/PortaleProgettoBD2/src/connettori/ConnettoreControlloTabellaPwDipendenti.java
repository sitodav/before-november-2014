package connettori;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class ConnettoreControlloTabellaPwDipendenti extends ConnettoreBaseDati{
	
	public boolean controllaMatchPwDipendente(String cf,String pw){ //funzione che ritorna true se cf//pw in input esistono nella tabella password_dipendenti della base dati
		/*Questo oggetto si deve connettere alla base di dati usando l'utente validatore_pass_dipendenti/validatore_pass_dipendenti
		 * e controllare che nella tabella password_dipendenti esista un match con i parametri in input 
		 * Siccome si presuppone che validatore_pass_dipendenti/validatore_pass_dipendenti sia un user la cui password conosce
		 * chi ha progettato la bd (e le classi che permettono di interfacciarla al portale web, tra cui questa, è più naturale 
		 * pensare che vada messa la pw dell'user qui, piuttosto che farla conoscere agli sviluppatori della web app che devono 
		 * passarla dall'alto*/
		 boolean result=false;
		 connetti("validatore_pass_dipendenti","validatore_pass_dipendenti"); //metodo che eredita da connettorebasedati
		 if(connected){ //se connetti non è andato a buon fine, connected starà a false
			 //se connetti è andato a buon fine, l'oggetto ereditato, conn1 manterrà connessione con bd
			 try {
				PreparedStatement pst=conn1.prepareStatement("select password from dagull.password_dipendenti where lower(cf)=? and dagull.password_dipendenti.valida = 1");
				//ovviamente se la password è scaduta, l'account del dipendente viene considerato invalido !
				pst.setString(1, cf);
				ResultSet rs=pst.executeQuery();
				//siccome cf è univoco, al più ci sarà un solo record nel result set. Quindi
				if(rs.next() && rs.getString(1).equals(pw)) //se c'e' almeno un risultato (quindi next() da true) e questa stringa è = pw in input
					result=true; //match trovato!
			 } 
			 catch (SQLException e) {
				 System.out.println("Errore sulla creazione del prepared statement/esecuzione query");
			 }
		 }
		 disconnetti();
		 return result;
		
			
	}
}
