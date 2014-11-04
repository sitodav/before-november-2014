package polygonwar;




import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;


public class MiaFinestra extends JFrame{
    MioPannello pane1;

    GamingTimer timerGame1;
    Timer timer1; //oggetto timer per spawn e aggiornamento stelle, per aggiornamento nemici,proiettili e per attrito astronave
    Timer timer2; //oggetto timer per spawn nuovi nemici


    //i proiettili vengon osparati quando premendo un tasto, parte il keylistener che lancia il metodo spawn bullet dello spawner

    Timer timer4; //quando la label ha mostrato la screen di fine gioco, con la scritta gameover, attiva questo timer, il quale listener, passati tot secondi, fa il dispose() che chiude la finestra
    //ed il programma


    public MiaFinestra(){
        super("PLAY!");
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400,600);
        setResizable(false);
        setLayout(new BorderLayout());


        pane1=new MioPannello(this);
        add(pane1,BorderLayout.NORTH);



        //alloco timer e gli passo (registrazione alla creazione) l'actionlistner per il timer event(ListenerTimerStelle)
        //e aggiungo esplicitamente anche il listener per simulare l'attrito dell'astronave che si sposta poco alla volta dietro
        timer1=new Timer(100,new ListenerTimerStelle());
        timer1.addActionListener(new ListenerPerAttrito());
        timer1.start(); //e lo faccio partire

        //creo e registro il timer per il controllo spawn NEMICI (che pero' sara' avviato e gestito all'interno del listener per il timer 2)
        timer2=new Timer(600,new ListenerTimerSpawnEnemy());
        timer2.start();

        //e registro il listener che controlla l'aggiornamento posizione nemici, sullo stesso timer che governa spawn e movimento stelle e attrito
        timer1.addActionListener(new ListenerTimerAggiornaEnemy());
        //e sempre su di quest'ultimo ci registro anche il listener che aggiorna posizione proiettili (NB NON SPAWNA ANCHE, per sparare c'e' il key list)
        timer1.addActionListener(new ListenerTimerAggiornaBullet());

        timerGame1=new GamingTimer(pane1.label2); //<--A DIFFERENZA DEGLI ALTRI, QUESTO TIMER L'HO IMPLEMENTATO IN MANIERA COMPLETAMENTE INCAPSULATA E TRASPARENTE: LA FINESTRA NON HA BISOGNO DI CONOSCERE COME VIENE GESTITO
        //L'HANDLER
        //nell'oggetto gamingTimer viene gia' istanziato tutto, sia il contatore che il timer che scandisce il passare di ogni secondo.
        //Noi dobbiamo soltanto limitarci, dopo aver istanziato l'oggetto timerGame di classe GamingTimer, a chiamarne il metodo attiva, che attivera' il timer, che lancera' ogni seconod
        //un segnale, e stesso l'oggetto di classe GamingTimer si occupera' di grabbare il segnale ogni secondo decrementando il contatore
        //e lo attivo
        timerGame1.attiva();

        timer4=new Timer(6000,new ListenerTimerCloseWindow()); //questo timer non lo attiviamo, ma lo attiva la label quando ha mostrato la screen di gameover







        //registro listener per key
        this.addKeyListener(new listnerUp());

    }






    public class ListenerTimerCloseWindow implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent e) {
            dispose(); //dispose della frame, che viene chiusa, ed il processo termina
        }

    }

    //implemento listener Stelle sul timer1
    public class ListenerTimerStelle implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent ae) {
            pane1.label1.spawnerStelle.update();
            pane1.label1.spawnerStelle.spawn();
            pane1.label1.control1.enemyVsBullet();
            //pane1.label1.repaint();
        }

    }





    public class ListenerTimerSpawnEnemy implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent ae) {
            pane1.label1.spawnerNemici.spawn();
            //pane1.label1.repaint();

        }

    }

    public class ListenerTimerAggiornaBullet implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent e) {
            pane1.label1.spawnerBullet.update();
            pane1.label1.repaint();
        }


    }


    public class ListenerTimerAggiornaEnemy implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent ae) {
            pane1.label1.control1.enemyVsBullet();
            pane1.label1.spawnerNemici.update();

            pane1.label1.control1.enemyVsPg();

            pane1.label1.repaint();



        }

    }



    public class ListenerPerAttrito implements ActionListener{ //usa anche lui lo stesso timer delle stelle


        public void actionPerformed(ActionEvent ae) {
            //per un fatto di scelta di gioco, l'elemento attrito lo lanciamo soltanto se siamo nella parte medio alta del campo di gioco
            for(int i=0;i<pane1.label1.pg.yCord.length;i++)
                 if(pane1.label1.pg.yCord[i]>=450) //appena trova un valore della coordinata y di un punto dell'astronave inferiore sotto una soglia, ritorna senza lanciare metodo attrito
                        return;
            pane1.label1.pg.attrito();



       }


    }



    //implementiamo nella JFrame (come innerclass) la classe del listner tasti
    //il modo in cui sono grabbati i key quando premuti, riguarda implementazione miafinestra
    //che poi chiama le implementazioni dell'oggetto astronave che riguardano il modo in cui sono implementati i veri movimenti

    //nEL LIST DEI TASTI METTO ANCHE IL CONTROLLO PER LO SPARO

    public class listnerUp implements KeyListener{
        boolean up=false,down=false,left=false,right=false;
        boolean outSx=false,outDx=false,outUp=false,outDown=false;


        @Override
        public void keyTyped(KeyEvent ke) {

                //non mi serve il key typed
        }

        @Override
        public void keyPressed(KeyEvent ke) {


            char c=ke.getKeyChar();

            if(c=='l'){

                pane1.label1.spawnerBullet.spawn();
                pane1.label1.control1.enemyVsBullet();

            }

            if(c=='w')
                up=true;
            if(c=='s')
                down=true;
            if(c=='a')
                left=true;
            if(c=='d')
                right=true;
            //se muoviamo appena e' verificata una delle 4, non sarebbe possibile accoppiare i movimenti diagonali etc
            //invece ogni volta che parte il keylistener per un singolo tasto, il suo effetto si aggiunge
            //a quello di key premute precedentemente (effetto salvato sulle 4 var di classe booleane) e in
            //base a questo si lancia uno degli 8 movimenti
            if(up==true && left==true){
                pane1.label1.pg.moveUpSx();
                pane1.label1.pg.inclinaLeft(); //se si preme un tasto dx o sx l'astronave cerca di inclinarsi (lanciando metodo che modifica le coordinate spaziali dell'oggetto astronave, metodo che
                //tuttavia compie l'inclinazione solo nel caso in cui l'astronave non sia gia' inclinata. E simmetricamente, viene raddrizzata (se non e' gia' raddrizzata) quando viene rilasciato il tasto di movimento lat
            }
            else if(up==true && right==true){
                pane1.label1.pg.moveUpDx();
                pane1.label1.pg.inclinaRight();
            }
            else if(down==true && right==true){
                pane1.label1.pg.moveDwDx();
                pane1.label1.pg.inclinaRight();
            }
            else if(down==true && left==true){
                pane1.label1.pg.moveDwSx();
                pane1.label1.pg.inclinaLeft();
            }
            else if(up==true)
                pane1.label1.pg.moveUp();
            else if(down==true)
                pane1.label1.pg.moveDown();
            else if(left==true){
                pane1.label1.pg.moveLeft();
                pane1.label1.pg.inclinaLeft();
            }
            else if(right==true){
                pane1.label1.pg.moveRight();
                pane1.label1.pg.inclinaRight();
            }

            //se up e' true in generale modifichiamo il disegno della navicella per aggiungere i propulsori accesi
            //e quando lasciamo il tasto up si spengono
            //quindi qua nel listner del tasto premuto


            if(up==true) //lo scriviamo una sola volta qua per non metterlo sempre sopra, tant il flusso arriva qui
                pane1.label1.pg.attivaPropulsori();


            correggiMovimento(); //lanciamo la correzione movimento che controlla se e' necessario risettarsi rispetto ai bordi
            pane1.label1.control1.enemyVsPg();
            pane1.label1.repaint(); //e lanciamo la repaint per la label che aggiorna anche posizione astronave xke' ci e' disegnata sopra
        }


        @Override
        public void keyReleased(KeyEvent ke) {
            char c=ke.getKeyChar();

            if(c=='w'){ //ho sollevato tasto w ...etc
                pane1.label1.pg.disattivaPropulsori(); //e se smettiamo di premere il tasto su, disattiviamo i propulsori
                up=false;
            }
            if(c=='s')
                down=false;
            if(c=='a'){
                left=false;
                pane1.label1.pg.raddrizzaFromLeft();
            }
            if(c=='d'){
                right=false;
                pane1.label1.pg.raddrizzaFromRight();
            }

            pane1.label1.repaint();

        }



        public void correggiMovimento(){ //e' un metodo della finestra e non dell'astronave, perche' e' considerata come una correzione che avviene quando si preme un tasto di movimento laterale ma gia' siamo sul bordo
            //questo riguarda il campo, il layout etc, quindi e' un metodo esterno alla classe dell'astronave
            //per evitare che si possa uscire dai bordi del layout
                for(int i=0;i<pane1.label1.pg.xCord.length;i++){
                    if(pane1.label1.pg.xCord[i]<0){
                        outSx=true; break; //fa scansione di tutte le coordinate x Se trova almeno una <=0,
                                            //e' uscito a sx, ed e' inutle che controlliamo se e' uscito a dx
                    }
                    else if(pane1.label1.pg.xCord[i]>=pane1.label1.getWidth()){
                        outDx=true; break;   //e viceversa
                    }

                }

                for(int i=0;i<pane1.label1.pg.yCord.length;i++){ //come sopra (out dx e sx contemp no, cosi' come up e down, ma accopiati a croce si!)
                    if(pane1.label1.pg.yCord[i]<=0){
                        outUp=true; break;
                    }
                    else if(pane1.label1.pg.yCord[i]>=pane1.label1.getHeight()){
                        outDown=true; break;

                    }
                }

                if(outSx==true){
                    pane1.label1.pg.moveRight();
                    outSx=false;
                }
                else if(outDx==true){
                    pane1.label1.pg.moveLeft();
                    outDx=false;
                }

                if(outUp==true){
                    pane1.label1.pg.moveDown();
                    outUp=false;

                }
                else if(outDown==true){
                    pane1.label1.pg.moveUp();
                    outDown=false;
                }


        }





    }


    }





