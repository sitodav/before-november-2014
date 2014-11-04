<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<link rel="stylesheet" type="text/css" href="processingjs/style1.css">
	<script src="processingjs/processing.js"></script>
	<script>
		function addLanciatore(){
			var sketch=Processing.getInstanceById("miosketch1");
			sketch.addLanciatore();			
	
		}

		function addBarra(){
			var sketch=Processing.getInstanceById("miosketch1");
			sketch.addBarra();
		}
	</script>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<canvas id="miosketch1" data-processing-sources="processingjs/sketch1.pde"></canvas><br>
	<button type="button" onclick="addLanciatore()">add-lanciatore</button><br>
	<button type="button" onClick="addBarra()">add-barra</button>
</body>
</html>