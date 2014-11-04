package polygonwar;









public class CollisionController{
    EnemySpawner spawnerEnemy;
    Astronave pg;
    BulletSpawner spawnerBullet;
    LabelStat label2;

    public CollisionController(EnemySpawner enemySpawner,Astronave pg,BulletSpawner bulletSpawner){
        spawnerEnemy=enemySpawner;
        this.pg=pg;
        spawnerBullet=bulletSpawner;
        this.label2=label2;
    }



    //tieni conto che questi 2 metodi sono chiamati dopo che i nemici si sono gia' mossi, quindi controllali nelle loro posizioni attuali
    //e se c'e' collisione prendi provvedimento
    public void enemyVsBullet(){
        for(int i=0;i<spawnerEnemy.arrayEnemy.length;i++){
            if(spawnerEnemy.arrayEnemy[i]==null)
                break;
            if(spawnerEnemy.arrayEnemy[i].colpito==true)
                continue; //il nemico risulta gia' colpito quindi e' come se non esistesse
            for(int j=0;j<spawnerBullet.arrayBullet.length;j++){
                if(spawnerBullet.arrayBullet[j]==null) //non ci sono piu' colpi spawnati
                    break;
                if(spawnerBullet.arrayBullet[j].colpito==true) //allora il proiettile e' gia' esploso contro qualcun altro, quindi e' come se non esistesse
                    continue;
                if(spawnerBullet.arrayBullet[j].x>=spawnerEnemy.arrayEnemy[i].xCord-5 && spawnerBullet.arrayBullet[j].x<=spawnerEnemy.arrayEnemy[i].xCord+60
                        && spawnerBullet.arrayBullet[j].y>=spawnerEnemy.arrayEnemy[i].yCord-5 && spawnerBullet.arrayBullet[j].y<=spawnerEnemy.arrayEnemy[i].yCord+15){
                    //allora nemico colpito e anche colpo esploso (quindi colpito)
                    spawnerEnemy.arrayEnemy[i].colpito=true;
                    spawnerEnemy.arrayEnemy[i].animazioneEsplosione=1; //va animata l'esplosione
                    spawnerBullet.arrayBullet[j].colpito=true;
                    //se il colore del proiettile combacia, allora incrementa punteggio e incrementa timerGame
                    if(spawnerBullet.arrayBullet[j].colore==spawnerEnemy.arrayEnemy[i].colore){
                        label2.scoring(+100);
                        label2.pane1.frameContenitore.timerGame1.incrementa();
                    }
                    else //altrimenti decrementalo
                        label2.scoring(-50);
                    //e lanciamo il repaint della label2 per ridisegnare la stringa del punteggio aggiornata
                    label2.repaint();
                }
            }
        }

    }

    public void enemyVsPg(){
        for(int i=0;i<spawnerEnemy.arrayEnemy.length;i++){
            if(spawnerEnemy.arrayEnemy[i]==null)
                break;
            if(spawnerEnemy.arrayEnemy[i].colpito==true) //cioe' il nemico risulta gia' colpito ed e' invisibile nella label
                continue;
            if((pg.xCord[1]>=spawnerEnemy.arrayEnemy[i].xCord && pg.xCord[1]<=spawnerEnemy.arrayEnemy[i].xCord+55
                    && pg.yCord[1]>=spawnerEnemy.arrayEnemy[i].yCord && pg.yCord[1]<=spawnerEnemy.arrayEnemy[i].yCord+10)
                || (pg.xCord[2]>=spawnerEnemy.arrayEnemy[i].xCord && pg.xCord[2]<=spawnerEnemy.arrayEnemy[i].xCord+55
                    && pg.yCord[2]>=spawnerEnemy.arrayEnemy[i].yCord && pg.yCord[2]<=spawnerEnemy.arrayEnemy[i].yCord+10)
                    || (pg.xCord[0]>=spawnerEnemy.arrayEnemy[i].xCord && pg.xCord[0]<=spawnerEnemy.arrayEnemy[i].xCord+55
                        && pg.yCord[0]>=spawnerEnemy.arrayEnemy[i].yCord && pg.yCord[0]<=spawnerEnemy.arrayEnemy[i].yCord+10)){ //allora abbiamo colpito un nemico con la punta nostra astronave

                spawnerEnemy.arrayEnemy[i].animazioneEsplosione=1;
                spawnerEnemy.arrayEnemy[i].colpito=true;
                //veniamo colpiti
                label2.colpito(); //ci limitiamo a dirlo alla label 2





            }
        }



    }


}
