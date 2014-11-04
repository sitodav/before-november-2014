<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>My Processing.js test03</title>
<script src=<%=request.getContextPath()+"/processingjs/processing.js"%>></script>
<script>
	function setRGB(){
		console.log("ciao");
		var sketch=Processing.getInstanceById("miosketch1");
		var r=parseInt(document.getElementById("red").value);
		var g=parseInt(document.getElementById("green").value);
		var b=parseInt(document.getElementById("blue").value);
		sketch.set(r,g,b);
	
	}
</script>

<script>
	var bond=false;

	function bindJavascript(){
		var sketch=Processing.getInstanceById("miosketch1");
		if(sketch!=null){
			sketch.bindJS(this);	
			bond=true;
		}	
		if(!bond)
			setTimeout(bindJavascript,250);
	}
	
	bindJavascript();


	function fillXY(x,y){
		document.getElementById("xfield").value=x;
		document.getElementById("yfield").value=y;
	}
</script>

</head>
<body>
	<canvas id="miosketch1" data-processing-sources=<%=request.getContextPath()+"/processingjs/sketch1.pde"%>></canvas>
	<br>
	<div id="div1">
	x/y<input type="textfield" id="xfield"><input type="textfield" id="yfield"><br>
	</div>
	<br>
	<input type="textfield" id="red"><br>
	<input type="textfield" id="green"><br>
	<input type="textfield" id="blue"><br>
	<button type="button" onClick="setRGB()">aggiorna</button>
</body>
</html>