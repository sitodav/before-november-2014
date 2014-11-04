<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>

<%if(session.getAttribute("user")==null) {%>
	<form action="validatore" method="post">
		<input type="text" name="user"><br>
		<input type="text" name="pass"><br>
		<input type="submit" value="invia">
	</form>
<%}

else {%>
	SEI LOGGATO<br>
	<a href="protected/pagina2.jsp">pagina2</a><br>
	<%@include file="protected/included/logcode.jsp"%>
<%} %>

</body>
</html>