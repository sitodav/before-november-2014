//COSTANTI
int XCENTER=0,YCENTER=0;
int SIZE_SQUARE=50;
int PADDING_SQUARES=20;
int SPEED_UPDOWN=20;
int TEXT_SIZE=32;



ViewMover view1;
ElementManager elemMan1;
PFont f=createFont("Arial",16,true);

void setup(){
 size(500,600);
 background(0);
 view1=new ViewMover(); 
 elemMan1=new ElementManager("abcdef",4); //la lunghezza stringa definisce n nel manager

}



void draw(){
 background(0);
 view1.check();
 elemMan1.paintUs();
}

class ElementManager{
   int n,v;
   String campione;
   ArrayList listaElementi=new ArrayList();
   
   ElementManager(String campione,int v){
      this.campione=campione;
      this.n=campione.length(); this.v=v;
      for(int i=0;i<n*v;i++){
        listaElementi.add(new Element(n,campione.charAt(0),(width-((n+1)*SIZE_SQUARE+PADDING_SQUARES))/2,i*(SIZE_SQUARE+PADDING_SQUARES)));        
      }
      
   }
   
   void paintUs(){
      for(int i=0;i<listaElementi.size();i++){
         ((Element)listaElementi.get(i)).paintMe();
      } 
   }
  
   class Element{
       ArrayList listaLettere=new ArrayList();
       int xcord,ycord;
       Element(int n,char blank,int xcord,int ycord){
          this.xcord=xcord; this.ycord=ycord;
          for(int i=0;i<n;i++){
             listaLettere.add(new Lettera(xcord+i*(SIZE_SQUARE+PADDING_SQUARES),ycord,blank));
          } 
       }
       
       void paintMe(){
         for(int i=0;i<n;i++)
           ((Lettera)listaLettere.get(i)).paintMe();
       }
       
       class Lettera{
          int x, y;
          char lettera;
          Lettera(int x,int y,char lettera){
           this.lettera=lettera; this.x=x; this.y=y;
          } 
          void setLettera(char lett) {lettera=lett;}
          void paintMe(){   fill(102,0,153,100); rect(x,y,SIZE_SQUARE,SIZE_SQUARE); textFont(f,TEXT_SIZE); fill(255,255); textMode(CORNER); text(lettera,x+(SIZE_SQUARE)/2-5,y+SIZE_SQUARE/2+2); }
          
       }
   }
    
}

class ViewMover{
 private int howmuch=0; 
 public void check(){
    if(mouseY>=height-50 && mousePressed && howmuch>-1*(height*2))
        howmuch-=SPEED_UPDOWN;
    else if(mouseY<=50 && mousePressed && howmuch<height*2)
        howmuch+=SPEED_UPDOWN;    
    translate(0,howmuch);
 }
}



