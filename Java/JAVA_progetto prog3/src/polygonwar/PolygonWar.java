package polygonwar;

//CONDIZIONI DI TERMINAZIONE: QUALCHE OGGETTO CONTROLLA CHE NON SIANO VERIFICATE LE CONDIZIONI DI USCITA (IL CONTROLLER COLLISIONI VERIFICA CHE NON VENIAMO COLPITI, E SE SIAMO COLPITI AVVISA LA LABSTAT CHE SE
//NON ABBIAMO PIU' VITA AVVISA LA LABEL...OPPURE SEMPRE LA LAB STAT CONTROLLA IL VALORE DEL TEMPO RIMANENTE E SE E 'FINITO AVVISA LA LABEL)..LA LABEL NEL CASO IN CUI SI VERIFICHI LA CONDIZIONE DEL FLAG IMPOSTATO
//STAMPA LA SCREEN DI GAMEOVER E FA PARTIRE UN TIMER DELLA FINESTRA, CHE DOPO TOT SECONDI FA IL DISPOSE DELLA FINESTRA, CHE VIENE CHIUSA, E QUINDI VIENE CHIUSO ANCHE IL PROCESSO XKE' C'E' L'EXIT ON CLOSE
//DEL PROCESSO SULLA FINESTRA

//aggiungo anche un conto alla rovescia, che si riavvia quando colpiamo un ' astronave di un colore valido, e quando arriva a 0 abbiamo perso


//CREA UNA CLASSE ASTRATTA O UN INTERFACCIA SPAWNER, DA CUI DISCENDONO/IMPLEMENTANO TUTTI GL ISPAWNER, METTENDO
//COME METODI ASTRATTI DA DEFINIRE L'AGGIORNA() E LO SPAWNA()

//lo spawer (classe) si aggancia a 2 classi: la finestra e la label. Alla finestra perche' e' da li' che partono i listener
//installati sulla finestra, che controllano i metodi per spawnare nuovi oggetti/aggiornarli
//mentre sulla label perche' e' da li' che si accede alle coordinate (mantenute negli spawner) degli oggetti spawnati, per disegnarli








public class PolygonWar{


    public static void main(String[] args){
        MiaFinestra fin1=new MiaFinestra();

    }


}
