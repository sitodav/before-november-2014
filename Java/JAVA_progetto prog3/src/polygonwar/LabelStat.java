package polygonwar;



import java.awt.*;
import javax.swing.ImageIcon;
import javax.swing.JLabel;

public class LabelStat extends JLabel{
    MiaLabel label1; //contiene riferimento all'altra label
    MioPannello pane1; //contiene riferimento al pannello che la contiene
    int vite;
    int punteggio;
    
    
    
    
    
    public LabelStat(MiaLabel label1,MioPannello pane1){
        this.label1=label1;
        this.pane1=pane1;
        vite=3; //viene decrementato ogni volta che veniamo colpiti, e se arriva a -1 moriamo
        punteggio=0;
    }
    
    public void scoring(int score){
        punteggio+=score; //se quando parte a 0, deve essere decrementato, per non permettere che i lpunteggio diventi negativo
        //se va <0 lo riportiam oa 0
        if(punteggio<0)
            punteggio=0;
    }
    
    public void colpito(){
        vite--;
        if(vite>=0){
            this.repaint();
        }
        
        else if(vite==-1){ //se mentre stiamo esplodendo (o siamo gia' morti) ricolpiamo un'alrta volta un nemico, non viene rilanciata l'esplosione!
                            //quindi parte solo quando la vita e' -1
            //HAI PERSO11!11!!!!!!!!
            //Quindi devo esplodere:
            //imposto attributi animazione e morte sull'astronave. Quindi essendo morta, l'astronave non verra' disegnata
            //ma verra' visualizzata l'animazione dell'esplosione
            //quindi fai partire l'animazione
            label1.pg.animazioneMorte=1;
            //INIZIAMO L'ANIMAZIONE ESPLOSIONE, E QUANDO SARA' TERMINATA, la label chiamera' la terminazione del gioco
            
            
        }
        
    }
    
    @Override
    public Dimension getPreferredSize(){
        return new Dimension(400,100);
    }
    
    protected void paintComponent(Graphics g){ 
        super.paintComponent(g);
        
        g.setColor(Color.RED);
        g.setFont(new Font("Font.BOLD",20,20));
        g.drawString(String.format("%s", punteggio),180,45);
        
        //e tempo rimanente
        g.drawString(String.format("%d",pane1.frameContenitore.timerGame1.getTimer()),350,45);
        if(pane1.frameContenitore.timerGame1.getTimer()==0){ //se il timer e' arrivato a 0, dopo averlo comunque disegnato, pone il flag sulla label che indica che andra' disegnata la schermata di fine partita
                label1.printGameOverStat=true;
                //e blocca il timer tanto non ci serve piu'
                pane1.frameContenitore.timerGame1.stop();
                
        }
        //e vite
        for(int i=0;i<vite;i++){
          g.setColor(Color.RED);
          g.fillOval((i*30)+30,30,20,20);
          g.setColor(Color.BLACK);
          g.fillOval((i*30)+35,30,10,10);
          g.fillOval((i*30)+35,41,10,10);
           
        }
        
        
        
    }
    
    
}