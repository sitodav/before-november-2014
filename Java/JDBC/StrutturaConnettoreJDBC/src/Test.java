import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;



class Connettore{
	//classe che gestisce la connessione alla BD tramite JDBC
	//la connessione viene aperta prima di qualunque op sulla base dati
	//e viene richiusa alla fine di ogni op sulla base dati
	
	private static final String JDBC_DRIVER="oracle.jdbc.OracleDriver";
	private static final String DB_URL="jdbc:oracle:thin:@192.168.1.64:1521:molotv-774d4308";

	private String usr;
	private String pw;
	
	Connection conn1;
	Statement st;
	PreparedStatement pst;
	ResultSet rs;
	
	//i metodi connetti e disconnetti non sono chiamati da chi usa il connettore
	//ma da ciascun metodo di lavoro sulla base di dati, all'inizio e alla fine 
	private void connetti() throws ClassNotFoundException,SQLException{
		
			Class.forName(JDBC_DRIVER);
			conn1=DriverManager.getConnection(DB_URL,usr,pw);
			System.out.println("CONNESSO!");
		
	}
	
	private void disconnetti() throws SQLException{
		
			if(conn1!=null)
				conn1.close();
		
	}
	
	public Connettore(String usr,String pw){
		this.usr=usr; this.pw=pw;
	}
	
	public void esempioSelectConStatement(){
		try{
			//metto tutto nello stesso tryu perchè voglio che se qualcosa solleva eccezione, le istruzioni successive (Quindi tutto il resto del try)
			//non vengano eseguite
			connetti(); //MI COLLEGO ALLA BD (puo' lanciare ClassNotFound o SQLException da progettazione)
			
			st=conn1.createStatement(); //puo' dare SQLException
			rs=st.executeQuery("SELECT * FROM UTENTI"); //puo dare SQLException
			int ncolon= rs.getMetaData().getColumnCount();
			String t;
			while(rs.next()){
				t=new String("");
				for(int i=1;i<=ncolon;i++){ //indici ORACLE partono da 1
					t+=rs.getString(i);
					t+="  ";
				}
				System.out.println("tupla ritornata> "+t);
			}
		}
		catch(ClassNotFoundException e){
			e.printStackTrace();
		}
		catch(SQLException e){
			e.printStackTrace();
		}
		finally{
			//il motivo per cui (anche se disconnetti() st.close() e rs.close() possono sollevare tutti e 3 SQLException)
			//uso 3 try diversi, è che se la prima (o la seconda) danno eccezione, va nel catch, e smette di fare il try dove è stata
			//sollevata l'eccezione, quindi se io mettessi tutte nello stesso try le chiusure, se la prima (o seconda) sollevano eccezione,
			//le successive non avvengono. Invece io voglio che anche se la disconnetti() ritorna eccezione, venga comunque tentata la st.close()
			try{
				disconnetti();
			}
				catch(SQLException e){e.printStackTrace();}
			
			
			try{
				if(st!=null)
					st.close();
			}
				catch(SQLException e){e.printStackTrace();}
			
			
			try{
				if(rs!=null)
					rs.close();
			}
				catch(SQLException e){e.printStackTrace();}
		}
		
		
	}
	
	public void esempioSelectConPreparedStatement(String nome,String cognome){
		try{
			connetti();
			pst=conn1.prepareStatement("select id from utenti where nome=? and cognome=?");
			pst.setString(1, nome);
			pst.setString(2, cognome);
			rs=pst.executeQuery();
			
			while(rs.next())
				System.out.println("Id ritornato> "+rs.getString(1));
			
		}
		catch(ClassNotFoundException e){
			e.printStackTrace();
		}
		catch(SQLException e){
			e.printStackTrace();
		}
		finally{
			try{disconnetti();} 
				catch(SQLException e){e.printStackTrace();}
			
			try{
				if(rs!=null)
					rs.close();
			}
				catch(SQLException e){e.printStackTrace();}
			
			try{
				if(pst!=null)
					pst.close();
			}
				catch(SQLException e){e.printStackTrace();}
		}
		
		
		
	}
		
	
	
}
