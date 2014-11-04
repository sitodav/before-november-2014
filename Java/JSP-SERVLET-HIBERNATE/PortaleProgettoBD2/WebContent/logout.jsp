<!-- questa jsp si limita a invalidare la sessione e a rimandare alla home page (quindi sostanzialmente è un logout) -->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Logging out</title>
</head>
<body>
	<%session.invalidate();
	  response.sendRedirect(request.getContextPath());
	%>
</body>
</html>