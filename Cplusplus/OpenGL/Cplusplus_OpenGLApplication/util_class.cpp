#include "util_class.h"



//devo fare questa inizializzazione fittizia, poichè questi membri, che devono per forza essere statici, se non sono inizializzati da fuori, il compilatore mi da errore!
gestore_rete &gestore_finestra::network=*(new gestore_client());
info_posizione &gestore_finestra::myinfo=*(new info_posizione());
char * gestore_finestra::keybuffer=NULL;
char * gestore_finestra::specialkeybuffer=NULL;
char * gestore_finestra::to_receive=NULL;
char * gestore_finestra::to_send=NULL;
vector<mesh *> gestore_finestra::oggetti;
vector<info_posizione> gestore_finestra::info_oggetti;
gestore_gravita gestore_finestra::gravita;
float gestore_finestra::lungh_spostam=.6;
vector<info_luce> gestore_finestra:: luci;
vector<NurbsNonOmogenea> gestore_finestra::superficiNurbs;
vector<info_posizione> gestore_finestra::info_CurveNurbs;




gestore_finestra::gestore_finestra(gestore_rete &netw,info_posizione &myinf,float fv,float znr,float zfr,float wwid,float whei,int *pargc, char **argv_in) : fov(fv),znear(znr),zfar(zfr),wwidth(wwid),wheight(whei),p_argc(pargc),argv(argv_in){
	
	network=netw;
	myinfo=myinf;
	gravita=gestore_gravita(&info_oggetti,&myinfo);

	keybuffer=(char *)new char[256];
	specialkeybuffer=(char *)new char[256];
	to_receive=(char *)new char[1024];
	to_send=(char *)new char[1024];	
	
	glutInit(pargc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(wwidth,wheight);
	glutCreateWindow("finestra");
	glutDisplayFunc(callback_display);
	glutSpecialFunc(callback_specialk);
	glutSpecialUpFunc(callback_specialk_up);
	glutKeyboardFunc(callback_keyboard);
	glutKeyboardUpFunc(callback_keyboard_up);
	glutTimerFunc(10,callback_timer_principale0,0);
	glutTimerFunc(10,callback_timer_socket,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,(float)wwidth/wheight,znear,zfar);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	//il glutMainLoop non lo lancio qui, ma viene lanciato tramite un metodo di questa classe, nel momento in cui è stato fatto tutto quello che si doveva fare con questa classe.
	//Poichè ad esempio altrimenti non potremmo aggiungere nuove mesh e nuove info oggetti a questa classe poichè una volta finito il costruttore, rimarremmo bloccati nel glutMainLoop();

	/*creo tutta la serie di nomi per textures (non ancora assegnati a questo punto)*/
	texthandlers=(GLuint *)new GLuint[200]; //200 per sicurezza
	indtexthandlernonusato=0;
	glGenTextures(200,texthandlers);
}


void gestore_finestra::start_glutMainLoop(){
	glutMainLoop();
}



void gestore_finestra::check_errori(){
GLenum errore;
while((errore=glGetError())!=GL_NO_ERROR)
	cout<<gluErrorString(errore)<<endl;

}





void gestore_finestra::callback_display(){
	glEnable(GL_DEPTH_TEST); //VA MESSO PER FORZA QUI !!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*DISEGNO MIRINO e ARMA e LUCE ARMA: questi non devono variare quando noi giriamo la visuale o 		ci spostiamo, 	 quindi li disegniamo prima di trasformare il mondo per simulare il cambio di 		visuale/movimento*/
	
	/*-----MIRINO-----*/
	/*essendo il mirino una semplice croce, non si usa una mesh nè un oggetto info_posizione per rappresentarlo, quindi non è disegnato usando la trafila delle mesh/info posizioni. Invece si usa una 
	routine apposita, al cui interno viene effettuato il posizionamento (visto che va un minimo traslato in avanti altrimenti non si vede)*/	
	glPushMatrix();
		disegna_mirino(.05);
	glPopMatrix();
	/*-----ARMA-----*/
	/*L'arma è invece un oggetto classe mesh, con associato l'oggetto info posizione, e si trova sempre 		in indice 0 del vector di mesh contenuto nel gestore_finestra */
	glPushMatrix(); 
		rototrasla_oggetto(info_oggetti[0]);
		oggetti[0]->mi_disegno();
	glPopMatrix();

	/*---LUCE ARMA sta sempre in indice 0---ATTENZIONE:il posizionamento di una luce è differente da 	 quello di una mesh (L'ho spiegato più giù perchè !!)
	/*visto che mi trovo faccio qui l'aggiornamento dello stato della luce*/
	glLightfv(luci[0].id,GL_POSITION,luci[0].coord);
	glLightfv(luci[0].id,GL_SPECULAR,luci[0].speccomp);
	glLightfv(luci[0].id,GL_DIFFUSE,luci[0].diffcomp);
	glLightfv(luci[0].id,GL_AMBIENT,luci[0].ambientcomp);
	/*e' sicuramente luce spotlight per creazione */	
	glLightf(luci[0].id,GL_SPOT_CUTOFF,luci[0].spotcutoff);
	glLightfv(luci[0].id,GL_SPOT_DIRECTION,luci[0].spotdircomp);
	glLightf(luci[0].id,GL_SPOT_EXPONENT,luci[0].spotexponent);
	/*potrebbe essere stata spenta*/
	if(luci[0].on) 
		glEnable(luci[0].id);
	else glDisable(luci[0].id);



	/*------ROTOTRASLAZIONE DEL MONDO, RIPOSIZIONAMENTO E DISEGNO DELLE MESH OGGETTI (tra cui mesh del peer),delle nurbs, E POI RIPOSIZIONAMENTO DELLE LUCI--------------*/
	glPushMatrix();
		rototrasla_mondo(); /*a seconda dell'opposto di nostri movimenti e vista, applica una 			trasformazione A TUTTO IL MONDO (usando myinfo), quindi ai successivi oggetti che andremo a rappresentare nel mondo

		/*------------------RAPPRESENTAZIONE MESH OGGETTI------------------------- 
		/*LA POSIZIONE IN CUI RAPPRESENTARLI E' MANTENUTA, PER CIASCUNA MESH, NEL CORRISPONDENTE OGGETTO INFO_OGGETTO
		Scorri su tutti gli oggetti presenti nel vector delle mesh, e per ognuno di questi
		usa il corrispondente oggetto info_posizione che indica la trasformazione relativa 
		alla rotazione/traslazione LOCALE DI QUESTO (quindi in aggiunta, se c'è stata, a quella di 			tutto il mondo).*/
		for(int i=1;i<info_oggetti.size();i++){/*parte da 1, perchè in 0 c'è arma che essendo 
							ancorata alla "testa" del personaggio, non deve 							subire le trasformazioni subite dal mondo per 								simulare la nostra rotazione testa/movimento */
			glPushMatrix();
				rototrasla_oggetto(info_oggetti[i]); /*questa funzione, usando le info 					contenute in info_oggetti[i] rototrasla il sistema di riferimento prima di 					disegnare oggetto*/
				oggetti[i]->mi_disegno(); /* in questa funzione vengono impostate le normali 
				prima di disegnare, per gestire la luce */
			glPopMatrix();
		}
		
		/*la stessa cosa per il vector di oggetti di classe Nurbs...*/
		for(int i=0;i<superficiNurbs.size();i++){
			glPushMatrix();
				rototrasla_oggetto(info_CurveNurbs[i]);
				superficiNurbs[i].mi_disegno();
				
			glPopMatrix();		
		}
		
		
		
		/*------LUCI (ad esclusione della luce sull'arma )-------*
		/*LA POSIZIONE IN CUI RAPPRESENTARLE È MANTENUTA, PER CIASCUNA LUCE, NELL'OGGETTO INFO_LUCE. TUTTAVIA SE LA LUCE SEGUE UNA MESH, ALLORA LE COORDINATE DELLA LUCE SONO 
		RISPETTO AL CENTRO DEL SISTEMA DI RIFERIMENTO SU CUI E' "CENTRATA (QUASI)" LA MESH, QUINDI PRIMA DI POSIZIONARE LA LUCE, BISOGNA CENTRARE IL SISTEMA DI RIFERIMENTO NEL PUNTO
		IN CUI E' CENTRATA LA MESH, E POI CHIAMARE LA funzione per modificare GL_POSITION. Questo per progettazione accade solo per la luce del peer.		
		/*Le luci, a differenza delle mesh le cui coordinate sono fatte in modo tale che siano
		centrate nell'origine del sistema di riferimento, quindi sono disegnate spostando 
		il sistema di riferimento per rappresentare la posizione della mesh nel mondo (che a sua
		volta è rototraslato secondo la myinfo).
		Per le luci invece vengono posizionate nel mondo (dopo averlo rototraslato) fornendo 
		direttamente le coordinate di queste nel mondo (rototraslato) e non centrando prima
		il sistema di riferimento in quel punto e posizionandole all'origine degli assi !*/
		for(int i=1;i<luci.size();i++){ /*parte da 1 poichè in 0 c'è luce su arma */
			if(i==1){ /*allora per quanto detto sopra, solo in questo caso essendo la luce del peer...*/
				glPushMatrix();
					rototrasla_oggetto(info_oggetti[1]); /*ottengo la trasformazione che mi centra il sistema di riferimento nella posizione in cui si trova il peer*/
								
				
			}
			
			glLightfv(luci[i].id,GL_POSITION,luci[i].coord);
			glLightfv(luci[i].id,GL_POSITION,luci[i].coord);
			glLightfv(luci[i].id,GL_SPECULAR,luci[i].speccomp);
			glLightfv(luci[i].id,GL_DIFFUSE,luci[i].diffcomp);
			glLightfv(luci[i].id,GL_AMBIENT,luci[i].ambientcomp);	
			/* operazioni se la luce è spotlight*/
			if(luci[i].spot){
				glLightf(luci[i].id,GL_SPOT_CUTOFF,luci[i].spotcutoff);
				glLightfv(luci[i].id,GL_SPOT_DIRECTION,luci[i].spotdircomp);
				glLightf(luci[i].id,GL_SPOT_EXPONENT,luci[i].spotexponent);			
			}
			if(i==1)
				glPopMatrix();
			if(luci[i].on)
				glEnable(luci[i].id);
			else glDisable(luci[i].id);
		}
		
	
	glPopMatrix();

	glutSwapBuffers();

}


void gestore_finestra::callback_specialk(int k ,int x ,int y){
	specialkeybuffer[k]=1;
}


void gestore_finestra::callback_specialk_up(int k, int x, int y){
	specialkeybuffer[k]=0;
}

void gestore_finestra::callback_keyboard(unsigned char k ,int x,int y){
	keybuffer[(int)k]=1;
}

void gestore_finestra::callback_keyboard_up(unsigned char k,int x,int y){
	keybuffer[(int)k]=0;
}



void gestore_finestra::callback_timer_socket(int val){
	
	if(!network.ricevi(to_receive)){ //allora la select nella funzione ricevi è andata in timeout
		glutTimerFunc(10,callback_timer_socket,0);		
		return;
	}
	if(strncmp(to_receive,"MOV",3)==0){//allora abbiamo ricevuto  messaggio che avvisa di movimento/rotazione del peer
		sscanf(to_receive,"MOV||%f||%f||%f||%f||%f\n",&info_oggetti[1].movimentox,&info_oggetti[1].movimentoy,&info_oggetti[1].movimentoz,&info_oggetti[1].angle_vista_toy,&info_oggetti[1].angle_vista_tox);
		fflush(stdout);
		glutPostRedisplay(); 
	}
	glutTimerFunc(10,callback_timer_socket,0);

	
}
	

void gestore_finestra::callback_timer_principale0(int val){
/*per evitare di creare troppi timer diversi (uno per il controllo del buffering dei tasti e l'aggiornameto delle info posizione, e l'avviso conseguente del peer, uno per la gravità che deve avvisare il peer ogni volta che un oggetto subisce una variazione di posizione a causa della gravità etc, il che porterebbe ad un sacco di timer ciascuno che invia il suo messaggio al peer, ho fuso piu' controlli in questa callback */

/*Uso un flag toupdate che viene messo a true se almeno un evento si è verificato che necessita che il peer sia avvisato
il flag è inizializzato col valore ritornato dal metodo calcola_applica_gravita dell'handler della gravità, che ritorna true se il nostro peer ha cambiato posizione a causa della gravità (e già questo basterà per far si' che si debba avvisare il peer) altrimenti ritorna false sul flag*/
	bool toupdate=gravita.calcola_applica_gravita(); 

/*qui inizia la parte del controllo del keybuffering, quindi in base ai tasti che risultano premuti (secondo la logica del key buffering) aggiorniamo le info posizione. Se almeno una volta viene aggiornata info posizione, toupdate diventa true, ad indicare che il peer deve essere avvisato. */
	/* controllo e aggiornamento per le rotazioni del mondo rispetto a noi, per simulare la rotazione della nostra testa*/
	if(specialkeybuffer[GLUT_KEY_UP]){
		toupdate=true;
		myinfo.angle_vista_tox+=2;
	}
	if(specialkeybuffer[GLUT_KEY_DOWN]){
		toupdate=true;
		myinfo.angle_vista_tox-=2;
	}
	if(specialkeybuffer[GLUT_KEY_LEFT]){
		toupdate=true;
		myinfo.angle_vista_toy+=2;
	}
	if(specialkeybuffer[GLUT_KEY_RIGHT]){
		toupdate=true;	
		myinfo.angle_vista_toy-=2;
	}
	
	/*aggiornamento movimento, questo viene scomposto nei vettori componenti sul sistema di riferimento del mondo ruotato*/
	if(keybuffer[(int)'w']){
		toupdate=true;
		myinfo.movimentoz-=lungh_spostam*sin((90+myinfo.angle_vista_toy)*PI/180.0f);
		myinfo.movimentox+=lungh_spostam*cos((90+myinfo.angle_vista_toy)*PI/180.0f);
	}

	if(keybuffer[(int)'s']){
		toupdate=true;
		myinfo.movimentoz+=lungh_spostam*sin((90+myinfo.angle_vista_toy)*PI/180.0f);
		myinfo.movimentox-=lungh_spostam*cos((90+myinfo.angle_vista_toy)*PI/180.0f);
	}
	if(keybuffer[(int)'d']){
		toupdate=true;
		myinfo.movimentoz+=lungh_spostam*sin(-myinfo.angle_vista_toy*PI/180.0f);
		myinfo.movimentox+=lungh_spostam*cos(-myinfo.angle_vista_toy*PI/180.0f);
	}
	if(keybuffer[(int)'a']){
		toupdate=true;
		myinfo.movimentoz-=lungh_spostam*sin(-myinfo.angle_vista_toy*PI/180.0f);
		myinfo.movimentox-=lungh_spostam*cos(-myinfo.angle_vista_toy*PI/180.0f);
	}
	/*nel caso venga premuto il tasto di salto, modifichiamo la gravità (dando valore negativo della gravità, che per convenzione porterà l'oggetto a salire verso l'alto, fino a che non si esaurisca la spinta
	verso l'alto) e diamo un valore di y leggermente maggiore del valore considerata la soglia del suolo (altrimenti il gestore della gravità considererebbe l'oggetto come appena arrivato al suolo)
	NB: non c'è bisogno di settare il flag toupdate a true, poichè ora essendoci un oggetto che sta subendo la gravità, non trovandosi al suolo (anzi ha spinta verso l'alto) sicuramente la volta successiva 
	che questa funzione di callback verrà chiamata, il metodo calcola_applica_gravita del gestore gravita ritornerà true e quindi sicuramente il peer verrà avvisato */

	if(keybuffer[(int)'f'] && myinfo.movimentoy<=0.0002){ 
		gravita.setta_miaaccel(-10.0);
		myinfo.movimentoy=0.0002;
	}
	
	/*nel caso in cui ci sia stato uno degli avvenimenti di cui sopra, dobbiamo avvisare il peer, mandandogli un messaggio di movimento*/
	if(toupdate){
		memset(to_send,0,1024);
		sprintf(to_send,"MOV||%f||%f||%f||%f||%f\n",myinfo.movimentox,myinfo.movimentoy,myinfo.movimentoz,myinfo.angle_vista_toy,myinfo.angle_vista_tox);
		network.invia(to_send);
		glutPostRedisplay();
	}
	/*in ogni caso riregistro questa callback*/
	glutTimerFunc(10,callback_timer_principale0,0);


}







void gestore_finestra::rototrasla_mondo(){
//questa serve per muovere il mondo simulando il mio movimento/rotazione
//rappresentiamo il mondo ruotato e traslato dei valori opposti di quella che è la mia rotazione e traslazione rispetto al mondo	
	glRotatef(-myinfo.angle_vista_tox,1.0,0.0,0.0);
	glRotatef(-myinfo.angle_vista_toy,0.0,1.0,0.0);
	glRotatef(-myinfo.angle_vista_toz,0.0,0.0,1.0);
	glTranslatef(-myinfo.movimentox,-myinfo.movimentoy,-myinfo.movimentoz);
}







void gestore_finestra:: aggiungi_mesh(mesh * mesh_oggetto){
	oggetti.push_back(mesh_oggetto);
}

void gestore_finestra:: aggiungi_info_posizione_mesh(info_posizione info){
	info_oggetti.push_back(info);
}


void gestore_finestra::aggiungi_luce(info_luce luce){
	luci.push_back(luce);
	/*se l'oggetto info_luce ha on=true allora viene attivata dalla callback di disegno*/
}



void gestore_finestra::aggiungi_info_CurvaNurbs(info_posizione info){
	info_CurveNurbs.push_back(info);
}

void gestore_finestra::aggiungi_CurvaNurbs(NurbsNonOmogenea curva){
	superficiNurbs.push_back(curva);
}





void gestore_finestra::rototrasla_oggetto(info_posizione &info){
//info contiene le informazioni dell'orientazione e della posizione dell'oggetto
//le trasformazioni servono per posizionare, prima di disegnare oggetto, il sistema di riferimento per il disegno
//nella posizione in cui si trova oggetto rispetto al mondo, e non sono fatte piu' dell'opposto del movimento, in quanto quello vale solo per il mondo rispetto a noi !
	glTranslatef(info.movimentox,info.movimentoy,info.movimentoz);
	glRotatef(info.angle_vista_toy,0.0,1.0,0.0);
	glRotatef(info.angle_vista_tox,1.0,0.0,0.0);
	glRotatef(info.angle_vista_toz,0.0,0.0,1.0);


}



void gestore_finestra::disegna_mirino(float raggio){
	glDisable(GL_LIGHTING);
	glColor3f(0.0,1.0,0.0);
	glTranslatef(0.0,0.0,-1.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
		glVertex3f(0.0,-0.5*raggio,0.0);
		glVertex3f(0.0,0.5*raggio,0.0);
		glVertex3f(-0.5*raggio,0.0,0.0);
		glVertex3f(0.5*raggio,0.0,0.0);
	
	glEnd();
	glEnable(GL_LIGHTING);


}










//--------------------------------*

bool gestore_gravita::calcola_applica_gravita(){

/*Attenzione: il primo oggetto mesh non subisce la gravità (poichè è l'arma), il secondo neanche (poichè è la mesh del peer, e la sua altezza non la calcoliamo noi ma viene calcolata dal peer e ci viene direttamente inviata, cosi' come noi calcoliamo la nostra e la mettiamo in myinfo e gliela inviamo, ed il terzo oggetto neanche poichè è il pavimento)

/*questa funzione calcola la gravità degli oggetti presenti*/
//ritorna true se il nostro personaggio subisce la gravità poichè solo in tal caso dobbiamo
//avvisare con un messaggio il peer inviandogli la nostra nuova posizione y
	bool flag=false;
	//printf("\n mia accel %f,%f\n",mia_accelerazione,mia_posizione->movimentoy); fflush(stdout);
	
	//calcolo la gravità sugli oggetti 
	for(int i=3;i<posizioni->size();i++){ // parte da 3 perchè in 0 c'è arma che non deve subire gravita 							ed in 1 c'è la mesh del peer, la cui posizione (che subisce 							la gravità) ci viene direttamente comunicata dal socket 						quindi non dobbiamo calcolarla noi, ed in 2 c'è il pavimento 							che non deve subire la gravità
		if((*posizioni)[i].movimentoy<=0.0001){
			(*posizioni)[i].movimentoy=0.0;
			accelerazioni[i]=0.0;
			continue;
		}
		(*posizioni)[i].movimentoy-=accelerazioni[i];
		accelerazioni[i]+=0.1;
	}
	//calcolo la gravità su di me (testa, visuale)
	if((*mia_posizione).movimentoy<=0.0001){
		(*mia_posizione).movimentoy=0.0;
		mia_accelerazione=0.0;	
	}
	else{
		flag=true;
		mia_posizione->movimentoy-=mia_accelerazione;
		mia_accelerazione+=0.1;
	}

	return flag;

}



void gestore_gravita::setta_accell_oggetto(int i,float val){
	accelerazioni[i]=val;
}


void gestore_gravita::setta_miaaccel(float val){
	mia_accelerazione=val;
}








