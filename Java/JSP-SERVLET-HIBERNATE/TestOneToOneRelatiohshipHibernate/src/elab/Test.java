package elab;
import org.hibernate.Session;

import pojos.*;

public class Test {

	public static void main(String[] args){
		Session hibSess=HibernateUtils.getSessionFactory().openSession();
		hibSess.beginTransaction();
		
		Info info=new Info();
		info.setCognome("Sito");
		info.setNome("davide");
		info.setId("aaabbb");
		
		Impiegato imp=new Impiegato();
		imp.setCodfisc("STIDVD88D");
		imp.setInfo(info);
		imp.setStipendio(1000000);
		
		
		
		//hibSess.save(info);
		hibSess.save(imp);
		
		
		hibSess.getTransaction().commit();
		hibSess.close();
		
		
		hibSess=HibernateUtils.getSessionFactory().openSession();
		hibSess.beginTransaction();
		
		
		//reverse perchè bidirectional
		Info info2=new Info();
		info2.setCognome("esposito");
		info2.setNome("gigi");
		info2.setId("asda");
		
		Impiegato imp2=new Impiegato();
		imp2.setCodfisc("asd812");
		imp2.setStipendio(10000);
		info2.setImp(imp2);
		
		hibSess.save(info2);
		
		
		hibSess.getTransaction().commit();
		hibSess.close();
	}
	
}
