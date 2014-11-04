<!-- questa pagina permette all'utente, che è un dipendente, di visualizzare alcuni campi della tabella dipendenti -->
<html>
<head>
<link rel="stylesheet" type="text/css" href="<%=request.getContextPath()+"/fogliostile1.css"%>">
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Operazioni per dipendenti</title>
</head>
<body>
	
	<!-- includo il codice per connettersi alla base dati (con dipendente/dipendente che attualmente sono settati
	per il client come parametri di sessione per le key username e password) fare query e stamparne risultato -->
	 <%@include file="included/queryTabellaDipendentiEStampa.jsp" %>
<!-- includo il form per il logout -->
<%@include file="included/form_per_home_e_logout.jsp" %>
</body>
</html>