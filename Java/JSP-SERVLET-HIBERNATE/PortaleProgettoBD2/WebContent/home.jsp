<!-- questa pagina è la pagina "home" della nostra web application
presenta 2 diverse visualizzazioni, a seconda che l'utente che la raggiunge (attraverso il browser) sia già loggato o meno.
 -->
<html>
<head>
<link rel="stylesheet" type="text/css" href="fogliostile1.css">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Home page portale Dagull</title>
</head>
<body>
	<!-- siccome è possibile che si venga rediretti a questa pagina per problemi o tentativi di log errati, in caso, è presente un attributo che è una stringa con un msg di servizio -->
	<%	String msg;
		if((msg=(String)session.getAttribute("messaggio_servizio"))!=null) {%>
	  		<err_text><%=msg %></err_text>
	  		<br><br>
	  		<!--e poi elimino l'attributo per evitare che ricaricando la pagina venga ristampato-->
	  		<%session.removeAttribute("messaggio_servizio");%>	
	  <%} 
	%>	
	
	
	<%if(session.getAttribute("ruolo")==null){%> <!-- ruolo è un oggetto di sessione (di tipo String) che:
													 se è stato impostato nella sessione (quindi non è null) indica che il client è loggato
													 inoltre contiene al suo interno la stringa che indica di che tipo di client si tratta-->
		<!-- in questo caso l'utente non ha effettuato il log, quindi gli presentiamo un form dove può scegliere i dati di login -->
		<h1>E' necessario il login.</h1><br>
		<h2>Immettere i dati di identificazione:</h2><br><br>
		<form action="validatore" method="post"> <!-- validatore è un url su cui è stata mappata una servlet -->
			<h3>Username:</h3> <input type="text" name="username"><br><br> <!-- si noti che username/password sono di utente base dati, se si è scelto uno dei ruoli human resource etc, o di dipendenti (in quel caso non sono utenti direti della bd) -->
			<h3>Password:</h3> <input type="text" name="password"><br><br> <!-- attenzione l'username di un dipendente è il suo CF -->
			<h3>Ruolo:</h3> <select name="ruolo">
				<option value="human_resource">Human resource</option>
				<option value="dipendente">Dipendente</option><br>
			</select><br><br><br>
			<input type="submit" value="log in"> <!-- quando premiamo submit, i dati inseriti vengono passati in post alla servlet mappata sull'url validatore (il mapping è fatto nel web.xml -->
		</form><br><br>
		<err_text>**L'username per i dipendenti è il CF (case insensitive)<br>
		</err_text>
		
	<%}
	  else{	
	
		//se siamo qui invece, il client risulta loggato e con un ruolo definito
		//quindi a seconda del ruolo, mando alla pagina jsp apposita.
		//si noti che le pagine jsp sono tutte nella cartella protected, in modo tale da poter applicare un filtro di protezione
		//sull'intera cartella, per evitare che da fuori, ci si possa collegare direttamente all'url di queste pagine, senza essere
		//loggati passando prima per la homepage.
		String ruolo=(String)session.getAttribute("ruolo");
		if(ruolo.equals("dipendente"))
			response.sendRedirect(request.getContextPath()+"/protected/pagina_per_dipendente.jsp");
		else if(ruolo.equals("human_resource"))
			response.sendRedirect(request.getContextPath()+"/protected/pagina_per_humanresource.jsp");
	  }%> 
	  <br><br><br>
	  <%@include file="protected/included/form_per_home_e_logout.jsp" %> <!-- per evitare ridondanza codice, faccio include di codice che si trova in un unico punto usando direttive jsp -->
		
</body>
</html>