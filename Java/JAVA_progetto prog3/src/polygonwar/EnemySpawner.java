package polygonwar;



//che un nemico sia colpito o no, nel suo array viene trattato allo stesso modo. Quello che cambia e' che se' risulta colpito, non viene disegnato, e se fa collisione non succede nulla (ne' con noastra
//astronave ne' con proiettili)

import java.awt.Color;

public class EnemySpawner implements Spawner{
    
    Enemy[] arrayEnemy;
    MiaLabel IamIn;
    
    public EnemySpawner(MiaLabel amIn){
        arrayEnemy=new Enemy[50];
        IamIn=amIn;
        
    }
    
    public void update(){
        for(int i=0;i<arrayEnemy.length;i++)
            if(arrayEnemy[i]!=null){
                arrayEnemy[i].yCord+=7;
                
                
                if(arrayEnemy[i].oscillazione==0)
                    arrayEnemy[i].xCord-=5; //sx
                else if(arrayEnemy[i].oscillazione==2)
                    arrayEnemy[i].xCord+=5; //dx
                //altrimenti rimane al centro
                
            }
                
        
    }
    
    public void spawn(){
        int iLast;
        for(iLast=0;iLast<arrayEnemy.length;iLast++)
            if(arrayEnemy[iLast]==null)
               break;
        if(iLast>=arrayEnemy.length-1)
            iLast=arrayEnemy.length-2;
        
        for(int i=iLast;i>=0;i--)
            arrayEnemy[i+1]=arrayEnemy[i];
    
        arrayEnemy[0]=new Enemy();
        
    }
    
    
    
    class Enemy{
        Color colore;
        boolean colpito;
        int animazioneEsplosione;
        int xCord;
        int yCord;
        int oscillazione;
        
        Enemy(){
            animazioneEsplosione=0; //3 fasi da disegnare appena viene colpita e passa a 1 (fase 1,2,3)
            colpito=false;
            xCord=10+(int)(Math.random()*450);
            yCord=0;
            oscillazione=(int)(Math.random()*3); //per controllare movimento sx dx o ct
            
            int c=(int)(Math.random()*4);
            switch(c){
                case 0: colore=Color.MAGENTA; break;
                case 1: colore=Color.BLUE; break;
                case 2: colore=Color.GREEN; break;
                case 3: colore=Color.cyan; break;
            }
            
        }
        
        
        
    }
    
}