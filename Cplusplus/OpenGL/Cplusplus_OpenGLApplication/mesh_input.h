#include <iostream>
#include <cstring>
#include "mesh.h"
#include "textureLib.h" //usata per leggere un'immagine in formato .sgi

using namespace std;

/*ATTENZIONE: il formato wavefront .obj esportato da sketchup è diverso da quello classico wavefront .obj*/


mesh *generamesh_dafile_senzanormali_senzatexture_mioformato(string path,float scale); //le normali sono calcolate nella classe mesh
mesh *generamesh_dafile_wavefrontobj_senzanormali_senzatexture(string path,float scale,float [],float [],float []); //le normali sono calcolate nella classe mesh, e non sono utilizzate le normale definite, nel formato wavefront obj per ciascun vertice
//le ultime 3 sono i materials


mesh_contextures * generamesh_dafile_wavefrontobj_senzanormali_contexture(string pathtexture,string pathmtl,float scale,GLuint handlers[],int *last);
/*anche qui le normali sono calcolate dalla classe mesh e non sono usate le normali definite nel formato wavefront.
Inoltre viene usata l'informazione sulle texture contenuta nel file .obj, al quale quindi è associato un file .mtl, del quale pure prendiamo in input il path. Nel file .mat è associato, al nome della texture contenuto nel file .obj, il path dell'immagine .sgi.*/




