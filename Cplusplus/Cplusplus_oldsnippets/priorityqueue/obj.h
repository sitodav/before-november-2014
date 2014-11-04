


template<typename T>
class obj{
		
	T dato;
	int prior;
public:
	obj(int newprior,T newdato):dato(newdato),prior(newprior){}
	obj(int newprior=0):prior(newprior){}
	void setPri(int newPrior);
	T getVal();
	int getPri();
	
};


template<typename T>
T obj<T>::getVal(){
	return dato;
}

template<typename T>
int obj<T>::getPri(){
	return prior;
}

template<typename T>
void obj<T>::setPri(int newPrior){
	if(prior>newPrior) return;
	prior=newPrior;
}
