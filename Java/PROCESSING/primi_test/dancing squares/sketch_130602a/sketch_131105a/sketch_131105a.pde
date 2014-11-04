color[] array;
punto[] punti;
int i;
void setup(){
  size(300,300);
  background(0);
  array=new color[10];
  array[0]=color(120,120);
  array[1]=color(220,120,150);
  array[2]=color(20,180,340);
  array[3]=color(250,100,80);
  array[4]=color(40,30,200);
  i=0;
  punti=new punto[20];
}

color colorePiuVicino(int x,int y){
  double minDist=width*height+1;
  int minInd=-1;
  for(int f=0;f<20;f++){
   if(punti[f]==null)
      break;
   double actual=Math.sqrt(Math.pow(x-punti[f].x,2)+Math.pow(y-punti[f].y,2));
   if(x==150 && y==50)
      println(actual);
   if(actual<minDist){
       minDist=actual;
       minInd=f;
   } 
  }
  
}

void mousePressed(){
    punti[i]=new punto(mouseX,mouseY,array[i++%5]);
    
    background(0);
    loadPixels();
    for(int indP=0;indP<width*height;indP++){
        int xCord=(int)Math.floor(indP/width);
        int yCord=(int)indP-xCord*width;
        pixels[indP]=colorePiuVicino(xCord,yCord);
    }
    updatePixels();
}

void draw(){
  ellipseMode(CENTER);
  
  for(int j=0;j<punti.length;j++){
    if(punti[j]==null)
      break;
    fill(punti[j].myColor);
    ellipse(punti[j].x,punti[j].y,10,10);  
  }
  
  
  
}

class punto{
  int x,y;
  color myColor;
  punto(int x,int y,color colore){
    this.x=x; this.y=y;
    myColor=colore;
  }
 

}

