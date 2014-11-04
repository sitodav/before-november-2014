package elab;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.hibernate.Session;

import pojos.Pass;


public class Validat extends HttpServlet {
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
		String user=req.getParameter("user");
		String pass=req.getParameter("pass");
	
		
		Session hibSess=HibernateHelper.getSession();
		hibSess.beginTransaction();
		
		Pass obtained=(Pass)hibSess.get(Pass.class, user);
		
		hibSess.getTransaction().commit();
		hibSess.close();
		
		if(obtained.getPass().equals(pass))
			req.getSession().setAttribute("user",user);
		resp.sendRedirect(req.getContextPath());
	}

}
