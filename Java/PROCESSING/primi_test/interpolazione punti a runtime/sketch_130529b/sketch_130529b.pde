ArrayList listaPunti;
PImage img;

void setup(){
  size(500,300);
  background(255);
  //noFill();
  smooth();  
  listaPunti=new ArrayList();
  img=loadImage("profilo.jpg");
  image(img,0,0,width,height);
}


void draw(){
  noStroke();
  fill(255,0,0);
  ellipseMode(CENTER);
  for(int i=0;i<listaPunti.size();i++)
      ellipse(((Point2D)listaPunti.get(i)).getX(),((Point2D)listaPunti.get(i)).getY(),4,4);
  noFill();
  stroke(255,255,0);
  
  
}

void mousePressed(){ //tutto sto bordello per aggiungere le coordinate poichè il primo e l'ultimo punto aggiunti devono essere messi 2 volte poichè fungono anche da puntid i controllo
//automaticamente (il beginShape() li prende automaticamente come punti di controllo, quindi se stessero una sola volta non verrebbero usati anche come punti normali
//di interpolazione)

  listaPunti.add(new Point2D(mouseX,mouseY));
  if(listaPunti.size()==1 || listaPunti.size()==3)
    listaPunti.add((Point2D)listaPunti.get(listaPunti.size()-1));
  else{
    listaPunti.set(listaPunti.size()-1,new Point2D(mouseX,mouseY));
    listaPunti.add((Point2D)listaPunti.get(listaPunti.size()-1));
  }
}


void keyPressed(){
  
  if(key=='i'){
   beginShape(); 
   for(int i=0;i<listaPunti.size();i++)
       curveVertex(((Point2D)listaPunti.get(i)).getX(),((Point2D)listaPunti.get(i)).getY());
   endShape();
   
 }
}

class Point2D{
 private int x,y;
 
 Point2D(int x,int y){
    this.x=x;
    this.y=y;
 }

 int getX(){
   return x;
 } 
 
 int getY(){
   return y; 
 }
}
