<html>
<head><title>Benvenuto</title></head>
<body>
<%if (session.getAttribute("username")!=null) {%>
	SEI LOGGATO<br>
	<a href="protetti/pagina2.jsp">pagina2</a>
	

<%}

else {%>
	<form action="validatore" method="post">
		<input type="text" name="username"><br>
		<input type="text" name="password"><br>
		<input type="submit" value="log in"><br>
	</form>

<%} %>
<%@include file="WEB-INF/inclusi/form_logout.jsp"%>

</body>
</html>