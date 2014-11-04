
<html>
<head>
<link rel="stylesheet" type="text/css" href="<%=request.getContextPath()+"/fogliostile1.css" %>">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Operazioni per Human Resource</title>
</head>
<body>
	<!-- controllo che arrivando qui non ci sia un messaggio di servizio -->
	<%	String msg;
		if((msg=(String)session.getAttribute("messaggio_servizio2"))!=null) {%>
	  		<err_text><%=msg %></err_text>
	  		<br><br>
	  		<!--e poi elimino l'attributo per evitare che ricaricando la pagina venga ristampato-->
	  		<%session.removeAttribute("messaggio_servizio2");%>	
	  <%} 
	%>	

	<br>
	<h1>Aggiorna salario dipendente:</h1>
	 <br><br>
	 <!-- un human resource puo' aggiornare salario di un dipendente -->
	 <form action="aggiornaSalarioDipendente.jsp" method="post">
	 	<h3>Codice Fiscale</h2><input type="text" name="cf"><br>
	 	<h3>Nuovo Salario</h2><input type="text" name="nuovo_salario"><br><br>
	 	<input type="submit" value="aggiorna"><br>
	 </form>
	 <br><br><br>
	 ---------------------------------------------------------------------------------
	 <br><br>
	  
	<!-- includo codice per fare query alla base di dati sulla tabella dipendenti (dopo essersi connessi usando
	 username e password per utente bd (in questo caso essendo ruolo human resource ha accesso "diretto" alla base dati
	 come user.  -->
	<%@include file="included/queryTabellaDipendentiEStampa.jsp" %>
	<!-- e includo il form per il logout -->
	<%@include file="included/form_per_home_e_logout.jsp" %>
</body>
</html>