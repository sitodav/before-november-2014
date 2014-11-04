package polygonwar;

//questo oggetto contiene il contatore che rappresenta lo scorrere del tempo prima che perdiamo
//e contiene anche il listener che gestira' il timer che scandisce il passare di ogni secondo


import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.Timer;

public class GamingTimer{
    
    LabelStat label2;
    private int cont;
    public Timer timerSecondi;
    
    public GamingTimer(LabelStat label){
        label2=label;
        cont=25;
        timerSecondi=new Timer(1000,new ListenerTimerGame());
        
    }
   
    public void incrementa(){
        cont+=5;
        //qui dovremmo lanciare il repaint per disgnare il timer aggiornato, tuttavia qundo colpiamo navicella, comunque viene mandato il repaint anche della label2 (per modificare punteggio)
        //, quindi non c'e' bisogno di chiamarlo anche da qui
    }
    
    public int getTimer(){
        return cont;
    }
    
    public void attiva(){
        timerSecondi.start();
    }
    
    public void stop(){
        timerSecondi.stop();
    }
    
     
    public class ListenerTimerGame implements ActionListener{ //innerclass

        @Override
        public void actionPerformed(ActionEvent e) {
            //modifichiamo il contatore
            cont--;
            //e chiamiamo il repaint della label2 dove e' contenuto il valore del timer
            label2.repaint();
            
            
        }
        
        
    }
    
}