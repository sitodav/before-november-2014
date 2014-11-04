package elab;

import java.util.ArrayList;
import java.util.List;

public class MainClass {
	public static void main(String[] args){
		
		ArrayList<MioOggetto> al=new ArrayList<MioOggetto>();
		for(int i=0;i<20;i++){
			String t=new String();
			for(int h=0;h<=i+1;h++)
				t+=((char)(100+Math.random()*10));
			al.add(new MioOggetto(t));
		}
		
		
		stampa(al);
		
		GenericSorter<MioOggetto> sorter=new GenericSorter<MioOggetto>(al);
		
		ArrayList<MioOggetto> sorted=sorter.sort();
		
		
		
		stampa(sorted);
		
		
	}
	
	private static <T> void stampa(List<T> coll){
		System.out.println("-----------------------------------");
		for(T el: coll)
			System.out.println(el.toString());
	}
}




class MioOggetto implements Comparable{

	private String data;
	public MioOggetto(String val){ data=val;}
	
	@Override
	public String toString() {
		return data;
	}
	
	public String getValore(){return data;}
	@Override
	public int compareTo(Object arg0) {
		if(getValore().length() > ((MioOggetto)arg0).getValore().length() )
			return 1;
		return 0;
	}
	
}