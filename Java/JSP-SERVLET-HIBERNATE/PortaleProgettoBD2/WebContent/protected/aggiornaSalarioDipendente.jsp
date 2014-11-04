<!-- questa jsp viene chiamata passando come parametri in post il cf del dipendente ed il suo nuovo salario da settare -->
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>aggiornamento in corso..</title>
</head>
<body>
	<%@page import="java.sql.Connection, java.util.ArrayList,java.sql.DriverManager, java.sql.PreparedStatement, java.sql.ResultSet" %>
	<%String cf=request.getParameter("cf").toLowerCase(); //rendo minuscolo, perchè nella query anche è reso minuscolo, in modo tale da rendere case insensitive il cf.
	  String nuovosal=request.getParameter("nuovo_salario");
	  float nuovosal_f=-1.0f;
	  String messaggio_servizio;
	  //ottenuto cf e nuovo salario
	  //faccio il parsing del salario perchè arriva come stringa, facendo attenzione a controllare che non sollevi eccezione (se ad esempio è stato inserito un char come nuovo salario)
	  try{
		  if(nuovosal!=null) //perchè non è detto che sia davvero arrivato come parametro di post
		  	nuovosal_f=Float.parseFloat(nuovosal);
	  }
	  catch(NumberFormatException ex){
		  messaggio_servizio="Il nuovo salario deve essere un intero o un float";
		  request.getSession().setAttribute("messaggio_servizio2",messaggio_servizio);
	  }
	 
	  
	  if(cf!=null && nuovosal_f!=-1.0f){ //cioè se il cf è arrivato correttamente, il salario anche, ed il parsing ha dato un valore numerico valido
		  //mi collego alla base dati
		  Class.forName("oracle.jdbc.OracleDriver"); //carico il driver
		  Connection conn1=DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:XE",(String)session.getAttribute("username"),(String)session.getAttribute("password"));
		  PreparedStatement pst=conn1.prepareStatement("update dagull.dipendente set salario=? where lower(cf)=?");
		  
		  pst.setFloat(1,nuovosal_f); //faccio il parsing della stringa presa come parametro post dalla jsp poichè arriva come stringa
		  pst.setString(2,cf);
		  int ntuplemodificate=pst.executeUpdate();//eseguo la query di update
		  
		  if(ntuplemodificate>0)
			  messaggio_servizio="Aggiornamento eseguito con successo";
		  else
			  messaggio_servizio="Errore. Ricontrolla i dati";
		  conn1.close();
		  //setto il messaggio di servizio nella sessione
		  request.getSession().setAttribute("messaggio_servizio2",messaggio_servizio);
		  
	  }
	  
	  response.sendRedirect(request.getContextPath()); //e in ogni caso ritorno alla home, che non riuscirà a leggere questo messaggio di servizio (perchè non lo cerca, essendo chiamato messaggio_di_servizio2)
  	  //ma la home rimanda alla pagina_per_humanresource che intercetterà il messaggio, stampando l'errore (o successo)
	%>
</body>
</html>