package polygonwar;



import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.ImageObserver;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.Timer;


public class MiaLabel extends JLabel{

    public MiaFinestra frameContenitore;
    public CollisionController control1;
    public Astronave pg;
    public StarSpawn spawnerStelle;
    public EnemySpawner spawnerNemici;
    public BulletSpawner spawnerBullet;
    public LabelStat label2; //e questa contiene riferimento alla label2
    boolean printGameOverStat;
  
   
    
    
    
    public MiaLabel(MiaFinestra frame){
        
        frameContenitore=frame;
        spawnerStelle=new StarSpawn(this); //istanziamo lo spawner per le "stelle"
        spawnerNemici=new EnemySpawner(this);
        pg=new Astronave(this); 
        spawnerBullet=new BulletSpawner(pg);
        control1=new CollisionController(spawnerNemici,pg,spawnerBullet);
        printGameOverStat=false;
        
        
    }
    
    
    
    
    
    
    
     @Override
    public Dimension getPreferredSize(){ //stessa dimensione per la label e per il pannello che la contiene
        return new Dimension(400,500);
    }
     
      protected void paintComponent(Graphics g){
        super.paintComponent(g);
        //se il flag che indica che dobbiamo stampare la label con i ppunteggi etc, allora:
        if(printGameOverStat==true){
            //STAMPA I PUNTEGGI
            g.setColor(Color.RED);
            g.setFont(new Font("Monospaced",30,30));
            g.drawString("GAME OVER",this.getWidth()/2-75,300);
            
            //e attiva il timer che dopo 6 secondi chiudera' (dispose()) la finestra, e quindi il processo
            frameContenitore.timer4.start();
            
            
            
            return;
        }
        //altrimenti continua qua
        
        
     
        
        
        
        
        
        g.setColor(Color.GRAY);
        //disegniamo le "stelle" attualmente "spawnate"
        for(int i=0;i<spawnerStelle.xCordStelle.length;i++){
            if(spawnerStelle.xCordStelle[i]==0) break; //xke non ci sono piu elementi nell'array di coordinate
            g.drawOval(spawnerStelle.xCordStelle[i], spawnerStelle.yCordStelle[i], (int)(1+Math.random()*3), (int)(1+Math.random()*3)); //piccolissimo ovale che rappresenta stellina con dimensione piccola ma cmq randomica 
        }
        
        
        
        
       //i nemici
    
        g.setColor(Color.LIGHT_GRAY);
        for(int i=0;i<spawnerNemici.arrayEnemy.length;i++){
            if(spawnerNemici.arrayEnemy[i]==null) 
                break; //cioe' non ci sono piu' elementi, quindi inutile continuare 
            if(spawnerNemici.arrayEnemy[i].animazioneEsplosione>0){//controlliamo se dobbiamo disegnare esplosione...il flag esplosione viene attivato insieme a quello che indica che e' stat ocolpito
                switch(spawnerNemici.arrayEnemy[i].animazioneEsplosione){ //l'animazione esplosione e' diviso in piu' cicli di repaint xke' altrimenti sarebbe troppo veloce il repaint per visualizzarla con un solo ciclo
                    case 1: 
                            g.setColor(Color.YELLOW);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord,spawnerNemici.arrayEnemy[i].yCord,20,25);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue; //tanto non entrera' nell'if dopo lo switch perche' e' stato colpito
                    case 2:
                            g.setColor(Color.ORANGE);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord+15,spawnerNemici.arrayEnemy[i].yCord,30,35);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue;
                    case 3: 
                            g.setColor(Color.ORANGE);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord+15,spawnerNemici.arrayEnemy[i].yCord,40,55);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue;
                    case 4:
                            g.setColor(Color.YELLOW);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord+20,spawnerNemici.arrayEnemy[i].yCord+10,45,15);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue;
                    case 5:
                            g.setColor(Color.YELLOW);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord+20,spawnerNemici.arrayEnemy[i].yCord,35,50);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue;
                    case 6:
                            g.setColor(Color.YELLOW);
                            g.fillRect(spawnerNemici.arrayEnemy[i].xCord+10,spawnerNemici.arrayEnemy[i].yCord,30,45);
                            spawnerNemici.arrayEnemy[i].animazioneEsplosione++;
                            continue;
                    default:
                            continue; //se si e' terminata l'animazione esplosione (quindi il valore flag e' andato oltre 6) comunque si fa continue perche' tanto non bisogna disegnarlo nemico perche' se era
                        //stato impostato animazione esplosione, sicuramente era stato colpito


                }
            }
            if(spawnerNemici.arrayEnemy[i].colpito==false){ //solo i non colpiti sono disegnati, e qui ci arriva se ovviamente non e' stata impostata l'animazione esplosione quindi non si era stati colpiti
               g.setColor(Color.LIGHT_GRAY);
               g.fillOval(spawnerNemici.arrayEnemy[i].xCord-2, spawnerNemici.arrayEnemy[i].yCord-1, 59,10);
               //g.setColor(Color.BLACK);
               //g.drawOval(spawnerNemici.arrayEnemy[i].xCord+18,spawnerNemici.arrayEnemy[i].yCord-5,20,10);
               g.setColor(Color.BLACK);
               g.fillOval(spawnerNemici.arrayEnemy[i].xCord+18,spawnerNemici.arrayEnemy[i].yCord-3,20,12);
               g.fillRect(spawnerNemici.arrayEnemy[i].xCord,spawnerNemici.arrayEnemy[i].yCord,5,3);
               g.setColor(spawnerNemici.arrayEnemy[i].colore);
               g.fillOval(spawnerNemici.arrayEnemy[i].xCord+2, spawnerNemici.arrayEnemy[i].yCord-2, 52,8);
               
            }
        }
        
        //i proiettili
        for(int i=0;i<spawnerBullet.arrayBullet.length;i++)
            if(spawnerBullet.arrayBullet[i]==null) 
                break;
            else if(spawnerBullet.arrayBullet[i].colpito==false){ //se non e' un colpo gia' esploso, disegnalo
               g.setColor(spawnerBullet.arrayBullet[i].colore);
               g.fillOval(spawnerBullet.arrayBullet[i].x, spawnerBullet.arrayBullet[i].y, 3,6);
               
            }
        
            
        
        
        
        //set di istruzioni per disegnare l'astronave, anche se e' esplosa, perche' se e' esplosa le esplosioni
        //le mettiamo sul disegno dell'astronave stessa che comunque ci dovra' essere.
        //Se invece l'animazione dell'esplosione e' terminata, allora non disegniamo l'astronave, perche' non c'e' piu'
        //Quindi se l'astronave non ha iniziato ad esplodere, o ha iniziato ad esplodere ma non ha finito di esplodere
        //la disegniamo, altrimenti sparisce! 
        if(pg.animazioneMorte<19){ //a 19 e' quando termina l'esplosione 
                g.setColor(pg.mioColore);
                g.fillPolygon(pg.xCord,pg.yCord,pg.xCord.length);
                g.setColor(Color.BLACK);
                for(int i=0;i<pg.xCord.length-1;i++)
                    g.drawLine(pg.xCord[i],pg.yCord[i],pg.xCord[i+1],pg.yCord[i+1]);
                g.drawLine(pg.xCord[0],pg.yCord[0],pg.xCord[pg.xCord.length-1],pg.yCord[pg.yCord.length-1]);
                g.setColor(pg.mioColore);
                g.drawLine(pg.xCord[0], pg.yCord[0], pg.xCord[0], pg.yCord[0]+5);
                g.drawLine(pg.xCord[2], pg.yCord[2], pg.xCord[2], pg.yCord[2]+5);
                g.setColor(Color.BLACK);
                g.drawLine(pg.xCord[1],pg.yCord[1],pg.xCord[1],pg.yCord[0]);
                //e propulsori astronave
                if(pg.propulsione==true){ //se muovendomi sopra nella frame, ho attivato i propulsori nell'astronave, ora che disegno l'astronave nella label che la contiene, disegno i propulsori
                    //in realta' i propulsori sono indicati come 2 segmenti nell'astronave, ma io nella label li disegno ognuno con 3 segmenti spiazzati di colore diverso perche' cosi' sono piu' visibili
                    g.setColor(Color.ORANGE);
                    g.drawLine(pg.xPropulsore1[0],pg.yPropulsore1[0],pg.xPropulsore1[1],pg.yPropulsore1[1]);
                    g.drawLine(pg.xPropulsore2[0],pg.yPropulsore2[0],pg.xPropulsore2[1],pg.yPropulsore2[1]); 
                    //quindi ricalco con una linea un po' staccata di colore diverso, i 2 propulsori
                    g.setColor(Color.RED);
                    g.drawLine(pg.xPropulsore1[0]+1,pg.yPropulsore1[0]+3,pg.xPropulsore1[1]+1,pg.yPropulsore1[1]);
                    g.drawLine(pg.xPropulsore2[0]+1,pg.yPropulsore2[0]+3,pg.xPropulsore2[1]+1,pg.yPropulsore2[1]); 
                    //e una terza ricalcatura con un segmento ancora piu' piccolo alla fine
                    g.setColor(Color.YELLOW);
                    g.drawLine(pg.xPropulsore1[0],pg.yPropulsore1[0]+8,pg.xPropulsore1[1],pg.yPropulsore1[1]+4);
                    g.drawLine(pg.xPropulsore2[0],pg.yPropulsore2[0]+8,pg.xPropulsore2[1],pg.yPropulsore2[1]+4); 
                }
        }

                //
                
                //se l'astronave ha iniziato il ciclo animazione esplosione:
                if(pg.animazioneMorte>0){
                    switch(pg.animazioneMorte){ 
                    case 1: 
                            g.setColor(Color.YELLOW);
                            g.fillRect(pg.xCord[0],pg.yCord[1],20,25);
                            pg.animazioneMorte++;
                            break;
                    case 2:
                            g.setColor(Color.ORANGE);
                             g.fillRect(pg.xCord[1],pg.yCord[2],25,20);
                            pg.animazioneMorte++;
                            break;
                    case 3: 
                            g.setColor(Color.ORANGE);
                             g.fillRect(pg.xCord[1],pg.yCord[1],40,55);
                            pg.animazioneMorte++;
                            break;
                    case 4:
                            g.setColor(Color.YELLOW);
                            g.fillRect(pg.xCord[0],pg.yCord[1]+5,30,15);
                            pg.animazioneMorte++;
                            break;
                    case 5:
                            g.setColor(Color.YELLOW);
                            g.fillRect(pg.xCord[1],pg.yCord[1],20,44);
                            pg.animazioneMorte++;
                            break;
                    case 6:
                            g.setColor(Color.YELLOW);
                             g.fillRect(pg.xCord[1],pg.yCord[1]+5,33,13);
                            pg.animazioneMorte++;
                            break;
                    case 7:
                            g.setColor(Color.RED);
                             g.fillRect(pg.xCord[0]-2,pg.yCord[1],50,62);
                            pg.animazioneMorte++;
                            break;
                    case 8:
                            g.setColor(Color.ORANGE);
                            g.fillRect(pg.xCord[0]+10,pg.yCord[1],30,52);
                            pg.animazioneMorte++;
                            break;
                    case 9:
                            g.setColor(Color.YELLOW);
                             g.fillRect(pg.xCord[1],pg.yCord[1]+5,33,13);
                            pg.animazioneMorte++;
                            break;
                        
                   case 10:
                            g.setColor(Color.YELLOW);
                            g.fillRect(pg.xCord[0],pg.yCord[1]+5,30,15);
                            pg.animazioneMorte++;
                            break;
                  case 11:
                            g.setColor(Color.ORANGE);
                             g.fillRect(pg.xCord[1],pg.yCord[2],25,20);
                            pg.animazioneMorte++;
                            break;
                  case 12:
                            g.setColor(Color.YELLOW);
                             g.fillRect(pg.xCord[1],pg.yCord[1]+5,33,13);
                            pg.animazioneMorte++;
                            break;
                 case 13:
                            g.setColor(Color.RED);
                             g.fillRect(pg.xCord[0]-2,pg.yCord[1],50,62);
                            pg.animazioneMorte++;
                            break;
                 case 14:
                            g.setColor(Color.YELLOW);
                             g.fillRect(pg.xCord[1],pg.yCord[1]+5,33,13);
                            pg.animazioneMorte++;
                            break;
                    case 15:
                            g.setColor(Color.RED);
                             g.fillRect(pg.xCord[0]-2,pg.yCord[1],50,62);
                            pg.animazioneMorte++;
                            break;
                    case 16:
                            g.setColor(Color.ORANGE);
                            g.fillRect(pg.xCord[0]+10,pg.yCord[1],30,52);
                            pg.animazioneMorte++;
                            break;
                  case 17:
                            g.setColor(Color.YELLOW);
                             g.fillRect(pg.xCord[1],pg.yCord[1]+5,33,13);
                            pg.animazioneMorte++;
                            break;
                 case 18:
                            g.setColor(Color.RED);
                             g.fillRect(pg.xCord[0]-2,pg.yCord[1],50,62);
                            pg.animazioneMorte++;
                            break;
                 case 19:
                         pg.animazioneMorte++;
                         break;
                 case 20:
                        pg.animazioneMorte++;
                        break;
                        
                 case 21: 
                     
                     pg.animazioneMorte++;
                     break;
                 case 22: //QUI LANCIAMO DI NUOVO
                     //IL METODO PAINT COMPONENT, IMPOSTANDO PERO' UN FLAG CHE GLI FA STAMPARE NON GLI OGGETTI DI GIOCO
                     //MA PUNTEGGI ETC
                     printGameOverStat=true;
                     this.repaint();
                     break;
                     
                     
                     
                        
 

                }
            }
                
        
        
        
            
            
            
        
        
        
        
        
       
   }
    
    
    
    
    
    
    
    
}