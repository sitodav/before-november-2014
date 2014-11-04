#include <vector>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include <map>
#include <utility>


using namespace std;



class faccia{
public:
	
	int nvertici;
	int *indici;
	float normale[3];
	
		/*usati... solo dalle mesh con textures*/
	string chiavetexture; 
	int *indici_textures;
	
	faccia(int *indi,int nverti,float *normal); /*costruttore per faccia senza text..*/
	faccia(int *indi,int nverti,float *normal,int *inditext,string chiavetext); /*costruttore per faccia con texture...*/


};




class mesh{
//questa classe mantiene un array di punti, che sono tutti gli univoci usati dalla mesh. E quando parte il costruttore di un oggetto mesh si passano solo questi.
//Poi si usa un metodo per aggiungere facce alla mesh (che vengono messe in un vector), passando al metodo della classe mesh l'array contenente gli indici (sull'array di punti univoci) dei punti che compongono la faccia, e tale array viene poi girato alla faccia (assieme alla normale, di quella faccia, calcolata nell'oggetto mesh, in quanto l'oggetto mesh puo' ottenere la corrispondenza tra indici e punti). In modo tale che l'oggetto classe mesh mantiene tutti i punti univoci, e tutte le facce, e ogni faccia contiene gli indici dei punti che la compongono e i valori della normale.
//Quindi nella classe mesh viene usato un metodo disegna_mesh che scorre sulle facce, e disegna ogni faccia, impostando il valore della normale
//CORREZIONE: IL COSTRUTTORE PRENDE IN INPUT ANCHE LE COMPONENTI MATERIALS, SCELTE IN INPUT DALL'UTENTE E NON LETTE DA FILE (PER COLORARE LA MESH IN MANCANZA D'ALTRO)

private: //queste 3 vanno private perchè non vogliamo che la classe mesh_contexture (che eredita da mesh) possa potervi accedere, poichè una mesh con texture ha componenti materials per ogni faccia. Una mesh senza texture ne ha per l'intera mesh
float compspeculare_generalemesh[3],compambientale_generalemesh[3],compdiffusiva_generalemesh[3];

protected:
	float **pti_univoci;
	int npunti;
	vector<faccia> facce;
	mesh(float **pti,int npunt,float scalefact);	//COSTRUTTORE RICHIAMABILE SOLO DALLE CLASSI FIGLIE, poichè chi crea dall'esterno una mesh senza texture, deve passare le componenti materials.
	void newell(int *indici,int nvertici,float *temp);
	

public:
	
	mesh(float **pti,int npunt,float scalefact,float compspec[],float compamb[],float compdiffusiva[]); 
	//il motivo per cui uso 2 costruttori (sopra) è che il primo deve essere usato dalla classe meshcontexture quando si costruisce mesh con textures (in quanto non c'e' bisogno di inizializzare i campi dei materials generali poichè ogni faccia ha i suoi letti da file) mentre il secondo viene chiamato quando si crea mesh senza texture (poiche passiamo al costruttore i colori generali dei materials)

        void push_faccia(int *indici,int nvertici); /*questa non c'è bisogno di farla virtual poichè c'è overloading in base al tipo e numero parametri in input*/
	virtual void mi_disegno(); /*virtual perchè...*/
	void check_errori();

};

//nell'oggetto di classe gestore_finestra viene mantenuto un vector di mesh, e la classe gestore_finestra mantiene i rispettivi oggetti di classe info_posizione che indicano posizione/rotazione di ciascuna mesh
//nella classe gestore_finestra ci sono i metodi per aggiungere nuove mesh alla lista di oggetti




class Texture{
public:
	GLuint nomehandler;
	float compdiffusiva[3];
	float compambientale[3];
	float compspeculare[3];
	Texture(){}
	Texture(GLuint hand,float amb[],float diff[],float spec[]){
		nomehandler=hand;
		compdiffusiva[0]=diff[0]; compdiffusiva[1]=diff[1]; compdiffusiva[2]=diff[2];
		compambientale[0]=amb[0]; compambientale[1]=amb[1]; compambientale[2]=amb[2];
		compspeculare[0]=spec[0]; compspeculare[1]=spec[1]; compspeculare[2]=spec[2];	
	}
};






/*la classe mesh con textures estende la classe mesh di base (che non gestisce le textures). In questa classe figlia aggiungiamo le strutture dati e la logica necessaria per disegnare le textures. 
Siccome nella struttura dati gestore_finestra, il vector delle mesh è di tipo vector<mesh> quindi puo' contenere anche oggetti di tipo mesh_contextures essendo questa figlia di mesh. Tuttavia per far sì che dal punto di vista del gestore finestra, non faccia differenza se sta lanciando lanciando il metodo mi_disegno per una mesh o per una mesh_contextures nella classe mesh_contextures viene fatto overriding del metodo mi_disegno della madre. Siccome pero' l'actual type per il vector<mesh> è appunto mesh, ed essendo in c++ il binding early, cioè a compile time, per far si' che parta il metodo mi_disegno della classe figlia nel caso un oggetto sul vector abbia actual tipe mesh_contextures, dobbiamo definire il metodo virtual nella madre (oltre a dover far si' che il vector sia di puntatori a mesh e non di mesh, altrimenti avviene upslicing e si perde il late binding nonostante il metodo sia virtual).*/

/*Nel file .obj ci sono anche i punti univoci relativamente alle textures. Questi non sono direttamente associati alla textures, ma è la faccia, che è associata ad una textures, che è indicata tramite gli indici (oltre che dei punti spaziali classici) dei punti univoci della textures. Quindi per ogni mesh devo mantenere la lista anche dei vertici UNIVOCI di texture. Poi ogni mesh usa più textures. Per ogni faccia quindi devo mantenere la corrispondenza con la textures di quella faccia (e nella mesh manterro' la corrispondenza tra nome texture e handler su cui si sarà bindata ...) Una faccia avrà 3 vertici, e ogni vertice ha associato l'indice sulla lista punti univoci SPAZIALI e l'indice sulla lista punti UNIVOCI TEXTURES.*/



/*NB:per ogni mesh con textures, ci salviamo quindi tutti i punti univoci di textures presenti nel file, indipendentemente dalla textures alla quale si riferiscono, tutti li mettiamo nella stessa struttura. Quando poi dovremo disegnare una faccia, lì vedremo qual è l'oggetto Texture associato che contiene informazione sui materials ed eventualmente (se ne ha una) handler texture che useremo per ricaricare (con la glBind) la texture vera e propria associata che disegneremo disegnando (con la funzione apposita) ciascuno dei punti texture (ai quali accediamo usando gli indici memorizzati nella faccia) */

class mesh_contextures: public mesh{
	
	float **pti_univoci_textures; /*tutti i punti univoci (NB BIDIMENSIONALI) usati dalle texture delle facce di questa mesh (NB UNA MESH PUO' USARE PIU' TEXTURES)*/
	int npuntitextures; /*num dei punti di cui sopra*/
	map<string,Texture> textures; /*usiamo una struttura hash per trovare, dato il nome di una texture associata ad una faccia, il valore dell'handler su cui è bindata la texture relativa*/

public:
	mesh_contextures(float **pti,int npunt,float scalefact,float **ptitext,int nptitext,map<string,Texture> maptocopy);	/*fa partire il costruttore della classe mesh normalmente, ed in piu' si salva i punti univoci delle textures ed il loro numero, e la mappa con le corrispondenze tra nometexture e oggetto classe Texture*/

	
	
	/*overload*/
	void push_faccia(int *indici,int nvertici,int *indicitexture,string chiavetexture);
	/*override in late binding*/
 	void mi_disegno();
	
	
	 	
};









