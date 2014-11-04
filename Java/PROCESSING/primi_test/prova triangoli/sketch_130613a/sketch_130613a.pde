Tri[] triangs;
int NUM_TRIANG=20;
int SECONDS=60;
//double SINGLE_STEP=Math.PI/(SECONDS*NUM_TRIANG);
float SINGLE_STEP;

void setup(){
 size(500,500);
 stroke(255);
 triangs=new Tri[20];
 for(int i=0;i<triangs.length;i++)
   triangs[i]=new Tri(float(i)+1);
 noFill();
 

  
}

void draw(){
 background(0); 
 translate(width/2,height/2);
 SINGLE_STEP=(mouseX+mouseY)/100;
 for(int i=triangs.length-1;i>=0;i--){
    rotate((float)SINGLE_STEP);
    triangs[i].paintMe(); 
 }
 //SINGLE_STEP+=Math.PI/(SECONDS*NUM_TRIANG);
 



}

class Tri{
  private int[] xcord,ycord;
  Tri(float i){
    xcord=new int[3]; ycord=new int[3];
    xcord[0]=0; ycord[0]=(int)(-10*i);
    xcord[1]=(int)(10*i); ycord[1]=(int)(10*i);
    xcord[2]=-(int)(10*i); ycord[2]=(int)(10*i); 
  }  
  
  void paintMe(){
     line(xcord[0],ycord[0],xcord[1],ycord[1]);
     line(xcord[1],ycord[1],xcord[2],ycord[2]);
     line(xcord[2],ycord[2],xcord[0],ycord[0]);    
  }
  
}

