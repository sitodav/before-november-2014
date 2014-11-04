package servlets;

import java.io.IOException;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import connettori.*;

/*Questa servlet, viene chiamata quando qualcuno sta cercando di fare il login nella homepage (parametri ricevuti in post).
 * A seconda del valore scelto per il campo "ruolo", prevede un diverso iter di identificazione (si legga avanti)
 * 
 */
public class ValidatriceLogin extends HttpServlet {
	
	private ConnettoreControlloTabellaPwDipendenti connettorePerDipendenti; //usato per validare i dipendenti
	private ConnettoreControlloLogUserDb connettorePerUser; //usato per validare gli user base dati
	
	@Override
	public void init() throws ServletException { //lanciata solo la prima volta che viene richiamata la servlet
		super.init();
		connettorePerDipendenti=new ConnettoreControlloTabellaPwDipendenti();
		connettorePerUser=new ConnettoreControlloLogUserDb();
	}
	
	//Innanzitutto facciamo override del metodo doGet, che viene chiamato quando qualcuno accede in get all'url su cui è mappata questa servlet.
	//Siccome questo è un comportamento scorretto poichè questa servlet è chiamata solo in post, l'accesso in get provoca il redirect alla home
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
				resp.sendRedirect(req.getContextPath()); //rimanda alla homepage (aggiornando url)
	}
	
	//questo metodo viene chiamato quando si accede all'url su cui è mappato la servlet, in post
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		
			String username,password,ruolo;
			//i valori immessi nel form della home page per il login, e passati in post, sono dei parametri sull'oggetto request
			username=req.getParameter("username").toLowerCase();
			password=req.getParameter("password");
			ruolo=req.getParameter("ruolo");
			
			if(username==null || password==null || ruolo==null)
				resp.sendRedirect(req.getContextPath()); //rimanda alla home page
			
			//ora a seconda che il ruolo scelto sia dipendente, o uno degli altri, dobbiamo prevedere un diverso iter di validazione
			boolean res=false; //flag usato per evitare duplicazione codice
			if(ruolo.equals("dipendente")){
				//nel caso un dipendente si vuole connettere, il suo username è il suo cf, e siccome questo è un dato inserito in una
				//tabella della base di dati, lo si vuole "sanitarizzare" e far si' che sia case insensitive (poichè il codice fiscale
				//è una cosa comune, non ha senso distinguere tra cf maiuscolo o minuscolo)
				username=username.toLowerCase(); //e renderemo minuscoli anche i valori estratti dalla bd in fase di validazione
				if(connettorePerDipendenti.controllaMatchPwDipendente(username, password))
					//ritorna true se il connettore ha trovato, connettendosi alla bd con un user di servizio, match nella tabella delle pw dei dipendenti
					//impostiamo quindi il flag
					res=true;		
				
			}
			else{
				
				//in tal caso il client ha scelto un ruolo che indica che è presente come user della base di dati
				//quindi lanciamo metodo, del connettore apposito, che controlla che davvero esista un user nella base dati (provando a connettersi)
				//e se esiste, che sia davvero di quel ruolo che il client ha indicato
				//in tal caso, username e pass forniti sono quelli di accesso alla base di dati, ed è naturale considerare che
				//debbano essere case sensitive (a differenza del cf che è l'username dei dipendenti che si vogliono connettere al portale)
				if(connettorePerUser.checkUserPw(username, password, ruolo))
					//allora esiste un user della basedati con quell'username/password e che ha quel ruolo, e setto il flag
					res=true;

			}
			if(res){ //in entrambi i casi (dipendente o altri ruoli) se il match è positivo, imposto come attributo sessione i seguenti
				req.getSession().setAttribute("ruolo",ruolo); //ritornando alla homepage ora, il client risulterà loggato
				//volendo impostare come parametri di session, al client connesso, se questo è un dipendente gli impostiamo dipendente/dipendente
				//come parametri di accesso alla base dati, altrimenti se il client ha un altro ruolo, quindi risulta avere accesso alla bd con un proprio
				//username e una propria password, gli settiamo quelli
				if(ruolo.equals("dipendente")){
					req.getSession().setAttribute("username","dipendente");
					req.getSession().setAttribute("password","dipendente");
				}
				else{
					req.getSession().setAttribute("username",username);
					req.getSession().setAttribute("password",password);
				}
			}
			else{
				//allora imposto il messaggio di servizio che indica che il login è fallito
				req.getSession().setAttribute("messaggio_servizio", new String("Log in fallito. Ricontrolla i dati (o che la password, se dipendente, non sia scaduta)."));
			}
			//in ogni caso (match trovato o non trovato) si torna alla homepage
			resp.sendRedirect(req.getContextPath());
	}
}
