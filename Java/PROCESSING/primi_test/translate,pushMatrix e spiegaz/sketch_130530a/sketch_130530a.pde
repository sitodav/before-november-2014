//All'interno della stessa chiamata, la translate è cumulativa
//Ad esempio se faccio translate(10...) poi disegno rettangolo e poi
//faccio un'altra translate(10...) e un altro rettangolo, il secondo rettangolo
//sarà piu' avanti di 10 rispetto al primo, e il primo di 10
//rispetto a dove sarebbe stato disegnato nel sistema normale di rif
//Quindi se si vuole che solo alcuni oggetti siano affetti dalla 
//translate, ci si basa sull'ordine di chiamata translate/disegno oggetto
//oppure si usa la pushMatrix()/popMatrix()

int x=0; 
//non trasli DI, ma trasli A, quindi se non usi una 
//variabile che si incrementa, e ogni volta trasli li', ogni ciclo 
//in cui sta la translate ti riporta allo stesso punto.
void setup(){
 size(300,300);
 background(0);
 fill(255);
 noStroke();
 smooth(); 
 rectMode(CENTER);
 }


void draw(){
  background(0);
  x++;
  fill(255,0,0,128);
  rect(50,50,100,100); //questo rettangolo non verrà toccato
  //poichè disegnato prima delle translate
  pushMatrix(); //salvo lo stato della matrice di trasformazione (che attualmente non presenta trasformazioni)
  //in pratica ho creato una copia dello stato della matrice di trasformazione, e le nuove operazioni andranno ad influire sulla copia
  //senza toccare l'originale (in questo senso salvato)
  translate(x,0); //una translate influenza solo le forme disegnate dopo la sua chiamata, e neanche quelle se prima di lei era stato chiamato
  //pushmatrix e dopo di lei popMatrix()
  rect(width/2,height/2,50,50);
  translate(x,0);
  rect(width/2,height/2,50,50);
  popMatrix(); //elimino la copia della matrice di trasformazione, sulla quale stavo lavorando, in questo modo ritorno alla vecchia
  //(successiva sullo stack) matrice di trasformazione, che non ha nessuna trasformazione su di essa poichè le translate l'avevamo fatte sulla copia
  //questo quindi corrisponde al "caricamento del vecchio stato"
  rect(50,160,100,100); //questo rettangolo quindi viene disegnato come se non si fosse fatta mai nessuna translate
  
}
