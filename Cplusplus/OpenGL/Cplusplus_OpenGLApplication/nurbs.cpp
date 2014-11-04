#include "nurbs.h"

NurbsNonOmogenea::NurbsNonOmogenea(int gradou, int gradov,int numrighecp,int numcolcp,GLfloat *reticolocpcartesiano,GLfloat *knu,GLfloat *knv){

	gradonurbsu=gradou;
	gradonurbsv=gradov;
	num_righe_reticolo_cp=numrighecp;
	num_colonne_reticolo_cp=numcolcp;
	/*ricorda che in realtà l'array allocato dinamicamente del reticolo è monodimensionale.
	I parametri del num_righe_reticolo e colonne servono per il calcolo degli stride e per gli 
	accessi simulando matrice 3d statica!*/
	num_knotsu=1+gradou+numrighecp;
	num_knotsv=1+gradov+numcolcp;
	
	/*alloco dinamicamente vett monodimensionale che simula una matrice 3d di punti 
	del reticolo di controllo*/
	reticolo_cp_cartesiano=(GLfloat *) new GLfloat[3*numrighecp*numcolcp];
	/*e lo riempio col contenuto del vettore in input (che ha la stessa struttura per scelta 		progettuale*/
	for(int i=0;i<3*numrighecp*numcolcp;i++)
		reticolo_cp_cartesiano[i]=reticolocpcartesiano[i];
	/*alloco vettore dei knots (in u ed in v)*/
	knotsu=(GLfloat *) new GLfloat[num_knotsu];	
	knotsv=(GLfloat *) new GLfloat[num_knotsv];
	/*ci copio i contenuti*/
	for(int i=0;i<num_knotsu;i++)
		knotsu[i]=knu[i];
	for(int i=0;i<num_knotsv;i++)
		knotsv[i]=knu[i];

	/*quindi posso creare la nurbs*/
	pnurbs=gluNewNurbsRenderer();
}



void NurbsNonOmogenea::mi_disegno(){
	glEnable(GL_AUTO_NORMAL);
	gluBeginSurface(pnurbs);
		gluNurbsSurface(pnurbs,num_knotsu,knotsu,num_knotsv,knotsv,num_colonne_reticolo_cp*3,3,reticolo_cp_cartesiano,gradonurbsu+1,gradonurbsv+1,GL_MAP2_VERTEX_3);

	gluEndSurface(pnurbs);
	glDisable(GL_AUTO_NORMAL); /*altrimenti interferisce con le normali impostate manualmente nelle 	mesh*/
	
	
}
