/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package playercontroller;


import java.net.InetAddress;
import playercontroller.segnalatori.KeyboardWindows;
import playercontroller.segnalatori.SegnalatoreGenerico;
import playercontroller.interfaccetcp.Server;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.animation.FillTransition;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.control.Tooltip;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;


/**
 *
 * @author sitodskij
 */
public class ControllerView {
    MainApp mainClass;
    private int portaSceltaPerServer;
    private Class classeSegnalatoreScelto;
    private double xOff,yOff;
    private GestoreAsincrono gest;
    private Thread threadPerGest;
    private final String PROTOCOLLO_RESYNC_REQUEST_DA_CLIENT="***RESYNC***";
    private final String PROTOCOLLO_SCELTA_SEGNALATORE_WINDOWS="***WINDOWSKEYBOARD";
    private Handler handler; //lo uso per far fare delle operazioni dal main thread, quando lo vuole il thread Gestore
    private Tooltip tooltipInfoOnRefresh;
    private String myBindedIp;
    //pannello principale
    @FXML 
    ImageView spostaImg;
    @FXML
    Rectangle spostaRect;
    @FXML 
    ImageView broadcastingImg;
    @FXML
    Rectangle broadcastingRectangle;
    @FXML
    ImageView refreshImg;
    boolean threadInAttesaServerAccept=false;
    @FXML
    Rectangle refreshRect;
    @FXML
    ImageView settingImg;
    @FXML
    Rectangle settingRect;
    boolean settingMenuOpened=false;
    @FXML
    ImageView warningImg;
    @FXML
    Rectangle warningRect;
    boolean warningMenuOpened=false;
    @FXML
    ImageView exitImg;
    @FXML
    Rectangle exitRect;
    
    ////pannello voce setting
    @FXML
    AnchorPane settingAnchorPane;
    @FXML
    Rectangle confirmSettingRect;
    @FXML
    ImageView confirmSettingImg;
    @FXML
    TextField textPortSetting;
    @FXML
    Label infoIp;
    
    
    ///pannello voce warning
    @FXML
    AnchorPane warningAnchorPane;
    @FXML
    Rectangle confirmWarningRect;
    @FXML
    ImageView confirmWarningImg;
    @FXML
    TextField textMessageWarning;
    
    //un'animazione che mi serve tenere globale
    Timeline animazioneRotazione1; 
    Timeline animazioneOpacityBroadcastingRect;
    
    public void initialize(){
        try{myBindedIp=InetAddress.getLocalHost().getHostAddress();} catch(Exception ex){/*TO-DO COSA FARE ?*/}
        //all'avvio, i menu a scomparsa sono contratti su se' stessi (nascosti quindi)
        settingAnchorPane.scaleXProperty().setValue(0.0); 
        warningAnchorPane.scaleXProperty().setValue(0.0); 
        //ed è ad opacità 0 il broadcasting rectangle perchè non siamo connessi
        broadcastingRectangle.opacityProperty().setValue(0.0);
        ///////////
        portaSceltaPerServer=4444;
        classeSegnalatoreScelto=null;; //non viene settato nulla all'inizio
        ///////////
        //i valori correntemente ATTIVI dei campi dei pannelli a scomparsa sono caricati quando i pannelli vengono aperti, in base ai valori che stanno nelle var corrispondenti
       
        //questa animazione la creo qui, ma viene avviata nel gestore
        animazioneOpacityBroadcastingRect=creaAnimazioneOpacitaRettangoloTastoCliccato(broadcastingRectangle); //in realtà non si avvia pero' quando il rect è cliccato, ma per i dati validi ricevuti
        
        //definisco e registro gli eevnti sui click delle immagini
        //e definisco per ogni tasto anche le animazioni
        //NB LE ANIMAZIONI ANIMANO LE ICONE MA SONO ATTIVATE PREMENDO SUI QUADRATI ATTORNO (E DIETRO POICHE' LE ICONE SONO INVISIBILI AGLI EVENTI MOUSE)
        //per evitare che un'animazione sia ripetuta 2 volte, uso booleani per ogni elemento del menu
        
        
        //TASTO PRINCIPALE DI REFRESH
        final Timeline animazione=creaAnimazioneImmagineTastoCliccato(refreshImg);
        final Timeline animazioneOpacitaRettangolo1=creaAnimazioneOpacitaRettangoloTastoCliccato(refreshRect);
        animazioneRotazione1=creaAnimazioneImmagineCheGira(refreshImg);
       //setto il tooltip all'avvio, ma dovro' reistallarne uno nuovo (poichè non si aggiorna da solo) se cambia la porta
        tooltipInfoOnRefresh=new Tooltip("listen on \nip: "+myBindedIp+"\nport :"+String.valueOf(portaSceltaPerServer));
        
        tooltipInfoOnRefresh.getStyleClass().add("ttip");
        Tooltip.install(refreshRect, tooltipInfoOnRefresh); 
        
        refreshRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                if(threadInAttesaServerAccept){ //avevo già cliccato sul refresh, devo aspettare che vada in timeout o arrivi un client per poter ripremere bottone
                    return;
                    
                }
                else{ 
                    //se il warning rect è rosso, devo farlo tornare blu. Pero' non posso saperlo, quindi ottengo il colore suo attuale, e lo uso come punto di partenza verso il blu
                    Color actual=(Color) warningRect.getFill();
                    (new FillTransition(Duration.millis(500),warningRect,actual,Color.DODGERBLUE)).play();
                    //poi avvio le altre animazioni
                    animazione.play(); 
                    animazioneOpacitaRettangolo1.play();
                    
                    if(threadPerGest!=null){
                        threadPerGest.interrupt();
                        
                    }
                    gest=new GestoreAsincrono();
                    threadPerGest=new Thread(gest);
                    threadPerGest.start();
                    
                }
          }
            
        });
        
        ////////////////////////////////////////////////
        //TASTO PRINCIPALE DI SETTING
        final Timeline animazione2=creaAnimazioneImmagineTastoCliccato(settingImg);
        final Timeline animazioneOpacitaRettangolo2=creaAnimazioneOpacitaRettangoloTastoCliccato(settingRect);
        final Timeline animazioneAperturaPannelloSetting=creaAnimazionePannelloAScomparsa_Apertura(settingAnchorPane,160);
        settingRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                if(!settingMenuOpened){
                    animazione2.play();
                    animazioneOpacitaRettangolo2.play();
                    animazioneAperturaPannelloSetting.play();
                    settingMenuOpened=true;
                    textPortSetting.setText(String.valueOf(portaSceltaPerServer));
                    
                }
            }
            
        });
       //////////////////////////////////////////////////////////////
        
       //TASTO PRINCIPALE WARNING
        final Timeline animazione3=creaAnimazioneImmagineTastoCliccato(warningImg);
        final Timeline animazioneOpacitaRettangolo3=creaAnimazioneOpacitaRettangoloTastoCliccato(warningRect);
        final Timeline animazioneAperturaPannelloWarning=creaAnimazionePannelloAScomparsa_Apertura(warningAnchorPane,160);
        warningRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                if(!warningMenuOpened){
                    //se il warning rect è rosso, devo farlo tornare blu. Pero' non posso saperlo, quindi ottengo il colore suo attuale, e lo uso come punto di partenza verso il blu
                    Color actual=(Color) warningRect.getFill();
                    (new FillTransition(Duration.millis(500),warningRect,actual,Color.DODGERBLUE)).play();
                    //e poi le altre animazioni
                    animazione3.play();
                    animazioneOpacitaRettangolo3.play();
                    animazioneAperturaPannelloWarning.play();
                    warningMenuOpened=true;
                }
                
                
            }
            
        });
        //////////////////////////////////////////////////////////////
        
       //TASTO PRINCIPALE EXIT
        final Timeline animazione4=creaAnimazioneImmagineTastoCliccato(exitImg);
        final Timeline animazioneOpacitaRettangolo4=creaAnimazioneOpacitaRettangoloTastoCliccato(exitRect);
        exitRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                animazione4.play();
                animazioneOpacitaRettangolo4.play();
                if(threadPerGest!=null)
                    threadPerGest.interrupt(); //questo potrebbe terminare dopo che il main thread è già morto, se per esempio sta bloccato in attesa del client (finchè non scade in timeout l'accept)
                new Thread(new Runnable(){ //la chiusura la faccio fare in un altro thread per evitare che non mi faccia animazione tasto

                    @Override
                    public void run() {
                        try {
                            Thread.sleep(1000);
                        } catch (InterruptedException ex) {
                            Logger.getLogger(ControllerView.class.getName()).log(Level.SEVERE, null, ex);
                        }
                        Platform.exit();
                    }
                    
                }).start();
                
            }
            
        });
        //////////////////////////////////////////////////////////////
        infoIp.setText(myBindedIp);
        //TASTO CONFERMA SETTING MENU
       final Timeline animazione5=creaAnimazioneImmagineTastoCliccato(confirmSettingImg);
       final Timeline animazioneOpacitaRettangolo5=creaAnimazioneOpacitaRettangoloTastoCliccato(confirmSettingRect);
       final Timeline animazioneChiusuraPannelloSetting=creaAnimazionePannelloAScomparsa_Chiusura(settingAnchorPane,160);
        confirmSettingRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                animazione5.play();
                animazioneChiusuraPannelloSetting.play();
                animazioneOpacitaRettangolo5.play();
                settingMenuOpened=false;
                portaSceltaPerServer=Integer.parseInt(textPortSetting.getText());
                System.out.println(portaSceltaPerServer);
                //installo un nuovo tooltip
                tooltipInfoOnRefresh.setText("listen on \nip: "+myBindedIp+"\nport :"+String.valueOf(portaSceltaPerServer));
                
            }
            
        });
        
        //////////////////////////////////////////////////////////////
        
        //TASTO CONFERMA WARNING MENU
       final Timeline animazione6=creaAnimazioneImmagineTastoCliccato(confirmWarningImg);
       final Timeline animazioneOpacitaRettangolo6=creaAnimazioneOpacitaRettangoloTastoCliccato(confirmWarningRect);
       final Timeline animazioneChiusuraPannelloWarning=creaAnimazionePannelloAScomparsa_Chiusura(warningAnchorPane,160);
        confirmWarningRect.setOnMouseClicked(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                animazione6.play();
                animazioneOpacitaRettangolo6.play();
                animazioneChiusuraPannelloWarning.play();
                warningMenuOpened=false;
            }
            
        });
        
        
        
        
    }
    
    public void collegaAlMain(MainApp main){
        this.mainClass=main;
        //il drag finestra va settato qui, e non nell'initialize() perchè li non avevamo ancora il collegamento col main
        //settato sempre sul rectangle dietro l'immagine che è invisibile al mouse, e il rectangle è con opacità 0 (invisibile)
        settaDragFinestra();
    }
    
    private void settaDragFinestra(){
        spostaRect.setOnMousePressed(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                xOff=event.getSceneX();
                yOff=event.getSceneY();
                
            }
            
        });
        
        spostaRect.setOnMouseDragged(new EventHandler<MouseEvent>(){

            @Override
            public void handle(MouseEvent event) {
                mainClass.main_stage.setX(event.getScreenX()-xOff);
                mainClass.main_stage.setY(event.getScreenY()-yOff);
                
            }
            
        });
 
    }
    
    private Timeline creaAnimazioneOpacitaRettangoloTastoCliccato(Rectangle rect){
        Timeline animazione=new Timeline(
                
                                 new KeyFrame(Duration.ZERO,new KeyValue(rect.opacityProperty(),0.42)),
                                 new KeyFrame(new Duration(200),new KeyValue(rect.opacityProperty(),0.1))
                                 
        );
        animazione.setAutoReverse(true);
        animazione.setCycleCount(2);
        return animazione;
    }
    
    private Timeline creaAnimazioneImmagineTastoCliccato(ImageView img){
        Timeline animazione=new Timeline(
                
                                 new KeyFrame(Duration.ZERO,new KeyValue(img.scaleXProperty(),1),new KeyValue(img.scaleYProperty(),1)),
                                 new KeyFrame(new Duration(100),new KeyValue(img.scaleXProperty(),1.2),new KeyValue(img.scaleYProperty(),1.2))
                                 
        );
        animazione.setAutoReverse(true);
        animazione.setCycleCount(2);
        return animazione;
        
    }
    
    private Timeline creaAnimazionePannelloAScomparsa_Apertura(AnchorPane pane,double off){
        Timeline animazione=new Timeline(
                
                                 new KeyFrame(Duration.ZERO,new KeyValue(pane.scaleXProperty(),0.0),new KeyValue(pane.translateXProperty(),-off),new KeyValue(pane.opacityProperty(),0.4)),
                                 new KeyFrame(new Duration(100),new KeyValue(pane.scaleXProperty(),1),new KeyValue(pane.translateXProperty(),0.0),new KeyValue(pane.opacityProperty(),1.0))
                                 
        );
        
        return animazione;
        
    }
    
    private Timeline creaAnimazionePannelloAScomparsa_Chiusura(AnchorPane pane,double off){
        Timeline animazione=new Timeline(
                
                                 new KeyFrame(Duration.millis(100),new KeyValue(pane.scaleXProperty(),1),new KeyValue(pane.translateXProperty(),0.0),new KeyValue(pane.opacityProperty(),1.0)),
                                 new KeyFrame(new Duration(200),new KeyValue(pane.scaleXProperty(),0.0),new KeyValue(pane.translateXProperty(),-off),new KeyValue(pane.opacityProperty(),0.4))
                                 
        );
        
        return animazione;
        
    }
 
    private Timeline creaAnimazioneImmagineCheGira(ImageView img){
        
        Timeline animazione=new Timeline(
                
                                 new KeyFrame(Duration.ZERO,new KeyValue(img.rotateProperty(),0)),
                                 new KeyFrame(new Duration(800),new KeyValue(img.rotateProperty(),360))
                                 
        );
        
        animazione.setCycleCount(Timeline.INDEFINITE);
        return animazione;
        
    
    }
   
    
    private class GestoreAsincrono implements Runnable{  
    /*Questa classe gira come thread a parte in un ciclo infinito *
     * In questa classe c'è allocato l'oggetto Server ed il Segnalatore
     */ 
        public Server server;
        public SegnalatoreGenerico segnalatoreAttuale;
        
        public GestoreAsincrono(){
            //l'oggetto server è bloccante sulla accept() quindi non puoi crearlo qui, bloccheresti il main thread dell'app
        }
        
        @Override
        public void run() {
            System.out.println("ThreadPARTITO");
            threadInAttesaServerAccept=true;
            animazioneRotazione1.play();
            server=new Server(portaSceltaPerServer);
            //da questa ritorna appena il server riesce a ritornare dall'accept(), perchè è scaduto il timeout o perchè è arrivato client
            animazioneRotazione1.stop();
            threadInAttesaServerAccept=false;
            System.out.println("Ritornata la accept");
            if(!server.getStatusServer()){
                System.out.println("Ma server invalido, thread termina!");
                System.out.println(server.getMessageError());
                gestisciErrore("timeout- no client disponibile");
                return;
            }
            broadcastingRectangle.opacityProperty().setValue(0.4); //diventa visibile il rettangolo verde dell'icona broadcasting
            textMessageWarning.clear();
            System.out.println("Server Valido"); 
            String rcvBuff; 
            
            // aspetto con un polling di leggere la prima stringa dal client (che è già connesso a questo punto) che mi dica che segnalatore usare
            
            while(!Thread.interrupted() && server.getStatusServer()){
                rcvBuff=server.extractData();
                if(rcvBuff==null) //il client non ci ha ancora inviato nulla
                    continue;
                //qui abbiamo letto la prima stringa valida ricevuta dal client (che è il suo indicatore di signaler)
                System.out.println("Stringa con indicazione signaler ricevuta: "+rcvBuff);
                if(rcvBuff.equals(PROTOCOLLO_SCELTA_SEGNALATORE_WINDOWS)){
                        System.out.println("IL SEGNALATORE RICHIESTO ESISTE");
                        classeSegnalatoreScelto=KeyboardWindows.class;
                        break;
                }
                /*TO-DO aggiungine altri di segnalatori
                /*TO-DO  aggiungi gestione del caso in cui la prima stringa valida non arrivi mai */
                
            }
            //arrivati qui non sappiamo se siamo usciti dal polling perchè a questo thread è stato indicato di terminare, se qualcosa è andato storto sul srv  o se tutto è andato bene
            if(!server.getStatusServer()){
                gestisciErrore(server.getMessageError());
                return;
            }
            else if(Thread.interrupted()){
                gestisciErrore(null); //cioè abbiamo interrotto ripremendo refresh, subito dopo esserci connessi la prima volta e prima di ricevere la prima stringa valida con indicazione signaler
                return;
            }
            //se stiamo qui tutto è andato bene, sappiamo quale segnaler ci vuole, ma lo dobbiamo ancora allocare basandoci sulla preferenza indicata in classeSceltaSegnalatore
            
            try {allocaSignaler();} catch(Exception e) {gestisciErrore("Errore newIstance() in allocaSignaler()");  return;}
            
            //arrivati qui tutto è andato bene e possiamo iniziare a ciclare per la lettura
            while(!Thread.interrupted() && server.getStatusServer()){
                
                rcvBuff=server.extractData();       
                if(rcvBuff==null) //il null ritornato dall'oggetto server NON vuol dire che il client si è disconnesso. Se il client si fosse disconnesso avrebbe impostato status a falso.
                    continue;
                /*Arrivato qui ho letto una stringa valida normale*/   
                System.out.println(rcvBuff);
                animazioneOpacityBroadcastingRect.play(); //faccio lampeggiare il rettangolo di broadcasting
                //devo distinguere tra una stringa normale ed una stringa di protocollo per riavvio sync
                if(rcvBuff.equals(PROTOCOLLO_RESYNC_REQUEST_DA_CLIENT)){
                    //allora simuliamo la pressione del tasto di refresh a tutti gli effetti, questo fa ricominciare tutto da capo
                    refreshRect.getOnMouseClicked().handle(null);
                }
                
                else{try {segnalatoreAttuale.launchSignal(rcvBuff);} catch (Exception e) {break;}}
            
            }
            
            /*TO-DO *
             * Arrivati qui o il thread è stato interrotto,o  qualcosa è andato storto (ad esempio ci ha disconnessi il client)
             */
            
            if(!server.getStatusServer())
                    gestisciErrore(server.getMessageError());
            else    
                     gestisciErrore(null); //se siamo stati noi ad interrompere, passiamo null come messaggio, poichè solo alcune operazioni vanno fatte del metodo gestisciErrore
            
            //metto una sleep per essere sicuro che la JVM sbindi le porte (non lo fa appena richiesto)
            try{Thread.sleep(5000);} catch(Exception e){/*TO-DO NON DEVO FARE NULLA QUI ?*/}
            return;
        }
        
        private void gestisciErrore(String strDaMettereNelWarning){
            
            server.stopEpulisciSrv();
            broadcastingRectangle.opacityProperty().setValue(0.0); //sparisce il triangolo verde
            System.out.println("PROBLEMA!?");
            if(strDaMettereNelWarning==null) 
                return;
            if(!warningMenuOpened)//il warningRect diventa rosso (se non è già aperto o se non siamo stati noi ad interrompere)
                    (new FillTransition(Duration.millis(500),warningRect,Color.DODGERBLUE,Color.RED)).play(); 
            
            textMessageWarning.setText(strDaMettereNelWarning);
        }
        
        private void allocaSignaler() throws InstantiationException, IllegalAccessException{
            segnalatoreAttuale=(SegnalatoreGenerico) classeSegnalatoreScelto.newInstance();
        }
        
    }
}
