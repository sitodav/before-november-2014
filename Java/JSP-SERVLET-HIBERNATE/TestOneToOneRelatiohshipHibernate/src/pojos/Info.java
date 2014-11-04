package pojos;
import javax.persistence.*;

@Entity
@Table (name="Infos")
public class Info {
	@Id
	@Column (name="info_id")
	private String id;
	
	@Column (name="name")
	private String nome;
	
	@Column (name="surname")
	private String cognome;
	
	@OneToOne (mappedBy="info", cascade=CascadeType.ALL)
	private Impiegato imp;

	public Impiegato getImp() {
		return imp;
	}

	public void setImp(Impiegato imp) {
		this.imp = imp;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getNome() {
		return nome;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public String getCognome() {
		return cognome;
	}

	public void setCognome(String cognome) {
		this.cognome = cognome;
	}
	
	
}
