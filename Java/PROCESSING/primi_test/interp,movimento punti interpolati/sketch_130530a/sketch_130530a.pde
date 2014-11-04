ArrayList listaPunti;
boolean paintMode=true;
boolean gravityMode=false;
boolean randomGravity=false;
boolean fillMode=false;
PImage sfondo;

void setup(){
 println("p:PAINT MODE  m:MOVE MODE  g:GRAVITY MODE  r:RANDOM GRAVITY  c:CLEAR SCREEN  f:FILLMODE");
 sfondo=loadImage("profilo.jpg");
 background(255);
 stroke(0);
 smooth();
 size(500,300); 
 listaPunti=new ArrayList();
  
}

void draw(){
  background(0);
  image(sfondo,0,0,width,height);
  noStroke();
  fill(255,0,0);
  ellipseMode(CENTER);
  for(int i=0;i<listaPunti.size();i++)
      ellipse(((Point2D)listaPunti.get(i)).getX(),((Point2D)listaPunti.get(i)).getY(),6,6);
  if(fillMode==false)
      noFill();
  stroke(255);
  beginShape(); 
  for(int i=0;i<listaPunti.size();i++)
       curveVertex(((Point2D)listaPunti.get(i)).getX(),((Point2D)listaPunti.get(i)).getY());
  endShape(); 
  if(gravityMode==true)
    gravita();
}


void mousePressed(){
   if(paintMode==true){
          listaPunti.add(new Point2D(mouseX,mouseY));
          if(listaPunti.size()==1 || listaPunti.size()==3)
            listaPunti.add((Point2D)listaPunti.get(listaPunti.size()-1));
          else{
            listaPunti.set(listaPunti.size()-1,new Point2D(mouseX,mouseY));
            listaPunti.add((Point2D)listaPunti.get(listaPunti.size()-1));
          }
    }
}

void mouseDragged(){
  if(paintMode==false)
     aggiornaSpostamenti(mouseX,mouseY); 
}

void keyPressed(){
   if(key=='p'){
     paintMode=true;
     println("PAINTMODE ON");
   }
   else if(key=='m'){
     paintMode=false;
     println("MOVEMODE ON");
   }
   else if(key=='c')
     listaPunti.clear();
   else if(key=='g'){
     if(gravityMode==false){
         gravityMode=true;
         println("GRAVITYMODE ON");
     }
     else {println("GRAVITYMODE OFF"); gravityMode=false;}
   }
   else if(key=='r'){
     if(randomGravity==false){
        randomGravity=true;
        println("RANDOMGRAVITY ON");
     }
     else {randomGravity=false; println("RANDOMGRAVITY OFF");} 
   }
   
   else if(key=='f'){
      if(fillMode==false)
         fillMode=true;
      else fillMode=false; 
   }
       
}

void gravita(){
  int toAsk;
  for(int i=0;i<listaPunti.size();i++){
    toAsk=1;
    if(randomGravity==true)
       toAsk=(int)(1+Math.random()*10);
    ((Point2D)listaPunti.get(i)).askSetGravity(toAsk); //sta al punto vedere se deve andare in alto o in basso    
    
  }
}


void aggiornaSpostamenti(int x,int y){
    for(int i=0;i<listaPunti.size();i++)
       if((x>=((Point2D)listaPunti.get(i)).getX()-6 && x<=((Point2D)listaPunti.get(i)).getX()+6 && y>=((Point2D)listaPunti.get(i)).getY()-6 && y<=((Point2D)listaPunti.get(i)).getY()+6 )){
           ((Point2D)listaPunti.get(i)).setXY(x,y);
       }
          
  
}

class Point2D{
 private int x,y;
 private int movement; // 1 giu ,-1 su
 
 Point2D(int x,int y){
    movement=1;
    this.x=x;
    this.y=y;
 }
 
 void askSetGravity(int qta){
   if(y>=height-2)
     movement=-1;
   else if(y<2)
     movement=1;
   
   y+=qta*movement;
       
   
 }

 int getX(){
   return x;
 } 
 
 int getY(){
   return y; 
 }
 
 void setXY(int x,int y){
   this.x=x;
   this.y=y;
 }
}

