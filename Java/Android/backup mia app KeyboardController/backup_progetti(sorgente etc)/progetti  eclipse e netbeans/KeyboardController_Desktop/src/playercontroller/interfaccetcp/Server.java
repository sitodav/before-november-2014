
package playercontroller.interfaccetcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;

//Questo server fa da ponte tra un utilizzatore di questa classe (ad es un'altra classe java) ed un client dall'altro lato.
//Questo server puo' servire un solo utente alla volta, (non si rimette in listen dopo aver accettato la connessione) e se il client si disconnette, va riavviato.
//L'oggetto server (che gira nel thread di dove è stato allocato l'oggetto) si interfaccia con  chi usa l'oggetto, mentre un suo thread secondario gestisce il client dall'altro lato.
//Chi si interfaccia con questo server adotta questo comportamento:
/*- crea un thread concorrente, crea oggetto server:
 *      controlla che sia stato creato correttamente (lo status) e se si va avanti
 * in un while infinito:
 * -ogni volta che vuole vedere cosa il client dall'altra parte aveva mandato controlla:
 *      se il server è ancora valido (lo status)
 *      in caso affermativo, lancia il metodo extractData. 
 *              controlla che non venga ritornato null, in tal caso il client dall'altra parte non ha inviato nulla
 *chiudi poi tutto col metodo apposito stopEPulisci
 */


public class Server {
    public ServerSocket mySocket; //socket su cui si binda e si mette in ascolto. Publico per...
    private boolean validServer; //var booleana che indica se c'e' stato qualche errore ed il server è inutilizzabile
    private String statusMessage; //messaggio che indica l'errore o altro
    private Server.ClientHandlerReader  clientHandlerR;
    private Thread threadPerClient; //il thread che gestisce il client
    private Vector<String> messaggiDaThreadClient; //la struttura sulla quale il thread del client gira i messaggi al main thread. I vector sono sincronizzati automaticamente
    public boolean chiudiForzatamente=false;
    
    public Server(int portToBind){ 
        validServer=false;
        statusMessage="Non Connesso";
        messaggiDaThreadClient=new Vector<String>();
        
        try {
            mySocket=new ServerSocket(portToBind);
            mySocket.setSoTimeout(20000);
            Socket clisock=mySocket.accept();
            clientHandlerR=new Server.ClientHandlerReader(clisock);
            threadPerClient=new Thread(clientHandlerR);
            threadPerClient.start();
        }
        catch (IOException ex) {
            
            validServer=false;
            statusMessage="Errore: (main thread)>impossibile bindare il socket/timeout attesa richiesta client/client connesso ma errore sul conn socket"+"\n"+ex.getMessage();
            stopEpulisciSrv();
        }
        
    }
    
    
    
    public boolean sendData(String data){
        if(!validServer)
            return false;
        clientHandlerR.outputStream.println(data);
        return true;
    }
    
    public String extractData(){ //la usa chi si interfaccia al server
        if(messaggiDaThreadClient.isEmpty())
            return null;
        return messaggiDaThreadClient.remove(0);
        
        
    } 
   
    public boolean getStatusServer(){
        return validServer;
    } 
    
    public String getMessageError(){
        return statusMessage;
    } 
    
    public void stopEpulisciSrv(){
        try {
            if(mySocket!=null)
                mySocket.close();
            if(threadPerClient!=null)
                threadPerClient.interrupt();
        } catch (IOException ex) {
            Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    
    private class ClientHandlerReader implements Runnable{
        private String rcvBuff;
        public Socket clientSocket;
        public PrintWriter outputStream;
        public BufferedReader inputStream;
        
        public ClientHandlerReader(Socket clientSocket) throws IOException{
            System.out.println("client connesso");
            this.clientSocket=clientSocket;
            outputStream=new PrintWriter(this.clientSocket.getOutputStream(),true);
            inputStream=new BufferedReader(new InputStreamReader(this.clientSocket.getInputStream()));
            validServer=true;
        }
        
        
        @Override
        public void run() {
            while(!Thread.interrupted()){
                try {
                    rcvBuff=inputStream.readLine(); //il thread del client legge dal socket del client, e mette sul vector dove puoi prenderseli il main thread
                    if(rcvBuff==null) //il client ha chiuso la connessione con FIN
                        break;
                    messaggiDaThreadClient.add(rcvBuff);
                    
                } catch (IOException ex) {
                    validServer=false;
                    statusMessage="Errore: (thread client)>Impossibile leggere dal socket"+"\n"+ex.getMessage();
                    pulisci();
                    return;
                }
            }
            validServer=false;
            statusMessage="Il client ha chiuso la connessione ";
            pulisci();

        }
        
        public void pulisci(){
            try {
                if(clientSocket!=null)
                    clientSocket.close();
                if(outputStream!=null)
                    outputStream.close();
                if(inputStream!=null)
                    inputStream.close();
            } catch (IOException ex) {
                Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    

}
