#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


class NurbsNonOmogenea{
	GLUnurbsObj *pnurbs;
	int gradonurbsu;
	int gradonurbsv;
	int num_righe_reticolo_cp;
	int num_colonne_reticolo_cp; /*siccome stiamo lavorando con array allocati dinamicamente, quindi
	monodimensionali, si sta simulando array allocati staticamente, quindi noi avremo un unico array 	monodimensionale allocato dinamicamente. In questo caso i parametri num_colonne etc serviranno 		per il calcolo degli stride*/
	
	int num_knotsu;
	int num_knotsv;
	GLfloat *reticolo_cp_cartesiano;
	GLfloat *knotsu,*knotsv;
	/*attenzione che il reticolo va allocato dinamicamente, poichè ovviamente
	solo in ingresso avremo i parametri relativi al size etc, pero' non possiamo
	usare allocazione tramite array di puntatori a puntatori a puntatori (matrice 3d
	poichè a noi serve che l'allocazione sia tutta continua come se fosse un array 
	statico 3d visto che dovrà essere usato con gli stride dalle routine di opengl
	Quindi dobbiamo allocare come un unico array monodimensionale (Quindi continuo)
	*/
public:
	NurbsNonOmogenea(){}
	NurbsNonOmogenea(int gradou, int gradov,int numrighecp,int numcolcp,GLfloat *reticolocpcartesiano,GLfloat *knu,GLfloat *knv);
	/*per semplicità presupponiamo che in input venga passato l'array del reticolo di cp (cartesiano) già come array monodimensionale, quindi lungo num_righereticolo*num_colonnereticolo*3*/
	void mi_disegno();
};
