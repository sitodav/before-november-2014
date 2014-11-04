#include "funzioni.h"



bool riempi_unica(int &n_cicli_backtracking,matrice *matrix,int i,int j){
//questo algoritmo trova la prima soluzione valida, e richiude tutte le chiamate ricorsive ritornando true. Dal main controlliamo che se e' ritornato true, dobbiamo stampare la matrice riempita validamente. Quindi la stampa e' fatta dal main, a catena ricorsiva richiusa.


for(int k=1;k<=matrix->ret_size();k++){
	bool clue=false;
	if(matrix->getter(i,j)!=0)
		clue=true;
	//innanzitutto se il valore alla casella attuale i-jesima, e' gia' 		impostato (diverso da 0) allora quello e' un clue inserito dall'utente, poiche' le 		caselle clue non vengono mai riazzerate. Quindi non lo si puo' modificare. 		Quindi gestiamo la cosa ,facendo in modo che la chiamata per quella 		
	//casella venga comunque fatta, perche' occorre andare comunque alle prossime, 		//tuttavia tutte le operazioni di modifica alla casella vengono saltate, quindi 	quella casella dovra' avere sempre lo stesso valore impostato di partenza
	
	//Se sono un clue, non devo provare ad inserire un valore nella mia casella, quindi 
	//non devo esaurire i valori possibili, e uscire dal for ritornando false,ma devo 
	//proseguire come se avessi gia' impostato validamente il mio valore. Quindi
	if(!clue && matrix->is_invalid(k,i,j)) //controllo da fare solo
		continue;		 	//se non sono clue
					
	if(!clue) //se non e' un clue lo posso modificare col valido trovato
		matrix->setter(k,i,j);
	//altrimenti prosegue avanti normalmente lasciando il valore del clue
	if(i==matrix->ret_size()-1 && j==matrix->ret_size()-1){ //abbiamo completato tutta =SOLUZIONE
		return true;		
	}	
	
	bool next_branch;

	if(j==matrix->ret_size()-1)
		next_branch=riempi_unica(n_cicli_backtracking,matrix,i+1,0);
	else next_branch=riempi_unica(n_cicli_backtracking,matrix,i,j+1);

	if(!next_branch){//se le ramificazioni mie figlie ritornano falso
		if(!clue){ //se non sono un clue, posso cambiare valore e 
						//riprovare stesso in questa chiamata
			matrix->setter(0,i,j); 
			continue;//e riprovo con un nuovo numero
		}
		 //altrimenti se sono clue, tutto quello avanti a me e' stato gia' provato, 			e infatti il ramo ricorsivo da me iniziato ritorna false, ma siccome io non 			posso cambiare,e' inutile proseguire col for, quindi deve cambiare il mio 			antenato, quindi ritorno false
		else	{
			n_cicli_backtracking++;
			return false;
		}
	}
	return true;
			
}
n_cicli_backtracking++;
return false;

}




/******************************************************************************************************/


/******************************************************************************************************/


/******************************************************************************************************/



void riempi_plurime(int &n_soluz,int &n_cicli_backtracking,matrice *matrix,int i,int j){
bool clue=false;
//questo algoritmo invece, quando riesce a riempire la matrice (soluzione valida) stampa la soluzione al momento (stampa eseguita dalla fine del ramo ricorsivo), ma non ritorna, bensi' tornando indietro ai bivi successivi, prova a proseguire cercando successive soluzioni. Quindi non ritornando in caso di soluzione trovata, ogni chiamata ricorsiva ritorna solo quando ha esaurito tutte i possibili valori validi da tentare e vuol dire che quindi qualcosa negli antenati deve essere modificato. Tutta la ramificazione ricorsiva terminera' quando (trovate tutte le soluzioni possibili) non ci saranno piu' possibili combinazioni da provare in nessuna delle istanze ricorsive.
//Ritornando solo quando sono esaurite tutte le possibili eventualita' in avanti, non abbiamo bisogno che la funzione ritorni un valore, in quanto non ci interessa controllarlo.
//Tutto questo, adottando la stessa politica della versione per singola soluzione, nei confronti dei clues

for(int k=1;k<=matrix->ret_size();k++){
	if(matrix->getter(i,j)!=0)
		clue=true;
	if(!clue && matrix->is_invalid(k,i,j)) //non va bene valore attuale k, proviamo 
		continue;
 		//incrementando k
	if(!clue)	
		matrix->setter(k,i,j); //va bene, lo piazziamo
	if(i==matrix->ret_size()-1 && j==matrix->ret_size()-1){ //abbiamo completato tutta la matrice=STAMPA 
		
		n_soluz++;
		matrix->stampa_matrice();
		cout<<"----  ----   ----"<<endl;
		if(!clue)
			matrix->setter(0,i,j); 
		else { 
			n_cicli_backtracking++;	
			return;	
		}
		//dopo aver trovato la soluzione, se la casella attuale e' un 		
		//clue, allora non potremo cambiare valore, quindi e' inutile proseguire col 
		//for, quindi ritorniamo e l'antenato cambiera' qualcosa
			
	}
		

	else{ //altrimenti non siamo arrivati ad una soluzione, ma dobbiamo andare ancora 			avanti con le chiamate ricorsive

		if(j==matrix->ret_size()-1)
			riempi_plurime(n_soluz,n_cicli_backtracking,matrix,i+1,0);
		else riempi_plurime(n_soluz,n_cicli_backtracking,matrix,i,j+1);


		//arriviamo qui quando tutta la ramificazione che parte da noi e va avanti, e' ritornata, quindi quando sono state provate tutte le combinazioni di valori per quelle ramificazioni, indipendentemente dal fatto che siano state trovate soluzioni o meno. Quindi quello che dobbiamo fare e' semplicemente cambiare il valore attuale per la nostra casella, e rilanciare il ramo ricorsivo(a patto che non siamo clue, in tal caso si ritorna direttamente senza riazzerare la casella e senza provare un nuovo valore in questa stessa chiamata). Questo processo verra' reiterato finche' non esauriamo i valori validi rispetto ai nostri antenati
		if(!clue)
			matrix->setter(0,i,j); //lo resettiamo per evitare che sia di 				intralcio al ricalcolo eventuale nel quadrato di qualche antenato, e 				riproviamo un nuovo valore ancora in questa chiamata continuando il 				for, o se i valori sono esauriti, ritornando al padre
		else { 
			n_cicli_backtracking++;
			return;
		} //altrimenti se siamo clue, e' inutile che continuiamo il for 			ma ritorniamo direttamente al padre, perche' noi non possiamo 			cambiare, e i nostri figli hanno assunto tutti i valori assumibili. Quindi 			occorre che cambi un antenato 	
	}	
}
//arrivati qui non abbiamo piu' valori che possiamo adottare per quella che e' la disposizione "lasciataci" dai nostri antenati, quindi ritorniamo, e il nostro antenato cambierà qualcosa, rilanciandoci
n_cicli_backtracking++;
}




/******************************************************************************************************/


/******************************************************************************************************/


/******************************************************************************************************/




void set_clues(matrice *matrix){
//il range valido dei valori inseribili, dipendono dal size del gioco. Per un 3-sudoku (quadrati di 3x3, 9 quadrati in tutto, griglia 9x9 ad esempio si possono inserire numeri da 1 a 9, quindi in generale da 1 a size_game). L'utente sceglie quanti metterne, e non viene effettuato nessun controllo sulla validita' della griglia fornita dall'utente come input di inizio gioco!!

int n_clues;
int clue,i,j,k=0;
cout<<"Inserisci numero \"clues\" da inserire:"<<endl;
cin>>n_clues;
while(k<n_clues){
	cout<<"-Inserisci clue seguito dall'indice riga e indice colonna:"<<endl;
	cin>>clue>>i>>j;
	if(clue>matrix->ret_size() || i<0 || i>=matrix->ret_size() || j<0 || j>=matrix->ret_size()){
		cout<<"Uno dei valori inseriti non e' valido, riprova."<<endl;		
		continue;
	}
	matrix->setter(clue,i,j);
	k++;	

}
cout<<endl;
matrix->stampa_matrice();


}






/******************************************************************************************************/


/******************************************************************************************************/


/******************************************************************************************************/






void read_clues(matrice* &matrix){
ifstream grid_input("griglia.txt");
int size;
int clue;


grid_input>>size;//leggiamo il primo char del file, che e' un numero che indica il size della matrice
matrix=new matrice(size); //che allochiamo di conseguenza
grid_input.ignore(1,'\n'); //saltiamo il \n successivo al primo carattere


for(int i=0;i<size;i++)
	for(int j=0;j<size;j++){
		grid_input>>clue;
		if(clue!=0) //per come e' ordinato il file, prima di finire una riga puo' leggere solo X o numeri 
			matrix->setter(clue,i,j); //validi, e quando arrivera' il momento di leggere il \n verra' saltato 					
		
		if(j==size-1) //allora il prossimo char da leggere va ignorato perche' e' il \n 
			grid_input.ignore(1,'\n');		
	}


matrix->stampa_matrice();
		
}



