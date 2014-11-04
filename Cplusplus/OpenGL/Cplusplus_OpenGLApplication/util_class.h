#include "network.h"
#include "mesh_input.h"
#include "nurbs.h"
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <vector>
#define PI 3.1415


class info_luce{
	public:
	float coord[4];
	float speccomp[3];
	float diffcomp[3];
	float ambientcomp[3];
	float spotcutoff;
	float spotdircomp[3];
	float spotexponent;
	bool spot;
	bool on;
	
	GLenum id;
	
	info_luce(){};
	/*il costruttore della luce prende in input:
	-identificativo luce di tipo GL_ENUM (GL_LIGHT1 etc), booleano che indica se è on, booleano che indica se è 
	di tipo spotlight, coordinate omogenee xyzw dove w se è 0 indica luce direzionale, se 1 indica posizionale
	ovviamente se indichiamo che deve essere spotlight deve essere anche posizionale, e poi tutti i parametri 
	per la luce, quindi rgb per componenti speculare, diffusiva, ambientale, cutoff direzione e exponent nel 
	caso di luce spotlight*/
	info_luce(GLenum ident,bool ison,bool isspot,float x,float y,float z,float w,float a,float b,float c,float d,float e,float f,float g,float h,float i,float l,float m,float n,float o,float p){coord[0]=x; coord[1]=y; coord[2]=z; coord[3]=w; speccomp[0]=a; speccomp[1]=b; speccomp[2]=c; diffcomp[0]=d; diffcomp[1]=e; diffcomp[2]=f; ambientcomp[0]=g; ambientcomp[1]=h; ambientcomp[2]=i;  id=ident; on=ison; spotcutoff=l; spotdircomp[0]=m; spotdircomp[1]=n; spotdircomp[2]=o; spotexponent=p; spot=isspot;}

};


/*----------------------------------------------------------------------------------*/





class info_posizione{
	public:
	float angle_vista_tox;
	float angle_vista_toy;
	float angle_vista_toz;
	float movimentoz;
	float movimentoy;
	float movimentox;

	info_posizione(){};	
	info_posizione(float angx,float angy,float angz,float movx,float movy,float movz) : angle_vista_tox(angx),angle_vista_toy(angy),angle_vista_toz(angz),movimentoy(movy),movimentoz(movz),movimentox(movx){}

};


//-----------------------------------------------------------------------------*
class gestore_gravita{
	vector<info_posizione> *posizioni; 
	info_posizione *mia_posizione;
	float *accelerazioni;
	float mia_accelerazione;

	public:
	gestore_gravita() {}
	gestore_gravita(vector<info_posizione> *pos,info_posizione *miapos): posizioni(pos),mia_posizione(miapos){
		accelerazioni=(float *)new float[posizioni->size()]();	
	}
	bool calcola_applica_gravita(); //la routine ritorna un bool, che è true se almeno un oggetto ha 		subito la gravità, in modo tale che il callback timer eviti di lanciare un postredisplay anche 		se nessun oggetto ha mutato la sua posizione
	void setta_accell_oggetto(int i,float val);
	void setta_miaaccel(float val);
	

};




//----------------------------------------------------------------------------*

//la classe gestore_finestra si occupa dell'inizializzazione della parte grafica relativa alla finestra, aggiorna la situazione utilizzando gli oggetti di network, e mantiene una lista di tutti gli oggetti
//di classe mesh.MENTRE LE INFO RELATIVE ALLA NOSTRA POSIZIONE/ORIENTAMENTO (usate da rototrasla_mondo) SONO CONTENUTE IN UN OGGETTO INFO_POSIZIONE SEPARATO DAL VECTOR INFO_OGGETTI IL PRIMO OGGETTO NEL VECTOR DI OGGETTI E' LA MESH RELATIVA ALL'ARMA, E IL CORRISPETTIVO PRIMO ELEMENTO DI INFO_OGGETTI E' L'INFO POSIZIONE DELL'ARMA.  IL SECONDO OGGETTO NEL VECTOR OGGETTI E' SEMPRE LA MESH DEL PERSONAGGIO DEL PEER, ED IL SECONDO OGGETTO DEL VECTOR INFO_OGGETTI CONTIENE LE INFO RELATIVE AL PERSONAGGIO DEL PEER.


/*INOLTRE C'È UN VECTOR DI OGGETTI INFO POSIZIONE PER LE SOLE NURBS, CON CORRISPONDENTE VECTOR DI OGGETTI DI CLASSE NURBS*/



class gestore_finestra{
	public:
	static vector<mesh *> oggetti; /*vector di puntatori a mesh e non mesh, altrimenti non funziona il latebinding sui metodi mi_disegno() di classi mesh e mesh_contextures */
	static vector<info_posizione> info_oggetti;
	static vector<info_luce> luci; //per le luci non c'è la divisione tra mesh e info mesh, ed inoltre non subiscono la gravità
	static vector<NurbsNonOmogenea> superficiNurbs;
	static vector<info_posizione> info_CurveNurbs; /*per le nurbs c'è divisione tra nurbs e oggetto 		info_posizione (che stanno in un vector dedicato) ma non subiscono la gravità, quindi non 		interessano al gestore gravità*/
	
		
	
	static gestore_rete &network; //devono essere per forza static perchè usati da funzioni statiche
	static info_posizione &myinfo;
	static gestore_gravita gravita;
	float fov;
	float znear;
	float zfar;
	int wwidth;
	int wheight;
	int *p_argc;
	char **argv;
	static char *keybuffer,*specialkeybuffer; //poichè usati da funzioni statiche
	static char *to_receive,*to_send;
	static float lungh_spostam;
	GLuint *texthandlers;
	int indtexthandlernonusato; //indice ultimo text handler non registrato ancora

	//statiche perchè le routine di opengl vogliono un puntatore a funzione, e in c++ i puntatori a funzioni di una classe non sono considerati come i puntatori a funzioni normali esterne a classi, a meno che queste non siano statiche
	static void callback_display();
	static void callback_specialk(int ,int ,int );
	static void callback_specialk_up(int, int, int);
	static void callback_keyboard(unsigned char ,int ,int );
	static void callback_keyboard_up(unsigned char,int ,int);
	static void callback_timer_socket(int val); 
	static void callback_timer_principale0(int val); //racchiude piu' funzioni di callback di timer nella stessa unica routine, per evitare di flooddare il peer	
	static void callback_timer_gravita(int val);
	static void check_errori();
	static void rototrasla_mondo(); //per muovere il mondo dell'opposto dei nostri movimenti/rotazioni di vista
	static void disegna_mondo(); //NON LA USO PIU (OBSOLETA)

	static void aggiungi_mesh(mesh * mesh_oggetto);
	static void aggiungi_info_posizione_mesh(info_posizione info); 
	//i metodi aggiungi_mesh e aggiungi_info_posizione_mesh vanno sempre chiamati assieme
	void aggiungi_luce(info_luce luce);
	//lo stesso per aggiungi_info_curvaNurbs e aggiungi_curvaNurbs
	void aggiungi_info_CurvaNurbs(info_posizione info);
	void aggiungi_CurvaNurbs(NurbsNonOmogenea curva);
	static void rototrasla_oggetto(info_posizione &info); //per traslare/ruotare sistema di riferimento, prima di disegnare un oggetto specifico, usando l'oggetto di classe info_posizione che indica 									orientamento e posizione oggetto rispettivo
	static void disegna_mirino(float raggio); //per il mirino non si usa la trafila delle mesh/oggetti info_posizione poichè tanto è una semplice croce, quindi si usa una routine apposita

	void start_glutMainLoop(); //la funzione a parte è necessaria, poichè se mettessimo il glutMainLoop nel costruttore(dove vengono fatte inizializzazioni gl) non avremmo poi la possibilità di aggiungere 						//mesh ed info oggetti
	

	
	gestore_finestra(){};
	gestore_finestra(gestore_rete &netw,info_posizione &myinf,float fv,float znr,float zfr,float wwid,float whei,int *pargc, char **argv);
	
	


};
