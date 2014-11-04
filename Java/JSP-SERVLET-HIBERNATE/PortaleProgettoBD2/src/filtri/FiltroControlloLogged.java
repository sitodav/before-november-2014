package filtri;
//Attenzione che questo filtro è stato mappato nel web.xml sull'url /protected/* cioè parte ogni qual volta si cerca
//di visitare un oggetto che si trova sotto la cartella protected
import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class FiltroControlloLogged implements Filter{

	@Override
	public void destroy() {
		// NIENTE
		
	}
	@Override
	public void init(FilterConfig arg0) throws ServletException {
		// NIENTE
		
	}
	@Override
	public void doFilter(ServletRequest arg0, ServletResponse arg1,
			FilterChain arg2) throws IOException, ServletException {
			
			//questo filtro viene mappato sull'url che porta a qualunque oggetto della cartella protected.
			//Tramite questo filtro si vuole evitare che un client possa, collegandosi direttamente ad esempio all'url
			//.../protected/pagina_per_engineer avere accesso a quella pagina senza che sia loggato. Siccome sarebbe scomodo
			//prevedere in ciascuna pagina, un controllo sull'oggetto di sessione che indica se il client è loggato o meno
			//utilizziamo quindi questo filtro.
			
			HttpServletRequest req=(HttpServletRequest)arg0;
			HttpServletResponse resp=(HttpServletResponse)arg1;
			if(req.getSession().getAttribute("ruolo")==null){
				//allora in tal caso l'attributo con chiave ruolo non è stato settato sulla sessione http del client, quindi non è loggato
				resp.sendRedirect(req.getContextPath()); //allora rimando il client alla home page
			}
			else
				arg2.doFilter(arg0,arg1); //altrimenti risulta loggato, quindi lo faccio proseguire all'url che stava visitando sotto /protected
		
	}

	

}
