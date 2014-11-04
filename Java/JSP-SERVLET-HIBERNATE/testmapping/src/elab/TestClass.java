package elab;

import java.util.List;

import org.hibernate.Query;
import org.hibernate.Session;

import pojos.*;

public class TestClass {
	public static void main(String[] args){
		Session hibSes=HibernateUtils.getSession();
		hibSes.beginTransaction();
		
		Query query=hibSes.createQuery("from Persona");
		List<Persona> lista=(List<Persona>)query.list();
		for( Persona el: lista)
			System.out.println(el.getNome());
		
		hibSes.getTransaction().commit();
		hibSes.close();
	}
}
