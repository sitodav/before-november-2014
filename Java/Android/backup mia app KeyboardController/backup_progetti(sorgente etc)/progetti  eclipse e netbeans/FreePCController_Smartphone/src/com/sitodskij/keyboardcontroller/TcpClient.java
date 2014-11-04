package com.sitodskij.keyboardcontroller;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Vector;

import android.util.Log;
import com.sitodskij.freepccontroller.R;



//Questa classe fa da ponte tra un utilizzatore della classe (un activity android) ed un server dall'altro lato. 
//attraverso i metodi dell'interfaccia pubblica della classe.
//L'oggetto principale (TcpClient) gira sul thread di chi alloca l'oggetto (quindi l'activity di Android), ed interagisce con questa.
//Prende inoltre i messaggi che l'activity vuole inviare al server e li invia (non ha bisogno di un thread secondario per fare questo in quanto Android permette che l'activity
//invii su socket).
//Viene usato un thread secondario che si occupa di inizializzare in background e connettere il socket al server (e poi il thread in questione termina)
//Viene usato un ulteriore thread che si occupa di stare sempre in ascolto (in background) sul socket, mettendo i messaggi ricevuti dal server su un vector, dal quale
//il thread dell'oggetto principale puo' prenderli per girarli all'activity che li richiede.
//Il thread che inizializza il socket si blocca fino a che il server non accetta la sua connessione. Solo quando questo ha terminato, prima di morire lancia
//il secondo thread (quello che puo' ricevere dal server). Tuttavia il main thread (l'activity che istanzia l'oggetto TcpClient) ritorna subito, quindi non è detto che
//quando l'oggetto TcpClient è creato, sia subito utilizzabile, e infatti bisogna guardare il valore di ritorno dei metodi chiamati
//L'activity che fa uso di questa classe TcpClient adotta il seguente comportamento:
/*
 * -Crea l'oggetto TcpClient passando IP e PORTA server : quando ritorna non è detto che il client sia connesso.
 * - prima di leggere/scrivere, assicurarsi che il client sia riuscito a connettersi (tcpclient.statusValid=true). Altrimenti aspettare
 * 
 * -quando il client si è riuscito a connettere (tcpClient.statusVaild=true)...
 * 
 * -Se l'activity vuole leggere quanto il server ha inviato:
 * 			-lancia il metodo extractData() dell'oggetto TcpClient.
 * 			-Se la stringa è non nulla, prosegue.
 * 			-Se la stringa è nulla:
 * 				-controlla lo stato di validità dell'oggetto TcpClient lanciando il suo metodo getStatus():
 * 						-se lo stato è valido, allora semplicemente il server non ci ha inviato dati nuovi, e proseguiamo.
 * 						-se lo stato è invalido, il server ci ha disconnessi (disconnessione pulita). Si deve fermare.
 * 
 * -Se l'activity vuole inviare al server dei dati:
 * 			-lancia il metodo sendData(STRINGA_DA_INVIARE) e controlla il valore booleano di ritorno:
 * 					-se è falso allora qualcosa è successo, e controlliamo il messaggio di stato con getStatusMessage().
 * 					-se è vero, proseguiamo.
 */

public class TcpClient {
	private String ipServer;
	private int portServer;
	private Socket mySocket;
	private boolean statusValid;
	private String statusMessage;
	private BufferedReader inputStream;
	private PrintWriter outputStream;
	private Vector<String> messaggiDaSocketReader;
	Thread thread_per_socketInitializer,thread_per_socketReader;
	
	
	
	public TcpClient(String ipServer,int portServer){ 
		statusValid=false;
		statusMessage=">>Socket non ancora connesso al server";
		this.ipServer=ipServer;
		this.portServer=portServer;
		messaggiDaSocketReader=new Vector<String>();
		
		thread_per_socketReader=new Thread(new SocketReader()); //lo creo soltanto, a lanciarlo sarà il primo thread quando avrà finito
		thread_per_socketInitializer=new Thread(new SocketInitializer()); //avvio il thread che iniziailzza il socket in background connettendolo al server
		thread_per_socketInitializer.start();
		
	}
	
	public boolean getStatus(){
		return statusValid;
	}
	public String getStatusMessage(){
		return statusMessage;
	}
	
	public boolean sendData(String data){ //ritorna il valore booleano a seconda che l'op sia riuscita o meno
	//chiamando questo metodo, lo si chiama nel thread dell'activity che ha istanziato questo oggetto. Siccome è un'op di send (non blocca) si puo' fare anche se quindi quest'oggetto è 
	//istanziato nell'activity di android.
	//(prima di lanciarlo pero', controlliamo lo stato dell'oggetto socket di classe TcpClient, poichè se lo stato è invalido non possiamo usare questo metodo)
		
		if(!statusValid){
			return false;
		}
		outputStream.println(data);
		return true;
	}
	
	public String extractData(){ 
		if(messaggiDaSocketReader.isEmpty())
			return null;

		return messaggiDaSocketReader.remove(0);
	}
	
	private class SocketInitializer implements Runnable{ //questa classe che gira come thread serve perchè il main thread in android non puo' fare alcune op bloccanti
	//questa classe accede ed inizializza il socket privato (e gli stream associati) della classe TcpClient, usando ipServer e portServer
		@Override
		public void run() {
			try {
				mySocket=new Socket(ipServer,portServer);
				//sta bloccato qui finchè non riesce a connettersi. Ma nb il main thread intanto pero' già ritornato dal costruttore. Quindi
				//quando si istanzia l'oggetto TcpClient, il fatto che si ritorni dal costruttore non vuol dire che il socket sia connesso al server!!
				//Bisogna invece aspettare che lo stato diventi valid!
				inputStream=new BufferedReader(new InputStreamReader(mySocket.getInputStream()));
				outputStream=new PrintWriter(mySocket.getOutputStream(),true);
				statusValid=true;
				thread_per_socketReader.start(); //avvio il thread che sta costantemente in ascolto in lettura sul socket e mette quanto letto sul vector
				
			} catch (UnknownHostException e) {
				statusMessage="Errore:(SocketInitializer thread)>Host sconosciuto \n"+e.getMessage();
				Log.e("MAIN_ACTIVITY",getStatusMessage());
				pulisci();
				return;
			} 
			  catch (IOException e) {
				statusMessage="Errore:(SocketInitializer thread)>I/O Exception \n"+e.getMessage();
				Log.e("MAIN_ACTIVITY",getStatusMessage());
				pulisci();
				return;
			}
			
			return;//il thread muore in ogni caso dopo 
			
		}
	}
	
	private class SocketReader implements Runnable{
		
		@Override
		public void run() {
			String rcvBuff;
			//questo thread deve essere sempre in esecuzione(legge in continuazione dal socket e mette sulla coda dei messaggi) 
			//a meno che qualche altra cosa non abbia impostato il valore del socket a disconnesso, oppure gli sia richiesto di interrompersi, o avviene un errore
			//causato da lui (in tal caso imposta lui i flag e gli errori).
			
			
			while(statusValid && !Thread.interrupted()){
				try {
					rcvBuff=inputStream.readLine();
					if(rcvBuff==null){ //il server si è disconnesso,
						statusMessage="(SocketReader thread)>Il server ci ha disconnessi (chiusura pulita)";
						statusValid=false;
					}
					//altrimenti metto il messaggio ricevuto dal server sulla coda e ricomincio il mio while
					messaggiDaSocketReader.add(rcvBuff);
					
				} catch (IOException e) {
					statusMessage="Errore:(SocketReader thread)>Errore I/O \n"+e.getMessage();
					Log.e("MAIN_ACTIVITY",getStatusMessage());
					statusValid=false;
				}
			}
			pulisci();
			
		}
		
	}
	
	
	public void chiudiTutto(){
		if(thread_per_socketInitializer!=null)
			thread_per_socketInitializer.interrupt();
		if(thread_per_socketReader!=null)
			thread_per_socketReader.interrupt();
	}
	
	private void pulisci(){
		
			try {
				if(mySocket!=null)
					mySocket.close();
				if(outputStream!=null)
					outputStream.close();
				if(inputStream!=null)
					inputStream.close();
			} catch (IOException e) {
				statusMessage="Errore:(SocketReader thread)>Errore I/O nella chiusura degli stream rimasti aperti \n"+e.getMessage();
				Log.e("MAIN_ACTIVITY",getStatusMessage());
			}
	}
	
}
