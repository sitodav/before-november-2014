obj obj1;
boolean flag=false;
int i=0;
float delta=0;
void setup(){
  size(700,700);
  noFill();
  smooth();
  background(0);  
  obj1=new obj(new int[]{50,100,150},new int[]{50,100,50});
  
}




void draw(){
  background(0);
  stroke(++i%255,(int)(i/2),(int)(i/3));
  pushMatrix();
  translate(mouseX,mouseY);
  delta+=1.0;
  rotate(delta);
  if(flag==true)
    obj1.paintMe();
  popMatrix();
  
}

void keyPressed(){
 if(key=='f')
    if(flag==false)
       flag=true;
    else flag=false; 
  
}

class obj{
 int[] x;
 int[] y;
 obj(int[] x,int[] y){
   this.x=x;
   this.y=y;
 }
 
 void paintMe(){
   /*
   beginShape();
   curveVertex(x[0],y[0]);
   for(int i=0;i<x.length;i++){
       println(x[i]+" "+y[i]);
      curveVertex(x[i],y[i]);
   }
   curveVertex(x[x.length-1],y[x.length-1]);
   endShape();
   */
   rect(0,0,50,50);
 } 
}
