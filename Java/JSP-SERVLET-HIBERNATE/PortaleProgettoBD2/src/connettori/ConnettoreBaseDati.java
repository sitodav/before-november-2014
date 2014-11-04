//NB: questa classe da delle funzioni generiche di connessione alla base di dati. Poi si creano dei connettori specifici
//che sono classi figlie di questa, con funzionalità ad hoc di interrogazione della base dati

package connettori;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class ConnettoreBaseDati {
/*Questa classe incapsula i metodi per accedere alla base di dati. Rappresenta un livello intermedio tra le classi (ad es le
 * servlet) che gestiscono la logica della web application, e la base dati. Questa classe accede alla base dati tramite jdbc.
 * In caso di errori, i metodi gestiscono internamente le eccezioni ed impostano un flag booleano presente nella classe.
 * L'utilizzatore della classe utilizzerà quindi questo flag per controllare lo stato delle operazioni richieste.
*/
	private static final String JDBC_DRIVER="oracle.jdbc.OracleDriver";
	private static final String DB_URL="jdbc:oracle:thin:@localhost:1521:XE";
	protected Connection conn1=null;
	protected boolean connected=false;
	
	protected void connetti(String usr,String pw){
		//i parametri in input, usr e pw, devono essere quelli di un user della base dati
		try{
			Class.forName(JDBC_DRIVER);
			conn1=DriverManager.getConnection(DB_URL,usr,pw);
			connected=true;
		}
		catch (ClassNotFoundException ex){
			System.out.println("Errore caricamento driver");
		}
		catch (SQLException ex){
			System.out.println("Errore di connessione");
		}
	}
	
	protected void disconnetti(){
		try{if(conn1!=null && connected)
			conn1.close();
			connected=false;
		}
		catch(SQLException ex){
			System.out.println("Errore chiusura connessione");
		}
	}
	
}
