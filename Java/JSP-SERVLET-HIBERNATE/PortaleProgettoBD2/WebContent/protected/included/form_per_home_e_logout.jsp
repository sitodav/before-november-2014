<!-- in questa jsp c'e' del codice che viene incluso con una direttiva @include da altre pagine, in modo tale da evitare ridondanza
     codice -->
<!-- questo rimanda alla pagina jsp che invalida la sessione (quindi logout.jsp che a sua volta poi ci rimanda alla home) -->
<form action="<%=request.getContextPath()+"/logout.jsp"%>" method="get">
	<input type="submit" value="logout">
</form>



