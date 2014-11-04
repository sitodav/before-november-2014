package com.sitodskij.keyboardcontroller;


import java.util.HashMap;
import java.util.Map;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData.Item;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import com.sitodskij.freepccontroller.R;

public class MainActivity extends Activity {

	
	
	
	private final String PROTOCOLLO_RESYNC_REQUEST_DA_CLIENT="***RESYNC***";
    private final String[] PROTOCOLLO_SCELTE_SEGNALATORI=new String[]{
    		"***WINDOWSKEYBOARD" //quella di indice 0, che corrisponde sullo spinner ad un elemento di indice 0
    		/*TO-DO ALTRI */
    };
    private String[] arrayStringheDiSceltaPerSpinner= new String[]{
    		"WINDOWS"
    		/*TO-DO ALTRI */
    };
    int indiceOsAttuale=0; //serve cosi' che ogni volta che parte l'alertdialog, si puo' risalire a quale elemento far apparire già selezionato nello spinner
    //e questo indice è utilizzato pure per vedere quale Os è stato scelto. Le stringhe nell'array servono solo per essere mostrate nello spinner
    
    
	private String ipServer="192.168.1.64";
	private int portaServer=4444;
	private TcpClient tcpClient;
    private boolean serverAvvisatoDelSegnalerScelto=false;
    private boolean clientConnesso=false;
    private Menu myActionBar;
    private StatusUpdater statusUpdater;
    private Thread threadStatusUpdater;
    private Handler handlerPerAggiornamentoIconaStato;
	
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		this.getWindow().setSoftInputMode(
			    WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		
		getWindow().setBackgroundDrawableResource(R.drawable.black2);
	    
		
		
		
	
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.actionbarmia, menu);
		myActionBar=menu;
		
	
		//creo l'handler per la comunicazione tra questo thread (che è il main, quello che ha creato le view) e il thread status updater
		handlerPerAggiornamentoIconaStato=new Handler(){
			public void handleMessage(Message msg){
				MenuItem broadcastingIcon=myActionBar.findItem(R.id.broadcastingIcon);
				Bundle bundle=msg.getData();
				if(broadcastingIcon!=null)
					broadcastingIcon.setTitle(msg.getData().getString("stato"));
				
			}
		};
		return super.onCreateOptionsMenu(menu);
	}
	
	public boolean onOptionsItemSelected(MenuItem item){
		switch(item.getItemId()){
		case R.id.collegaIcon:
			//a quanto pare va creato un nuovo dialog per ogni volta che va usato, altrimenti crasha.
			//creo un linear layout, ci metto dentro tutti gli elementi che mi servono (con i valori presi dalle variabili erlative), 
			//e setto il linear layout come view dell'AlertDialog
			LinearLayout lay=new LinearLayout(this); lay.setOrientation(LinearLayout.VERTICAL);
			final EditText ipField=new EditText(this),portField=new EditText(this);
			ipField.setText(ipServer); portField.setText(String.valueOf(portaServer));
			lay.addView(portField); lay.addView(ipField);
			final Spinner sceltaOsSpinner=new Spinner(this);
			ArrayAdapter<String> adapterPerSpinner=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_dropdown_item,arrayStringheDiSceltaPerSpinner);
			sceltaOsSpinner.setAdapter(adapterPerSpinner);
			sceltaOsSpinner.setSelection(indiceOsAttuale);
			lay.addView(sceltaOsSpinner);
			
			String strTastoDiOk=(clientConnesso) ? "RESYNC" : "SYNC" ; //a seconda dello stato del client, cambia la scritta del pulsante di ok del dialog
			(new AlertDialog.Builder(this)) //AlertDialog.Builder è proprio nome della classe per gli alert dialog
				.setView(lay)
				.setTitle("SYNC PANEL")
				.setPositiveButton(strTastoDiOk,new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						//carico il contenuto settato (al momento in cui premo il tasto di OK) nelle variabili di classe
						indiceOsAttuale=sceltaOsSpinner.getSelectedItemPosition();
						portaServer=Integer.parseInt(portField.getText().toString());
						ipServer=ipField.getText().toString();
						//ora dobbiamo distinguere i comportamenti a seconda che non siamo connessi, o che lo siamo già
						if(!clientConnesso){
							apriEAvvia();
							//arrivati qui non è detto che il client sia realmente connesso. Non ci interessa per ora, controlleremo poi.
						}
						else{ //sono già connesso, e sto richiedendo un resync
							boolean valoreInutile=scrivi(PROTOCOLLO_RESYNC_REQUEST_DA_CLIENT);
							/* TO-DO
							 * controlla che non sia importante gestire il caso in cui scrivi qui ritorni false...
							 */
							//Log.e("MAIN_APP","HO CHIESTO DI RISINCRONIZZARMI MENTRE ERO CONNESSO");
							stoppaEChiudi();
							apriEAvvia();
						}
						
						
					}
				})
				.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						
					}
				})
				.show();

			break;
	
		
		}
		
		
		return true;
	}
	
	
	

	
	
	private void stoppaEChiudi(){
		//Log.e("MAIN_APP","CHIUDO TCPCLIENT,INTERROMPO STATUS THREAD");
		tcpClient.chiudiTutto();
		/*NB *
		 * QUI E' FONDAMENTALE METTERCI UNA SLEEP, POICHE' DOPO AVER CHIUSO TCPCLIENT, (QUINDI IL SUO STATO E' A INVALIDO) DOBBIAMO DARE IL TEMPO AL
		 * THREAD CHE UPPA LO STATO DI ACCORGERSENE, PRIMA DI STOPPARLO. SE NON LO FACCIAMO, NON RIESCE A RICOLLEGARSI PER QUALCHE MOTIVO !!!!
		 * Ed inoltre do anche il tempo alle porte del server di sbindarsi per ribindarsi sulle stesse (la jvm ci mette tempo)
		 */
		try {Thread.sleep(4000);} catch (Exception e) {/*TO-DO DEVO FARE QUALCOSA IN QUESTO CASO ?*/}
		threadStatusUpdater.interrupt(); //e interrompo il thread statusUpdater
		
		
	}
	
	private void apriEAvvia(){
		//Log.e("MAIN_APP","CREO TCPCLIENT,CREO THREAD STATUS UPDATER");
		tcpClient=new TcpClient(ipServer,portaServer);
		statusUpdater=new StatusUpdater();
		threadStatusUpdater=new Thread(statusUpdater);
		threadStatusUpdater.start();
	}
	
	private boolean scrivi(String str){
		if(!clientConnesso)
			return false;
		if(!tcpClient.sendData(str)){ //se arrivati qui, un false ritornato da tcpClient.sendData() non puo' voler dire che non eravamo ancora connessi, ma che lo eravamo e siamo caduti
			stoppaEChiudi();
			return false;
		}
		return true;
	}

	public void cliccato(View view){
		switch(view.getId()){
		case R.id.up:
			scrivi("up");
			break;
		case R.id.down:
			scrivi("down");
			break;
		case R.id.left:
			scrivi("left");
			break;
		case R.id.right:
			scrivi("right");
			break;
		case R.id.enter:
			scrivi("enter");
			break;
		case R.id.esc:
			scrivi("esc");
			break;
		case R.id.altf4:
			scrivi("altf4");
			break;
		case R.id.backspace:
			scrivi("backspace");
			break;
		case R.id.windowsexplorer:
			scrivi("windowsexplorer");
			break;
		case R.id.taskbar:
			scrivi("taskbar");
			break;	
		case R.id.windows:
			scrivi("windows");
			break;
		case R.id.tab:
			scrivi("tab");
			break;	
		case R.id.alttab:
			scrivi("alttab");
			break;	
		case R.id.minimizeall:
			scrivi("minimizeall");
			break;
		case R.id.aggiorna:
			scrivi("aggiorna");
			break;
		case R.id.backward:
			scrivi("backward");
			break;
		case R.id.forward:
			scrivi("forward");
			break;
		case R.id.property:
			scrivi("property");
			break;
		case R.id.fullscreen:
			scrivi("fullscreen");
			break;
		case R.id.urlbar:
			scrivi("urlbar");
			break;
		case R.id.scrivi:
			String testo= ((EditText)findViewById(R.id.barraTesto)).getText().toString();
			if(testo!=null)
				scrivi(testo);
			break;
			
		}
	}
	
	
	
	private class StatusUpdater implements Runnable{
		//fino a che questo thread non si accorge che lo stato dell'oggetto tcpclient appena creato, non diventa VALIDO, non è permesso a quest'app
		//usare il clientTcp per inviare dati. Quindi appena questo thread si accorge che è connesso effettivamente il tcpClient, si occupa di inviare la prima stringa al 
		//server che avvisa di che segnaler è stato scelto e poi mette connesso==true cosi' sarà possibile per l'app inviare le successive stringhe al server.
		@Override
		
		public void run() {
		
			while(!Thread.interrupted()){
				if(!tcpClient.getStatus() && clientConnesso){
					//Log.e("MAIN_APP","CLIENT DIVENTATO DISCONNESSO");
					serverAvvisatoDelSegnalerScelto=false;
					clientConnesso=false;
					/*faccio si' che sia il main thread ad aggiornare l'item dell'action bar usando l'handler*/
					Message msg=handlerPerAggiornamentoIconaStato.obtainMessage();
					Bundle bundle=new Bundle();
					bundle.putString("stato", "disconnected");
					msg.setData(bundle);
					handlerPerAggiornamentoIconaStato.sendMessage(msg);
					
				}
				
				else if(tcpClient.getStatus() && !clientConnesso){
					
					//Log.e("MAIN_APP","CLIENT DIVENTATO CONNESSO");
					//controlliamo se è già stata mandata la prima stringa che avvisa il server del segnaler scelto
					//in caso contrario la invio io (nb devo usare direttamente il sendData del tcpClient, poichè il metodo scrivi del main thread sarà bloccato fino a quando questo stesso thread non sarà arrivato piu' avanti
					if(!serverAvvisatoDelSegnalerScelto){
						//Log.e("MAIN_APP","AVVISO SERVER DEL SIGNALER!");
						//uso l'indice della stringa dello spinner scelta, che ha lo stesso indice dell'equivalente stringa protocollo nell'array stringhe apposito
						if(!tcpClient.sendData(PROTOCOLLO_SCELTE_SEGNALATORI[indiceOsAttuale])){
							//se siamo arrivati qui il client era connesso, quindi un sendData negativo vuol che è subito ricaduto....
							stoppaEChiudi();
							return; //facendo return qui, il thread muore e non arrivera' mai a sbloccare clientConnesso, quindi non si potranno mandare messaggi, e bisognerà sincronizzare da capo per forza
						}
						//Log.e("MAIN_APP","SERVER AVVISATO DEL SIGNALER!");
						serverAvvisatoDelSegnalerScelto=true;
							
					}	
					//Log.e("MAIN_APP","SETTO IL VIA LIBERA PER LA MAIN APP");
					clientConnesso=true;
					/*faccio si' che sia il main thread ad aggiornare l'item dell'action bar usando l'handler*/
					Message msg=handlerPerAggiornamentoIconaStato.obtainMessage();
					Bundle bundle=new Bundle();
					bundle.putString("stato", "connected");
					msg.setData(bundle);
					handlerPerAggiornamentoIconaStato.sendMessage(msg);
					
				}
			}
			
		}
		
	}
	
}
