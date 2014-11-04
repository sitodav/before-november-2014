package elab;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import org.hibernate.service.ServiceRegistry;

public class HibernateHelper {
	private static SessionFactory sf;
	static{
		Configuration conf=new Configuration();
		conf.configure();
		ServiceRegistry reg=new StandardServiceRegistryBuilder().applySettings(conf.getProperties()).build();
		sf=conf.buildSessionFactory(reg);
	}
	
	public static Session getSession(){return sf.openSession();}
}
