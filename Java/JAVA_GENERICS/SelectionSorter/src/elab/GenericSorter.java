package elab;

import java.util.ArrayList;

class GenericSorter <T extends Comparable> implements Sorter<T>{
	private ArrayList<T> strutt;

	
	
	public GenericSorter(ArrayList<T> toCopy){
		strutt=new ArrayList<T>();
		for(T el: toCopy)
			strutt.add(el);
	}
	
	
 	
	@Override
	public ArrayList<T> sort() {
		for(int i=0;i<strutt.size()-1;i++){
			int imin=i;
			for(int j=i+1;j<strutt.size();j++)
				if( strutt.get(j).compareTo(strutt.get(imin))==0)
					imin=j;
				
				T temp=strutt.get(i);
				strutt.set(i, strutt.get(imin));
				strutt.set(imin, temp);
			
		}
		
		return strutt;
		
	}

}
