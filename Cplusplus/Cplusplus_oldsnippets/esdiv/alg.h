class alg{
private:
	int *a;
	int size;
	int funz(int p,int u);	

//overloading di funz, noi dall'esterno lanciamo solo la funz senza parametri, pero' non e' quella usata davvero, poiche' per la ricorsione bisogna passare ogni volta gli indici da stringere. Quindi da funz chiamiamo la funz con intp e int u come parametri, e gli passiamo come parametri gli estremi che almeno la prima volta soddisfano che in a[1]<a[n]

public:
	alg(){
		size=10;	
	}
	alg(int n){
		size=n;	
	}
	void alloca_riempi();
	int funz();
	
	
	

};
