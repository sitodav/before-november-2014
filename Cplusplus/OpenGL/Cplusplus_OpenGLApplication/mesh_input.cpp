#include "mesh_input.h"
#include <fstream>
#include <cstring>
#include <sstream>
#include <errno.h>

using namespace std;



mesh *generamesh_dafile_senzanormali_senzatexture_mioformato(string path,float scale,float compspec[],float compamb[],float compdiffusiva[]){
//legge una mesh da file (formato mio)come semplice insieme di punti univoci e una serie di facce, dove ogni faccia è rappresentata da un insieme di indici relativi ai punti univoci che la compongono. Dal file non vengono lette informazioni relativamente alle normali, o alla texture della mesh.


ifstream file(path);
if(file.fail()){
		perror("");
		exit(1);
}
string s;
stringstream ss;
int num_puntiuniv;

//leggo il numero di punti "univoci", che è il primo valore del file
getline(file,s); //con la getline prendo dal file (ifstream) e lo metto in un oggetto string
num_puntiuniv=atoi(s.c_str()); //prendo l'array di char dall'oggetto di classe string, e ci applico atoi
//alloco la struttura contenente i punti univoci, da passare al costruttore della classe mesh
float **puntiuniv=(float **)new float *[num_puntiuniv];
//e riempio
for(int i=0;i<num_puntiuniv;i++){
	puntiuniv[i]=(float *)new float[3]; //alloco riga dell'array 2d
	getline(file,s); //leggo una nuova riga del file
	//l'oggetto di classe stringstream ss che utilizzo come appoggio, viene ripulito
	ss.str(string());
	ss.clear();
	//metto la string letta nella stringstream di appoggio
	ss<<s;

	//la getline con il delimitatore ',' in input, legge ogni volta fino al delimitatore
	//quindi leggo, dalla stringstream di sopra, ogni ',' mettendo in una string, e trasformo in 
	//float
	string t;
	float r;
	int j=0;
	while(getline(ss,t,',') && j<3){
		puntiuniv[i][j++]=atof(t.c_str()); //nb l'atoi lavora con array di char, quindi 					estraiamo tramite c_str() l'equivalente dalla stringa
	}
	
	
}

//arrivati a questo punto la struttura (array di punt ad array di float) è riempito con le coordinate dei punti univoci (ogni riga un punto, e ogni colonna le coordinate, 3 colonne, x,y,z)

//Quindi posso iniziare ad allocare l'oggetto di classe mesh poichè il costruttore prende in input soltanto la struttura dei punti univoci ed il numero di questi
mesh *miamesh=(mesh *)new mesh(puntiuniv,num_puntiuniv,scale,compspec,compamb,compdiffusiva);

//ora dobbiamo continuare a leggere dal file le facce, e aggiungerle di volta in volta
//il prossimo elemento sul file è il numero di facce
getline(file,s);
int num_facce=atoi(s.c_str());
for(int i=0;i<num_facce;i++){
	getline(file,s); //leggo riga successiva file e metto in una string
	//pulisco la stringstream di appoggio
	ss.str(string());
	ss.clear();
	//metto il contenuto della string nella stringstream di appoggio	
	ss<<s;
	//leggo dalla stringstream come stream, sapendo che il numero prima del '-' indica il numero
	//di vertici
	getline(ss,s,'-');//quindi leggo dalla stringstream fino al separatore, e metto in una string
	int num_verticifaccia=atoi(s.c_str()); //e trasformo, l'array di char preso dalla string, in int
	//alloco il vettore di int, contenente indici, relativi ai punti univoci, della faccia, da 		//passare al metodo push_faccia della classe mesh 
	int *indici=(int *)new int[num_verticifaccia];
	//devo ora leggere i numeri rimanenti sull'ultima riga letta, poichè quelli sono gli indici
	for(int j=0;j<num_verticifaccia;j++){
		getline(ss,s,',');
		indici[j]=atoi(s.c_str());	
	}
	//e pusho la faccia nella mesh, che automaticamente si calcolerà usando newell le normali
	//e se la salverà nel vector di facce
	miamesh->push_faccia(indici,num_verticifaccia);
}

return miamesh;


}






/*_----------------------------------------------------------------__*/

mesh *generamesh_dafile_wavefrontobj_senzanormali_senzatexture(string path,float scale,float spec[],float amb[],float diff[]){
//la nostra classe mesh per essere istanziata necessita che al costruttore si passi un punt ad array di punt
//di tipo float che rappresenti tutti i punti distinti, e il numero di tali punti
//Quindi scorriamo sul file e prendiamo tutti i vertici (punti distinti) che nel formato .obj si trovano
//su una line che inizia per v


string line; //ci salvo le righe che leggo
stringstream t; //passo le righe che leggo in questa stringstream di appoggio, per usarla come stream successivo di lettura
float **punti_univoci;	
int numpunti=0;

//apro file
ifstream file(path);
if(file.fail()){
	perror("");
	exit(1);
}


//Siccome a differenza del formato definito da noi, nell'.obj non c'è all'inizio un modo per sapere quanti sono i punti distinti, prima scorriamo e li contiamo
while(getline(file,line,'\n'))
	if(line[0]=='v' && line[1]==' ')
		numpunti++;

//risetto la posizione dell'offset dello stream all'inizio:
//per farlo devo:
file.clear();//va messa altrimenti se non puliamo l'EOF flag comunque crede di stare alla fine anche se abbiamo spostato all'inizio offset ed inoltre va messo PRIMA !!
file.seekg(0,ios::beg); //offset


punti_univoci=(float **)new float *[numpunti];
int indpunto=0;
int indcord;

while(getline(file,line,'\n')){
	if(line[0]=='v' && line[1]==' '){ //se è una riga per un punto univoco
		punti_univoci[indpunto]=(float *)new float[3]; //alloca quella riga sull'array
		//ora dobbiamo fare il parsing della riga per prenderci i 3 float che indicano xyz
		//uso lo stringstream come stream di appoggio, e prima devo pulirlo
		t.str(string());
		t.clear();
		t.str(line); //metto il contenuto della riga letta dalla string nella stringstream
		//e lavoro sulla stringstream come uno stream di lettura in input (per valori float)
		
		//per fare il parsing dei 3 float dalla stringstream:
		//devo saltare la prima parte della riga "v "
		t.seekg(2,ios::beg);

		indcord=0;
		while(indcord<3) //leggo i 3 float dalla stringstream
			t>>punti_univoci[indpunto][indcord++];
		
		indpunto++;
	}
}


//lancio il costruttore passandogli anche i materials generali (visto che stiamo in una mesh senza texture)
mesh *miamesh=new mesh(punti_univoci,numpunti,scale,spec,amb,diff);

/*ora devo leggere le facce dal file
Non ho bisogno di conoscere a priori il numero di facce, visto che la classe mesh permette di aggiungere una faccia alla volta*/

//resetto l'offset (dopo aver fatto il clear obbligatorio) sul file
file.clear();
file.seekg(0,ios::beg);

/*il formato della faccia negli obj è ad esempio:
f 1/2/8 3/13/8 4/14/8

f indica faccia, le tre triple a/b/c sono relativi ai 3 vertici del triangolo che è la faccia,
a è l'indice del punto univoco che rappresenta il vertice
b è  l'indice della texture (????)
c è l'indice della normale associata a quel vertice

attenzione: gli indici sono 1-biased negli .obj files, a noi servono 0 biased

per ogni riga che inizi con "f " quindi a noi interessano gli a di ciascuna delle tre triple */
int indici[3];
stringstream token;
string tripla;

while(getline(file,line,'\n')){

	if(line[0]=='f' && line[1]==' '){
		t.clear();
		t.str(line); //è equivalente a t.str(string()) seguito da t<<line
		//salto "f "
		t.seekg(2,ios::beg);
	
		int numind=0;
		while(getline(t,tripla,' ')){ //itererà 3 volte, perchè tre triple separate da ' '
			/*ho estretto una tripla
			ora devo prendere il primo numero prima di /	
			*/	
			token.clear();
			token.str(tripla);
			getline(token,tripla,'/'); /*una singola chiamata a getline mette in tripla quello che viene prima di / nella tripla sopra estratta. Ora dobbiamo estrarre 				l'intero dalla stringa tripla, quindi*/			
			sscanf(tripla.c_str(),"%d",(indici+numind));	
			indici[numind]-=1; //poichè sono 1 biased i valori degli indici nel formato .obj e a noi servono 0 biased
			
			numind++;		
		}
		
		miamesh->push_faccia(indici,3); //le facce hanno sempre 3 vertici poichè triangoli sempre
		
	}

}


//a questo punto abbiamo la mesh pronta! 
return miamesh;


}














mesh_contextures * generamesh_dafile_wavefrontobj_senzanormali_contexture(string pathobj,string pathmtl ,float scale,GLuint handlers[],int * last){
//la nostra classe mesh per essere istanziata necessita che al costruttore si passi un punt ad array di punt
//di tipo float che rappresenti tutti i punti distinti, e il numero di tali punti, oltre al fattore di scala. 
/*Essendo questo un parser che considera anche info sulle texture, dovremo passare al costruttore di oggetto di classe mesh_contextures oltre ai parametri elencati sopra,anche un puntatore ad array di puntatori (ad array di 2 elementi) di float, contenenti le coordinate di tutti i punti UNIVOCI di textures, ed il numero di tali punti. 
Inoltre dobbiamo passargli una map<string,Texture> contenente elementi di classe Texture associati ad una stringa. La stringa (chiave della map) è il nome della texture (quello usato nel .obj) e l'oggetto di classe Texture associato, contiene informazioni sui materials (ambientale/diffusiva/speculare) e il valore di un handler che ci dice se c'è effettivamente una texture in senso di immagine registrata (bindata) su quell'handler, oppure se nel .mtl il riferimento a quella texture è soltanto per dei valori di materials e non c'è nessuna vera texture.*/


string line; //ci salvo le righe che leggo
stringstream t; //passo le righe che leggo in questa stringstream di appoggio, per usarla come stream successivo di lettura
stringstream t2;
string nometext,pathtext;
float **punti_univoci,**punti_univoci_textures;	
int numpunti=0,numpuntitextures=0;
map<string,Texture> mappa;

/*ci salvo info delle immagini che leggo */
GLubyte *image; 
GLsizei	width, height;
int imageWidth, imageHeight, components;
char *path;
float temp[3][3]; //per salvare le componenti ambient/diff/spec che leggo da file per la texture



/*per prima cosa apro il file .mtl (di cui mi viene fornito in input il path) che contiene per ogni nome di texture usato nel .obj, le componenti ambientale/diffusiva/speculare +  SE ESISTE il path dell'immagine associata ()...Quindi uso le info in questo file per riempire la map<string,Texture> con oggetti che associano al nome di texture (usato nel .obj e nel .mtl) un oggeto Texture che contiene componenti ambientale/diffusiva/speculare e l'handler con un valore che indica se c'è registrata sopra una texture vera e propria o no (se no ha valore -1)*/

//apro file .mtl
ifstream filemtl(pathmtl);
if(filemtl.fail()){
	perror("");
	exit(1);
}

//scorro il file, leggendo una riga per volta, e individuando le righe che iniziano per "newmtl"
while(getline(filemtl,line,'\n'))
	if(line.compare(0,6,"newmtl")==0){ /*trovata riga che inizia per "newmtl"*/
		/*devo aggiungere un oggetto alla map<string,Texture>*/	
		/*prima pero' devo ottenere il nome della texture, che sta dopo "newmtl " e li salvo in 
		nome text*/
		
		nometext=line.substr(7,string::npos); /*prendi tutti i char dal 7 alla fine, e questo è il nome della texture*/
		
		/*le 3 righe che seguono ora sono le componenti (rgb) ambientale diffusiva e speculare*/
		/*le riga successiva è sempre fatta di 3 float che riguardano l'rgb della componente diffusiva*/
		for(int i=0;i<3;i++){
			getline(filemtl,line,'\n');
			/*devo fare il parsing della stringa line*/
			/*salto le prime 3 lettere che non mi servono*/
			line=line.substr(3,string::npos);
			/*mi trasformo il contenuto di line in uno stream (mettendolo in una stringstream)*/
			t2.clear();
			t2.str(line);
			for(int j=0;j<3;j++){
				t2>>temp[i][j];
			
			}
			
					
		}
		/*quindi nella prima riga di temp avro' la componente ambientale, nella seconda la diffusiva e 
		nella terza la speculare*/
		

		/*A QUESTO PUNTO, A QUESTA ENTRY PUO' ESSERE ASSOCIATA UN'IMMAGINE (QUINDI TEXTURE STRETTA)
		OPPURE NO. NEL SECONDO CASO ALLORA POSSIAMO GIA' INSERIRE L'OGGETTO DI CLASSE TEXTURE NELLA MAPPA
		FACENDO ATTENZIONE AD INDICARE CHE IN REALTÀ NON VI È ASSOCIATA UN'IMMAGINE TEXTURE (METTENDO
		L'HANDLER A -1) SE INVECE C'E' IL PATH DELL'IMMAGINE COME RIGA SUCCESSIVA NEL .MTL, DOBBIAMO
		APRIRE L'IMMAGINE, REGISTRARE UN NUOVO HANDLER etc */
		/*Leggo quindi la riga successiva*/
		getline(filemtl,line,'\n');
		
		if(line.compare(0,6,"map_Kd")!=0){ //una riga indicante il path di un'immagine inizia con map_kd 
			/*allora non c'è il path immagine, quindi non vi è associata un'immagine a questa entry*/
			/*quindi creo e inserisco oggetto di classe Texture nella mappa*/
			mappa[nometext]=Texture(-1,temp[0],temp[1],temp[2]);
			/*si noti che ho passato -1 come valore dell'handler, ad indicare che non vi è immagine
			associata, e ho passato poi i 3 vector di componenti di luce*/
			continue;		
		}

		/*altrimenti se arriva qui vi è path immagine associato, e questo path sta in line*/
		
		/*copio la riga nello stringstream*/
		t.clear();
		t.str(line);
		/*uso la stringstream come buffer, leggendo fino al primo spazio, che è una parte
		che non mi serve...*/
		getline(t,line,' ');
		/*se ora rilancio la getline, viene presa la parte rimanente della riga, che è il path
		di quella texture*/
		getline(t,pathtext,'\n');
		
		pathtext=string("mesh/")+pathtext; /*i path contenuti nel .mtl file sono a partire dalla cartella in cui sono contenuti. Quindi mettendoli nella cartella mesh, dobbiamo aggiungere "mesh/" alla parte di path dell'immagine che leggiamo nel .mtl*/

		path=(char *)new char[pathtext.length()+1];
		strcpy(path,pathtext.c_str());
		path[strlen(path)-1]='\0';
		
  		/* Uso la routine per leggere immagine da file */
		image = (GLubyte *) read_texture(path, &imageWidth, &imageHeight, &components);
	
		/*eseguo il bind prendendo il primo handler NON ancora registrato, dalla lista degli handler precedentemente attivati nel metodo di init dell'ambiente di opengl, nell'oggetto gestore_finestra*/
		glBindTexture(GL_TEXTURE_2D,handlers[*last]);
		
		/*posso incapsulare tutto nell'oggetto di classe Texture e aggiungerlo alla mappa*/
		mappa[nometext]=Texture(handlers[*last],temp[0],temp[1],temp[2]);
		/*si noti che ho passato come valore dell'handler all'oggetto Texture, quello che ho registrato*/

		(*last)++; /*incremento il valore dell'indice ultimo handler non ancora registrato (attenzione che questo int sta nell'oggetto classe gestore_finestra, e lo modifichiamo tramite punt */
		
		/*creiamo il MipMaps automatico con autoscale in potenza di 2*/
		gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,imageWidth,imageHeight,GL_RGBA,GL_UNSIGNED_BYTE,image);		
		
		/*a questo punto è stata registrata una texture (facilemnte ricaricabile in fase di disegno) su quell'handler, al quale è associato (tramite la mappa) il nome della texture (che è usato nel .obj
		e nel .mtl)*/
		
	}


/*Apro il file .obj*/

ifstream file(pathobj);
if(file.fail()){
	perror("");
	exit(1);
}

/*scorro il file, contando il numero di punti SPAZIALI univoci ed il numero di punti TEXTURE univoci*/


while(getline(file,line,'\n'))
	if(line[0]=='v' && line[1]==' ')
		numpunti++;
	else if(line[0]=='v' && line[1]=='t')
		numpuntitextures++;

/*alloco gli array che li conterranno (numpunti x 3 come size di quello dei punti spaziali, numpuntitextures*2 per quelli textures)*/

punti_univoci=(float **)new float *[numpunti];
for(int i=0;i<numpunti;i++)
	punti_univoci[i]=(float *)new float[3];

punti_univoci_textures=(float **)new float *[numpuntitextures];
for(int i=0;i<numpuntitextures;i++)
	punti_univoci_textures[i]=(float *)new float[2]; /*2 perchè i punti sono in coordinate 2d*/




/*riazzero l'offset sul file*/
file.clear();
file.seekg(0,ios::beg);

/*scorro di nuovo sul file, prendendo le righe relative a punti univoci spaziali o di textures, e li metto nel relativo array*/

int indpunto_spaz=0,indcord_spaz=0;
int indpunto_text=0,indcord_text=0;

while(getline(file,line,'\n')){
	if(line[0]=='v' && line[1]==' '){
		
		/*riga per punto univoco spaziale*/
		//dobbiamo fare il parsing della riga per prenderci i 3 float che indicano xyz
		//uso lo stringstream come stream di appoggio, e prima devo pulirlo
		t.str(string());
		t.clear();
		t.str(line); //metto il contenuto della riga letta dalla string nella stringstream
		//e lavoro sulla stringstream come uno stream di lettura in input (per valori float)
		
		//per fare il parsing dei 3 float dalla stringstream:
		//devo saltare la prima parte della riga "v "
		t.seekg(2,ios::beg);

		indcord_spaz=0;
		while(indcord_spaz<3) //leggo i 3 float dalla stringstream
			t>>punti_univoci[indpunto_spaz][indcord_spaz++];
		
		
		indpunto_spaz++;
		
		
	}
	else if(line[0]=='v' && line[1]=='t'){
		/*punto univoco texture*/	
		/*tutto come per il punto univoco spaziale, solo che sono solo 2 le coordinate da 			prendere*/
		//dobbiamo fare il parsing della riga per prenderci i 3 float che indicano xyz
		//uso lo stringstream come stream di appoggio, e prima devo pulirlo
		t.str(string());
		t.clear();
		t.str(line); //metto il contenuto della riga letta dalla string nella stringstream
		//e lavoro sulla stringstream come uno stream di lettura in input (per valori float)
		
		//per fare il parsing dei 3 float dalla stringstream:
		//devo saltare la prima parte della riga "v "
		t.seekg(2,ios::beg);

		indcord_text=0;
		while(indcord_text<2) //leggo i DUE float dalla stringstream
			t>>punti_univoci_textures[indpunto_text][indcord_text++];
		
		
		indpunto_text++;
	}

}



/*a questo punto ho :
	-tutti i punti SPAZIALI univoci 
	-tutti i punti TEXTURE univoci
	-i nomi delle texture usate dalla mesh, a cui sono associati gli handler di texture (su cui abbiamo bindato texture caricate in memoria)

A QUESTO PUNTO POSSO GIA' ISTANZIARE L'OGGETTO MESH_CON_TEXTURE, IL QUALE COSTRUTTORE PRENDE IN INPUT L'ARRAY DI PUNTI UNIVOCI SPAZIALI, IL NUMERO DI QUESTI, IL FATTORE DI SCALA DELLA MESH, L'ARRAY DI PUNTI UNIVOCI DI TEXTURES, IL NUMERO DI QUESTI PUNTI, E LA MAPPA CHE CONTIENE ASSOCIAZIONE TRA NOME TEXTURE CON oggetto Texture*/

mesh_contextures *miamesh=(mesh_contextures *)new mesh_contextures(punti_univoci,numpunti,scale,punti_univoci_textures,numpuntitextures,mappa);



/*Ora leggo dal file tutte le righe riguardanti le facce, che contengono 3 triple, dove ogni tripla è relativa ad un vertice della faccia triangolare, e i 3 elementi della tripla sono nell'ordine indice di punto UNIVOCO spaziale, indice di punto UNIVOCO di texture, ed il terzo indice di punto univoco di normale (non ci serve). Come texture associata alla faccia, si usa l'ultima texture riferita con 
"usemtl nometexture" prima della riga della faccia


/*riazzero lo stream*/
file.clear();
file.seekg(0,ios::beg);


string lastNomeText,tripla;
int indici[3],indici_text[3],indind=0;
stringstream token;


while(getline(file,line,'\n')){
	/*devo salvarmi ogni nuova occorrenza di una riga che inizia con "usemtl nometexture" perchè a quella texture si riferiranno le successive facce. 
	*/
	if(line.compare(0,6,"usemtl")==0){
		lastNomeText=line.substr(7,string::npos);
		continue;
	}
	/*quindi da questa iterazione in poi, tutte le facce che incontreremo (come righe) useranno
	la texture il cui nome sta in lastNomeText*/
	
	if(line[0]=='f'){
		
		/*riga di faccia*/
		line=line.substr(2,string::npos); /* salto "f " e mi piazzo all'inizio prima tripla*/
		
		/*metto il contenuto della stringa line, in una stringstream, in modo tale da usarla 
		come stream di lettura con la getline*/
		t.clear();
		t.str(line);
		
		indind=0;
		while(getline(t,tripla,' ')){ //itera solo 3 volte
			sscanf(tripla.c_str(),"%d/%d/",&indici[indind],&indici_text[indind]);
			indici[indind]-=1; indici_text[indind]-=1; //poichè a noi servono 0 biased
			indind++;	
		}
		
		
		/*quindi per la faccia trovata, ho gli indici spaziali e quelli di texture per ciascuno 
		dei vertici, ed ho inoltre il nome della texture che usa. Posso quindi inserire nuova 			faccia sull'oggetto di classe mesh che sto preparando, passando i 2 vettori 
		di indici, il numero di indici (sempre 3 in questo caso) e il nome della texture 
		usata da quella faccia*/
		
		miamesh->push_faccia(indici,3,indici_text,lastNomeText);
		/*Il metodo push_faccia di un oggetto di class mesh_contextures a sua volta lancia
		costruttore della classe faccia (overloading del costruttore) e la mette sul suo vector 
		interno*/
	
	}

}


/*a questo punto ho una mesh contenente:
	-tutti i punti SPAZIALI univoci (coordinate xyz)
	-tutti i punti TEXTURE univoci (coordinate xy)
	-una mappa che associa ad ogni nome texture un oggetto di classe Texture che contiene componenti ambientale/etc e un handle rsu cui è eventualmente bindata una texture vera e propria
	-un vector di facce ognuna contenente:
		vettore di indici per il disegno dei vertici spaziali
		vettore di indici per il disegno mesh
		il nome della texture relativo alla faccia
		normali
posso quindi ritornare la mesh*/
return miamesh;


}

















