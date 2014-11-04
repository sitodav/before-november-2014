Button fasterBut;
Button slowerBut;
float alfa=0.00001;
float coef=0.00001;
void setup(){
 size(300,300);
 stroke(255);
 //noFill();
 smooth(); 
 rectMode(CENTER);
 fasterBut=new Button(width-50,0,50,50);
 slowerBut=new Button(0,0,50,50);
}

void draw(){
  background(0);
  fasterBut.paintMe();
  slowerBut.paintMe();
  translate(width/2,height/2);
  for(int i=1;i<100;i++){
    stroke(255);
    fill(i*3,i%255,i*2);
    rotate(alfa);
    alfa+=coef;
    rect(0,0,200-i,200-i);
  }  
  
}

void mousePressed(){
  coef=0.00001;
  if(fasterBut.mouseIn()==true)
    coef*=10;
  else if(slowerBut.mouseIn()==true)
    coef/=10;
}

class Button{
 int x,y,xsize,ysize;
  Button(int x,int y,int xsize,int ysize){
    this.x=x; this.y=y; this.xsize=xsize; this.ysize=ysize;
 } 
 boolean mouseIn(){
    if(mouseX>=x-2 && mouseX<=x+2+xsize && mouseY>=y-2 && mouseY<=y+2+ysize)
       return true;
    else return false; 
 }
 
 void paintMe(){
   stroke(255);
   ellipseMode(CORNER);
   fill(0,255,0,100);
   ellipse(x,y,xsize,ysize);
   
  
 } 
}
