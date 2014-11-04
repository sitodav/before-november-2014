package polygonwar;


//non disegniamo nel pannello, ma nella label inserita nel pannello (quella che contiene lo sfondo)

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;


public class MioPannello extends JPanel{
    MiaFinestra frameContenitore;
    MiaLabel label1;
    LabelStat label2;
   
    
    MioPannello(MiaFinestra frame){
        frameContenitore=frame;
        label1=new MiaLabel(frameContenitore); //creiamo la prima label
        add(label1); //la aggiungiamo al pannello
       this.setBackground(Color.BLACK);
       
       label2=new LabelStat(label1,this); //creiamo la seconda label, passandogli il riferimento alla prima label
       //e passiamo il riferimento della seconda label alla prima
       label1.label2=label2;
       //ora le 2 label si vedono tra loro
        //e aggiungiamo anche la seconda label al pannello
       add(label2);
       //e solo ora possiamo passare la label 2 al collision controller che dovra' lavorare con le vite
       label1.control1.label2=label2;
        
        
        
        
    }
    
    @Override
    public Dimension getPreferredSize(){
        return new Dimension(400,600);
    }
    
   
    
    
   
    
}