package elab;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;

import pojos.*;

import javax.servlet.http.HttpServletResponse;

import org.hibernate.Session;

public class Inserter extends HttpServlet {
	@Override
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
		resp.sendRedirect(req.getContextPath());
	}
	
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
		Session hibSess=HibernateHelper.getSession();
		hibSess.beginTransaction();
		
		String nome=req.getParameter("nomepersona");
		String id=req.getParameter("idcasa");
		
		Proprietario p=new Proprietario();
		p.setCf(nome);
		
		
		Casa c=new Casa();
		c.setId(id);
		
		p.setCasa(c);
		
		hibSess.save(p);
		hibSess.getTransaction().commit();
		hibSess.close();
		
		resp.sendRedirect(req.getContextPath()+"/protected/pagina2.jsp");
	}
	
}
