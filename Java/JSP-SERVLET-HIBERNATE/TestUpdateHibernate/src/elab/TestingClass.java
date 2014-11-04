package elab;

import org.hibernate.Session;
import pojos.Casa;

public class TestingClass {
	public static void main(String[] args){
		Session hibernSess=HibernateUtils.getSessionFactory().openSession();
		hibernSess.beginTransaction();
		
		Casa toSearch=new Casa();
		toSearch.setAddress("seconda trav");
		toSearch.setNumber(5);
		Casa toUpd=(Casa)hibernSess.get(Casa.class, toSearch);
		hibernSess.close();
		
		hibernSess=HibernateUtils.getSessionFactory().openSession();
		hibernSess.beginTransaction();
		toUpd.setNumber(12);
	
		
		try{
			hibernSess.update(toUpd);
			hibernSess.getTransaction().commit();
			hibernSess.flush();
		} catch(Exception e){e.printStackTrace();}
		
		finally{
			
			hibernSess.close();
		}
	}
}
