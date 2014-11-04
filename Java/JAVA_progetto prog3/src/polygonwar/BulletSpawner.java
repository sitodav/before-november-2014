package polygonwar;

//mentre per lo spawner stelle/nemici etc, bastavano poche posizioni,
        //dopo le quali sicuramente l'oggetto essendo uscito dallo schermo, poteva essere sostituito, per i colpi
        //occorre tenere un sacco di posizioni perche' uno puo' sparare tantissimi colpi mentre si sta spostando sull'asse x,
        //quindi mentre i primi sparati arrivano alla fine della label, ne vengono spawnati tanti altri. Quindi innaanzitutto servono
        //tante posizioni nell'array ed eventualmente un controllo che faccia uscire i colpi che hanno superato la label, anche prima
        //che arrivino nell'ultima posizione dell'array, perche' a quel punto potrebbero avere delle coordinate di disegno talmente
        //fuori range label, che il programma potrebbe crashare (visto che essendo cosi' grande il numero di indici nell'array, prima che 
        //un oggetto venga traslato alla fine le sue coordinate y vengono decrementate tantissimo - vanno vers ol'alto quindi y decrementa)
    



//Il bulletspawner e' istanziato come oggetto nella Label, visto che e' da li' che bisogna accedere alle posizioni
//di tutti i proiettili, per disegnare quelli che sono stati generati
//Il bulletspawner, che e' come se fosse "un fucile" e' controllato dal key listener installato sulla finestra


import java.awt.Color;

public class BulletSpawner implements Spawner{
    //ha bisogno di sapere dove sta l'astronave, quindi lo istanziamo passandogli puntatore all'oggetto astronave
    Astronave shooter; //shooter e' l'oggetto che sparando, spawna i colpi, quindi usa BulletSpawner
    Bullet[] arrayBullet;
    
    public BulletSpawner(Astronave shooter){
        this.shooter=shooter;
        arrayBullet=new Bullet[200]; 
    
    }
    
    public void update(){
        for(int i=0;i<arrayBullet.length;i++)
            if(arrayBullet[i]!=null)
                arrayBullet[i].y-=15; //perche' i colpi si muovono verso l'alto
    
    }
    
    
    
    public void spawn(){ //anche se i proiettili si muovono verso l'alto, sono processati sull'array che ne tiene traccia
        //allo stesso modo degli altri oggetti che si muovono verso il basso. Quindi nelle posizioi dell'array con indici piu' bassi
        //ci saranno quelli appena "sparati" quindi con valori della y piu' alti
        int iLast;
        for(iLast=0;iLast<arrayBullet.length;iLast++)
            if(arrayBullet[iLast]==null)
               break;
        if(iLast>=arrayBullet.length-1)
            iLast=arrayBullet.length-2;
        
        for(int i=iLast;i>=0;i--)
            arrayBullet[i+1]=arrayBullet[i];
    
        arrayBullet[0]=new Bullet();
    }
    
    
    
    public class Bullet{ //inner class
        Color colore;
        int x;
        int y;
        boolean colpito;
        
        public Bullet(){
         //quado sparato, un proiettile assume il colore che aveva l'astronave nel momento in cui l'ha sparato
         colore=shooter.mioColore;
         x=shooter.xCord[1]; //il colpo viene spawnato sulla punta dell'astronave
         y=shooter.yCord[1];
         colpito=false;
        }
    }
}
