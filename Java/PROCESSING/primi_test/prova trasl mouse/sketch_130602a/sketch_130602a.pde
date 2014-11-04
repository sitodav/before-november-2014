int x=0;
int y=0;



void setup(){
  size(500,500);
  background(0);
  stroke(255);
  rectMode(CENTER);  
  
}


void draw(){
  background(0);
  translate(mouseX-width/2,mouseY-height/2);
  rect(50,50,100,100);
  rect(200,200,30,30);
  rect(235,372,50,50);
  
}


void keyPressed(){
 switch(key){
  case 'a':
    x+=5;
    break;
  case 's':
    y-=5;
    break;
  case 'd':
    x-=5;
    break;
  case 'w':
    y+=5;
    break;
 } 
  
}




