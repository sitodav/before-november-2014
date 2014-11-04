	<!-- qui c'e' il codice per fare in una jsp la query alla tabella dipendenti e stamparne il risultato
	     Si noti che l'username e password con il quale ci si connette alla base dati, sono presi come attributi di sessione
	     che per essere arrivato qui, il client deve necessariamente avere (e se è un dipendente ha dipendente/dipendente altrimenti
	     ha i propri di accesso alla bd).
	     Questo codice è stato messo qui, da includere in altre jsp, perchè è usato da jsp diverse.
	      -->
	<%@page import="java.sql.Connection, java.util.ArrayList,java.sql.DriverManager, java.sql.Statement, java.sql.ResultSet" %>
	<!-- qui dichiaro gli import delle classi che servono alla jsp -->
	
	<%
	  //creo un arraylist di stringhe, dove la prima stringa è l'header che indica i campi della tabella,
	  //e tutte le altre stringhe sono una concatenazione dei campi delle tuple ottenute dalla query
	  ArrayList<String> lista=new ArrayList<String>();
	  Class.forName("oracle.jdbc.OracleDriver"); //carico il driver per connettermi alla base dati
	  
	  Connection conn1= DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:XE",(String)session.getAttribute("username"),(String)session.getAttribute("password"));
	  //connessione effettuata, creo lo statement 
	  Statement st=conn1.createStatement();
	  
	  //se sono un semplice dipendente, mi viene nascosto il salario degli altri dipendenti e altri campi. Altrimenti li vedo
	  String query;
	  if( session.getAttribute("ruolo").equals("dipendente") )
		  query="select nome,cognome,citta,cfsupervisore,idmagazzino,idmacchinario from dagull.dipendente";
	  else query="select * from dagull.dipendente";
	  //eseguo la query	  
	  ResultSet rs=st.executeQuery(query);
	  //in rs c'e' il risultato query, ed i suoi metadata contengono il totale dei campi di cui sono composte le tuple risultanti
	  //ottengo questo totale
	  int totCol=rs.getMetaData().getColumnCount();
	  String t=new String();
	  //concateno i nomi dei campi sempre interrogando il metadata e costruisco quindi header tabella
	  for(int i=1;i<=totCol;i++){
	  		t+=rs.getMetaData().getColumnName(i);
	  		t+=" || ";
	  }
	  lista.add(t); //l'header è in indice 0 nell'array list
	  while(rs.next()){ //e a questo punto per ogni tupla ritornata (nel resultset alla posizione corrente) concateno i vari campi in una nuova stringa
		  t=new String();
		  for(int i=1;i<=totCol;i++){
			  t+=rs.getString(i);
			  t+= " || ";
		  }
		  lista.add(t);  //che alla fine aggiungo all'array list
	  }%>
	  
	 <!-- stampo header -->
	 <br><br><h1>Tabella dei dipendenti</h1><br><br>
	  --------------------------------------------------------------------------<br>
	 <testoheader><%=lista.get(0)%></testoheader><br>
	 --------------------------------------------------------------------------<br><br>
	 <!-- stampo le tuple risultanti dalla query -->
	  <testoquery><%  
	  for(int i=1;i<lista.size();i++){%>
		 <%=lista.get(i)%><br>----------------------------------------------------------------------------------------------------------------------------------<br>
	  <% }%></testoquery>
	  <%conn1.close(); %>
	  
	  