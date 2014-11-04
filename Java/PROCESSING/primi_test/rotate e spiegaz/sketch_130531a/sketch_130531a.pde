//La draw, funziona come la translate:
//lo stato viene riportato a quello originario di default (sistema
//di riferimento con 0,0 nell'angolo in alto a sx finestra) ad ogni nuovo
//lancio della funzione draw, quindi le rotate devono stare all'interno della draw.
//Una rotate influenza soltanto le figure disegnate dopo la sua chiamata
//Il funzionamento con la popmatrix è lo stesso.

//Ogni rotazione avviene rispetto all'origine del sistema di riferimento. Quindi se si vuole ruotare attorno ad un altro punto che
//non sia origine del sistema di riferimento o si porta l'oggetto sull'origine del sistema di riferimento, lo si fa ruotare,
//e lo si riporta alla posizione originaria (NON USATO) oppure al contrario si porta prima l'origine del sistema di riferimento
//sul punto attorno al quale vogliamo far ruotare un corpo, e poi applichiamo la translate al corpo (e a meno che non sia l'ultima rotate
//del metodo draw() o a meno che non venga chiamata una popMatrix() dobbiamo ricordarci che se facciamo altre chiamate alla rotate, nella stessa
//draw() queste saranno cumulative.

float x=0.1;
void setup(){
  
  size(300,300);
  background(0);
  noFill();
  stroke(255);
  rectMode(CENTER);
  
}


void draw(){
x+=0.1;
background(0);  
pushMatrix();
translate(width/2,height/2);
rotate(x);
rect(0,0,50,50);
popMatrix();

}
