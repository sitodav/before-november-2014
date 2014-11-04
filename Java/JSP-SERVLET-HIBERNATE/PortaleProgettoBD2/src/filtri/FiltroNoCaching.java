package filtri;

import java.io.IOException;

import javax.servlet.Filter;
//questo filtro fa si che quando una pagina (qualunque pagina) è caricata, non faccia caching nel browser.
//In modo tale che ogni volta che viene richiesta una seconda volta, viene sempre riscaricata dal server, garantendo
//dati e visualizzazioni aggiornate
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;

public class FiltroNoCaching implements Filter{
	@Override
	public void doFilter(ServletRequest arg0, ServletResponse arg1,
			FilterChain arg2) throws IOException, ServletException {
			
		HttpServletResponse hsr=(HttpServletResponse)arg1;
		hsr.setHeader("Cache-Control", "no-cache, no-store, must-revalidate");
		hsr.setHeader("Pragma", "no-cache");
		hsr.setDateHeader("Expires", 0);
		arg2.doFilter(arg0, arg1);
		
	}

	@Override
	public void destroy() {		}
	
	@Override
	public void init(FilterConfig arg0) throws ServletException {	}

	
	

}
