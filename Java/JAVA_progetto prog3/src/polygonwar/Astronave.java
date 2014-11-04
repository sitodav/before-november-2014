package polygonwar;



import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class Astronave{
    int animazioneMorte; //finche' e' su 0, vuol dire che non morti 
    
    Timer timerColore;
    Color mioColore;
    MiaLabel iAmIn;
    public int[] xCord;
    public int[] yCord;
   
    public int inclinazione; //0 e' dritta, 1 inclinata a sx e 2 a dx
    boolean propulsione; //dalla label che disegna l'oggetto astronave controlliamo il valore della propulsione (attivata o disattivata dal metodo di questa classe astronave
    //in base alla richiesta dell'oggetto Label in base al tasto premuto/rilasciato) e se risulta attiva aggiungiamo anche il disegno dei propulsori dietro l'astronave, altrimenti li
    //eliminiamo (le coordinate del disegno della propulsione sono contenute sempre in un array qui)
    public int[] xPropulsore1;
    public int[] yPropulsore1;
    public int[] xPropulsore2;
    public int[] yPropulsore2;
    
    public Astronave(MiaLabel iAmIn){
        animazioneMorte=0;
        
        this.iAmIn=iAmIn;
        mioColore=Color.GREEN;
        propulsione=false;
        inclinazione=0;
        xCord=new int[]{230,250,270};
        yCord=new int[]{270,250,270};
        
       //i propulsori li inizializzo direttamnete (in base alle coordinate astronave) quando avro' bisogno di visualizzarli perche' lancio i lmetodo sotto
        //attiva i propulsori
                
       //per il timer del colore
        timerColore=new Timer(5000,new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent e) {
                int c=(int)(Math.random()*4);
            switch(c){
                case 0: mioColore=Color.MAGENTA; break;
                case 1: mioColore=Color.BLUE; break;
                case 2: mioColore=Color.GREEN; break;
                case 3: mioColore=Color.cyan; break;
            }
            }
            
        });
        
        timerColore.start();
       
     }
    
    
    
    public void attivaPropulsori(){
        
        if(propulsione==false)
            propulsione=true;
        aggiornaPropulsori();
        
              
        
    }
    
    public void aggiornaPropulsori(){
        //e aggiorna i propulsori in base alla posizione nuova dell'astronave cosi' l'astronave quando si muove se li porta dietro
        xPropulsore1=new int[]{xCord[0],xCord[0]};
        yPropulsore1=new int[]{yCord[0],yCord[0]+15}; 
        xPropulsore2=new int[]{xCord[2],xCord[2]}; 
        yPropulsore2=new int[]{yCord[2],yCord[2]+15};
        
    }
    
    
    public void disattivaPropulsori(){
        if(propulsione=true)
            propulsione=false;
        
    }
    
    public void attrito(){ //l'oggetto di classe astronave ha anche un metodo che fa spostare poco alla volta l'astronave all'indietro 
        yCord[0]+=4;
        yCord[1]+=4;
        yCord[2]+=4;
        //e riaggiorna i propulsori (che siano attivati o no) ricalcolandoli sulla base delle coordinate dell'astronave, cambiate
        aggiornaPropulsori();
        
    }
    
    public void moveUp(){
        for(int i=0;i<yCord.length;i++)
            yCord[i]-=12;
        
        
    }
    
    public void moveDown(){
        for(int i=0;i<yCord.length;i++)
            yCord[i]+=12;
    }
    
    public void moveLeft(){
        for(int i=0;i<xCord.length;i++)
            xCord[i]-=12;
    }
    
    public void moveRight(){
        for(int i=0;i<xCord.length;i++)
            xCord[i]+=12;
    }
    
    public void moveUpDx(){
        for(int i=0;i<xCord.length;i++){
            xCord[i]+=12;
            yCord[i]-=12;
        }
    }
    
    public void moveUpSx(){
        for(int i=0;i<xCord.length;i++){
            xCord[i]-=12;
            yCord[i]-=12;
        }
    }
    
    public void moveDwDx(){
        for(int i=0;i<xCord.length;i++){
            xCord[i]+=12;
            yCord[i]+=12;
        }
    }
    
    public void moveDwSx(){
        for(int i=0;i<xCord.length;i++){
            xCord[i]-=12;
            yCord[i]+=12;
        }
    }
    
    public void inclinaLeft(){
        if(inclinazione==0){
            inclinazione=1;
            xCord[2]-=15;
            xCord[0]+=5;
        }
        
    }
    
    public void inclinaRight(){
        if(inclinazione==0){
            inclinazione=2;
            xCord[0]+=15;
            xCord[2]-=5;
        }
        
    }
    
    public void raddrizzaFromLeft(){
        if(inclinazione==1){
            inclinazione=0;
            xCord[2]+=15;
            xCord[0]-=5;
        }
    }
    
    public void raddrizzaFromRight(){
        if(inclinazione==2){
            inclinazione=0;
            xCord[0]-=15;
            xCord[2]+=5;
        }
    }
    
   
}
