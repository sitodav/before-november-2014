package polygonwar;



//questa classe contiene 2 array di coordinate di "punti", (solo coordinate punti, il modo in cui sono disegnate le stelle dipende dall'oggetto Label in cui sono contenute)
//ed un metodo che 
// che spawna puntini dall'alto (stelle) che si spostano man mano verso il basso, mantenendo
//la propria coordinata x (mentre la y si incrementa sempre di piu') che e' stata generata allo spawn in maniera randomica (quando il puntino
//compare sopra a tutto)
//ogni volta che viene lanciato il repaint sulla label che contiene campo di gioco, vengono anche ridisegnate le posizioni aggiornate di questi puntini (che fanno 
//da sfondo in movimento per il nostro campo di gioco)
//questo metodo viene chiamato dal listener dell'evento timer che lo attiva

public class StarSpawn implements Spawner{
    MiaLabel labelContenitore; 
    public int[] xCordStelle;
    public int[] yCordStelle;
    
    StarSpawn(MiaLabel youAreIn){ //ci teniamo puntatore alla label in cui l'oggetto istanziato di questa classe e' istanziato (per avere accesso ad informazioni quali grandezza label etc)
        labelContenitore=youAreIn;
        xCordStelle=new int[30]; //30 stelle max in verticale visualizzabili nel campo di gioco
        yCordStelle=new int[30];
        
    }
    
    
    public void update(){
        int iUltimo=0;
        
        while(iUltimo<xCordStelle.length-1 && xCordStelle[iUltimo+1]!=0)
            iUltimo++;
   
        for(int i=iUltimo;i>=0;i--){
            
            if(i==xCordStelle.length-1) 
                    continue; //l'ultimo elemento viene sempre perso, nel caso in cui si sia riempito l'array fino alla fine
            
            xCordStelle[i+1]=xCordStelle[i]; //la coord x di quell'elemento, si sposta all'indice successivo
            yCordStelle[i]+=20; //si incrementa la y (visto che scende verso il basso rispetto al giro precedente)
            yCordStelle[i+1]=yCordStelle[i]; //e si sposta anche la coord y verso l'elemento indice successivo
            
        }
        
        //in questo modo ci troviamo sicuramente l'elemento in indice 0 dei 2 array vuoto (nel caso peggiore avremo perso l'ultimo elemento dell'array, ma tanto non ci interessa piu' e quindi
        //lo facciamo sparire tranquillamente)
    
    }
    
    
    public void spawn(){
        
        xCordStelle[0]=1+(int)(Math.random()*labelContenitore.getWidth());  //la x della nuova stella che appare in alto, sara' randomica
        yCordStelle[0]=5; 
        
    }
    
    
    
 
    
}