#include "mesh.h"


mesh::mesh(float **pti,int npunt,float scalefact){
	npunti=npunt;
	pti_univoci=(float **)new float *[npunti];
	for(int i=0;i<npunti;i++){
		pti_univoci[i]=(float *)new float[3];
		for(int j=0;j<3;j++)
			pti_univoci[i][j]=pti[i][j]*scalefact;	
	}
}



mesh::mesh(float **pti,int npunt,float scalefact,float compspec[],float compamb[],float compdiffusiva[]):mesh(pti,npunt,scalefact){

	//chiamo il costruttore overloaded che si salva i punti, npunti e fatt scala, ed in piu' mi salvo le comp 
	//di materials
	compspeculare_generalemesh[0]=compspec[0];
	compspeculare_generalemesh[1]=compspec[1];
	compspeculare_generalemesh[2]=compspec[2];



	compambientale_generalemesh[0]=compamb[0];
	compambientale_generalemesh[1]=compamb[1];
	compambientale_generalemesh[2]=compamb[2];
	

	compdiffusiva_generalemesh[0]=compdiffusiva[0];
	compdiffusiva_generalemesh[1]=compdiffusiva[1];
	compdiffusiva_generalemesh[2]=compdiffusiva[2];
}







void mesh::push_faccia(int *indici,int nvertici){
	float tnormals[]={0.0,0.0,0.0}; //lo passiamo a newell e viene riempito con i valori della normale, che ripassiamo al costruttore della faccia
	newell(indici,nvertici,tnormals);
	facce.push_back(faccia(indici,nvertici,tnormals));
}







void mesh::newell(int indici[],int nvertici,float temp[]){
	for(int i=0;i<nvertici;i++){
		int suc=(i+1)%nvertici;
		temp[0]+=(pti_univoci[indici[i]][1]-pti_univoci[indici[suc]][1])*(pti_univoci[indici[i]][2]+pti_univoci[indici[suc]][2]);
		temp[1]+=(pti_univoci[indici[i]][2]-pti_univoci[indici[suc]][2])*(pti_univoci[indici[i]][0]+pti_univoci[indici[suc]][0]);
		temp[2]+=(pti_univoci[indici[i]][0]-pti_univoci[indici[suc]][0])*(pti_univoci[indici[i]][1]+pti_univoci[indici[suc]][1]);
	}
	//normalizzo
	float m=sqrt(temp[0]*temp[0]+temp[1]*temp[1]+temp[2]*temp[2]);
	temp[0]/=m;
	temp[1]/=m;
	temp[2]/=m;

}





void mesh::mi_disegno(){
	
	
	//imposto i materials(i generali)
	glMaterialfv( GL_FRONT, GL_AMBIENT,  compambientale_generalemesh );
	glMaterialfv( GL_FRONT, GL_DIFFUSE,  compdiffusiva_generalemesh );
	glMaterialfv( GL_FRONT, GL_SPECULAR, compspeculare_generalemesh );
	
	//scorro su tutte le facce contenute nel vector
	for(vector<faccia>::iterator it=facce.begin();it!=facce.end();it++){
		//imposto prima il valore della normale
		glNormal3fv(it->normale);
		//per ogni faccia scorro sul suo array di indici, e prendo i punti nell'array di punti univoci contenuto proprio nella classe mesh
		glBegin(GL_POLYGON);
		for(int j=0;j<it->nvertici;j++){
				glVertex3fv(pti_univoci[it->indici[j]]);
		}	
		glEnd();

	}
	check_errori();

}








void mesh::check_errori(){
GLenum err;
while((err=glGetError())!=GL_NO_ERROR)
	cout<<gluErrorString(err)<<endl;

}


//------------------------------------------------------*


faccia::faccia(int *indi,int nverti,float *normal){
	indici_textures=NULL; /*siccome questo costruttore parte per le facce che non usano textures, allora si potrà controllare il tipo di faccia (con textures o senza) controllando lo stato di questo puntatore*/
	nvertici=nverti;
	indici=(int *)new int[nvertici];
	for(int i=0;i<nvertici;i++)
		indici[i]=indi[i];
	normale[0]=normal[0];
	normale[1]=normal[1];
	normale[2]=normal[2];
	
}









faccia::faccia(int *indi,int nverti,float *normal,int *inditext,string chiavetext){
	nvertici=nverti;
	indici=(int *)new int[nvertici];
	for(int i=0;i<nvertici;i++)
		indici[i]=indi[i];
	normale[0]=normal[0];
	normale[1]=normal[1];
	normale[2]=normal[2];
	
	/*per textures*/
	chiavetexture=chiavetext;

	indici_textures=(int *)new int[nvertici];
	for(int i=0;i<nvertici;i++)
		indici_textures[i]=inditext[i];
	

}







//--------------------------------------------------------------------------------_*


mesh_contextures::mesh_contextures(float **pti,int npunt,float scalefact,float **ptitext,int nptitext,map<string,Texture> maptocopy):mesh(pti,npunt,scalefact){ 
		/*nella lista di inizializzazione viene chiamato il costruttore della madre, normalmente, paassando i parametri soliti ed in piu' gestisce qui esplicitamente per le textures
nb: ptitext è l'array 2d di punti univoci di textures usati da questa mesh...*/
			
		textures=maptocopy; /*copio corrispondenza oggetti Textures con chiave*/
		npuntitextures=nptitext; /*num punti univoci di textures*/
		/*mi salvo tutti i punti univoci di textures*/
		pti_univoci_textures=(float **)new float *[npuntitextures];
		for(int i=0;i<npuntitextures;i++){
			pti_univoci_textures[i]=(float *)new float[2];
			for(int j=0;j<2;j++)
				pti_univoci_textures[i][j]=ptitext[i][j]/**scalefact*/;	//il fattore di scala non ci vuole qua poichè le coordinate relative alla texture sono sempre le stesse !!
		}
			
}






void mesh_contextures::push_faccia(int *indici,int nvertici,int *indicitexture,string chiavetexture){
			
			float tnormals[]={0.0,0.0,0.0}; //lo passiamo a newell e viene riempito con i valori della normale, che ripassiamo al costruttore della faccia
			newell(indici,nvertici,tnormals);
			facce.push_back(faccia(indici,nvertici,tnormals,indicitexture,chiavetexture));	/*pusho su facce una faccia, pero' facendo partire il costruttore di faccia che considera anche indicitextures e chiave*/	
			
	
}



void mesh_contextures::mi_disegno(){
bool texturingwithimage;

	//scorro su tutte le facce contenute nel vector
	for(vector<faccia>::iterator it=facce.begin();it!=facce.end();it++){
		
		/*PER OGNI FACCIA */
		//PER LA LUCE
		//recupero e attivo la normale
		glNormal3fv(it->normale);
		

		/*PER LE TEXTURE, DEVO RECUPERARE INNANZITUTTO OGGETTO TEXTURE ASSOCIATO CON TEXTURE CHE HA 
		QUEL NOME*/
		Texture text=textures[it->chiavetexture];
	
		/*Sicuramente a questa texture, sono associate le componenti ambientale, diffusiva e speculare, 
		che attivo*/
		
		glMaterialfv( GL_FRONT, GL_AMBIENT,  text.compambientale );
		glMaterialfv( GL_FRONT, GL_DIFFUSE,  text.compdiffusiva );
		glMaterialfv( GL_FRONT, GL_SPECULAR, text.compspeculare );
		/*ora devo controllare se con quella texture, c'era associata un'immagine, e se c'era avevo
		settato l'handler diverso da -1*/
		if(text.nomehandler==-1)
			texturingwithimage=false;
		else texturingwithimage=true;
			
		if(texturingwithimage==true){
			//in tal caso
			/*attivo oggetto*/
			glEnable(GL_TEXTURE_2D);
			/*ricarico la texture che avevo bindato precedentemente sull'handler*/
			glBindTexture(GL_TEXTURE_2D,text.nomehandler);		
		}
		
		
	
		
		/*INIZIO IL DISEGNO, SIA DEI PUNTI GEOMETRICI CHE DI QUELLI TEXTURE(se texturing faccia è ON)*/
		//per ogni faccia scorro sul suo array di indici, e prendo i punti nell'array di punti univoci (sia per quelli geometrici che per quelli mesh) contenuto proprio nella classe mesh
				
		glBegin(GL_POLYGON);
		for(int j=0;j<it->nvertici;j++){
				if(texturingwithimage==true)
					glTexCoord2fv( pti_univoci_textures[it->indici_textures[j]] );
				/*in ogni caso vertice faccia disegnato*/
				glVertex3fv(pti_univoci[it->indici[j]]);
		}	
		glEnd();
		
		if(texturingwithimage==true)
			glDisable(GL_TEXTURE_2D); //disabilito per sicurezza siccome ci sono meshc che non usando
						/*textures potrebbero avere problemi se questo rimane attivo*/

	}
	check_errori();

}








