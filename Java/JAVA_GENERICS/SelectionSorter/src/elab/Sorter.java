package elab;

import java.util.Collection;

interface Sorter<T extends Comparable>{
	public abstract Collection<T> sort();
}



