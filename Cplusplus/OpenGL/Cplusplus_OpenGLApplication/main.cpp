#include "util_class.h"





int main(int argc,char *argv[]){
//il primo parametro indica se si vuole essere server o client.
//se si passa 0, si è server, e allora andra' passata anche la porta (quindi argc==3)
//se si passa 1, si è client, e si deve passare anche ip, quindi argc==4

gestore_rete * network; //puntatore, per evitare l'upslicing poichè l'actual type è di una delle due classi DERIVATE (o gestore_client o gestore_server)

if(argc<3){
	printf("\n<0/1> [ip] <porta>\n");
	exit(1);
}

int scelta,porta;
char ip[INET_ADDRSTRLEN];

sscanf(argv[1],"%d",&scelta);
if(!scelta){
	sscanf(argv[2],"%d",&porta);
	network=(gestore_server *)new gestore_server(porta);
	
}

else{
	if(argc==3){
		printf("<0/1> <ip> <porta>\n");
		exit(1);
	}
	sscanf(argv[2],"%s",ip);
	sscanf(argv[3],"%d",&porta);
	network=(gestore_client *)new gestore_client(porta,ip);
	
	
}

//indipendentemente da che sia server o client
network->start_comunicazione();
if(network->get_stato()){
	cout<<network->get_errore()<<endl;
	exit(1);	
}

info_posizione myinfo(0.0,0.0,0.0,0.0,0.0,0.0); //inizializzo oggetto info_posizione che rappresenta ME e lo passo al costruttore della classe gestore_finestra
gestore_finestra fin(*network,myinfo,60,0.1,500,1500,500,&argc,argv);




/*occorre PER FORZA inserire nel gestore finestra, almeno la mesh che rappresenta l'arma (ed il relativo oggetto info_posizione) e la mesh che rappresenta il peer (e relativa info posizione).
Quindi nel vector oggetti, della classe gestore_finestra, all'indice 0 c'è sempre mesh arma, e all'indice 1 la mesh del peer. Nel vector di info_oggetti invece, all'indice 0 c'è l'oggetto info_posizione dell'arma, e nell'indice 1 quello della mesh del peer. Parallelamente nel vector luci in indice 0 c'è la luce relativa all'arma, in indice 1 c'è la luce relativa al peer .


/*----------PER LA MESH DELL'ARMA ----------------*/
//prevedo per mesh peer, essendo mesh senza texture, dei materials scelti da me
float diffarma[]={0.4,0.1,0.8}, ambarma[]={0.2,0.5,0.9}, specarma[]={0.2,0.2,0.9};
//lancio routine che mi legge il file di tipo .obj che contiene il modello dell'arma
mesh *mesh_arma=generamesh_dafile_wavefrontobj_senzanormali_senzatexture("mesh/ak47.obj",1.0,diffarma,ambarma,specarma);
//la aggiungo
fin.aggiungi_mesh(mesh_arma);
//creo oggetto che rappresenta posizione e orientamento della mesh dell'arma
fin.aggiungi_info_posizione_mesh(info_posizione(6.5,7,0,3,-5.0,-3.7));





/*--------------PER LA MESH DEL PERSONAGGIO AVVERSARIO (PEER)-----*/
//prevedo per mesh peer, essendo mesh senza texture, dei materials scelti da me
//float diffpeer[]={0.4,0.1,0.8}, ambpeer[]={0.2,0.5,0.9}, specpeer[]={0.2,0.2,0.9};
float diffpeer[]={0.1,0.1,0.1}, ambpeer[]={0.1,0.1,0.1}, specpeer[]={0.1,0.1,0.1};

//uso la routine che mi costruisce un oggetto di classe mesh leggendo info da file .obj
mesh *mesh_personaggio_peer=generamesh_dafile_wavefrontobj_senzanormali_senzatexture(string("mesh/provame.obj"),0.1,diffpeer,ambpeer,specpeer);

//aggiungo la mesh all'oggetto di classe gestione_finestra
fin.aggiungi_mesh(mesh_personaggio_peer);

//creo oggetto per info posizione mesh (del personaggio del peer) e lo aggiungo all'oggetto classe gestore_finestra
fin.aggiungi_info_posizione_mesh(info_posizione(0,0,0,0,0,0)); 





//Aggiungo poi le mesh di oggetti del paesaggio 

/*-----PAVIMENTO--------------------------------------*/

//prevedo per il pavimento, essendo mesh senza texture, dei materials scelti da me
float diffpav[]={0.5,0.3,0.6}, ambpav[]={0.3,0.2,0.4}, specpav[]={0.6,0.4,0.7};
//lancio la routine che mi crea oggetto mesh leggendo da file, per il pavimento
mesh *mesh_pavimento=generamesh_dafile_wavefrontobj_senzanormali_senzatexture(string("mesh/livell.obj"),20,diffpav,ambpav,diffpav);
//lo salvo nel gestore finestra
fin.aggiungi_mesh(mesh_pavimento);
//creo oggetto info_posizione per la posizione e l'orientamento del pavimento
fin.aggiungi_info_posizione_mesh(info_posizione(0,0,0,0,-20.0,0));


/*---SEDIA----*/
mesh *mesh_sedia=generamesh_dafile_wavefrontobj_senzanormali_contexture("mesh/sedia.obj","mesh/sedia.mtl",20.0,fin.texthandlers, &fin.indtexthandlernonusato); /*in realtà l'actual type della variabile mesh_sedia è la classe mesh_contextures, il formal type è mesh */
fin.aggiungi_mesh(mesh_sedia);
fin.aggiungi_info_posizione_mesh(info_posizione(0,0,0,-10,500.0,-10));


/*---AUTO----*/
mesh *mesh_car=generamesh_dafile_wavefrontobj_senzanormali_contexture("mesh/car.obj","mesh/car.mtl",10.0,fin.texthandlers, &fin.indtexthandlernonusato); /*in realtà l'actual type della variabile mesh_sedia è la classe mesh_contextures, il formal type è mesh */
fin.aggiungi_mesh(mesh_car);
fin.aggiungi_info_posizione_mesh(info_posizione(0,0,0,-100,2000,20));












/*-------------------------_PER LE LUCI-----------------*/
/*La prima aggiunta, quella quindi che finirà nel vector<info_luce> luci in indice 0, deve essere sempre quella che
è proiettata dalla nostra arma, di tipo spotlight, bianca */
info_luce lucearma(GL_LIGHT1,true,true,0,0,0,1,1,1,0,1,1,0,1,1,0,15,0,0,-1,.5);
/*la creo che punta, come in realtà già di default, verso -1 (la posiziono un po' in avanti perchè idealmente dovrebbe stare sul fucile */
fin.aggiungi_luce(lucearma);

/*La luce in indice 1 è sempre quella proiettata dal peer. Ora siccome non sappiamo a priori dove si troverà il peer, come coordinate posizionali alla luce, passiamo soltanto quelle che ci permettono, una volta centrato il sistema di riferimento nella posizione in cui si trova il peer, di rappresentare la luce come se uscisse dalla sua arma. E quindi caratteristiche identiche a quelle della luce della nostra arma */
info_luce lucepeer(GL_LIGHT2,true,true,0,0,0,1,1,1,0,1,1,0,1,1,0,15,0,0,-1,.5);
fin.aggiungi_luce(lucepeer);


info_luce luce(GL_LIGHT3,true,false,0,50,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0); //luce ACCESA, posizionale NON SPOTLIGHT,  totalmente bianca
fin.aggiungi_luce(luce);


/*PER LE NURBS (che usano un vector a parte per gli oggetti nurbs e un vector a parte per gli info posizione*/

GLfloat cp[4*4*3]={-2,-2,-2,-2,-2,-2,-2,1,-2,-2,2,-2,
			-1,-2,-2,-1,-1,2,-1,1,2,-2,2,-2,
			1,-2,-2,1,-1,2,1,1,2,1,2,-2,
			2,-2,-2,2,-1,-2,2,1,-2,2,2,-2};
GLfloat knotsv[]={0,0,0,0,1,1,1,1};
NurbsNonOmogenea nurbs(3,3,4,4,cp,knotsv,knotsv);
fin.aggiungi_CurvaNurbs(nurbs);
fin.aggiungi_info_CurvaNurbs(info_posizione(-90,22,0,-5,0.0,-20));


//ora che ho aggiunto tutto quello che dovevo aggiungere come oggetti (mesh) e info oggetti (corrispondenti) posso far partire il glutMainLoop
fin.start_glutMainLoop();







}
