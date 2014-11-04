<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>

<br>
<br>
	<form action="<%=request.getContextPath()%>/inseritore" method="post">
		nome
		<input type="text" name="nomepersona"><br>
		casa
		<input type="text" name="idcasa"><br>
		<input type="submit" value="inserisci">
	</form>
<br>
<br>


<%@include file="included/logcode.jsp"%>
</body>
</html>