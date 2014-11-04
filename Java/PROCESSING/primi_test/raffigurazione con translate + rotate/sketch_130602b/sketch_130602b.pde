float i=0.0;
float alfa=-1.0;
int colore=128;
void setup(){
 size(500,500); 
 background(255);
 stroke(0);
 smooth();
 frameRate(1600);
  
  
}



void draw(){
 //background(255);
  fill(color);
 alfa+=1.0;
 translate(width/2,height/2);
 rotate(alfa+i);
 ellipse(alfa,alfa,10,10); 
 if(alfa>=200.0){
  alfa=0;
  colore=(colore+1)%255; 
  i-=1.0;
 }
 
}
