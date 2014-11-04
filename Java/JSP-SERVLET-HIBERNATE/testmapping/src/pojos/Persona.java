package pojos;
import java.io.Serializable;

import javax.persistence.*;


@Entity
@Table (name="persona")
public class Persona implements Serializable{
	@Id
	@Column (name="cf")
	private String codfisc;
	private String nome;
	private String cognome;
	@OneToOne (cascade=CascadeType.ALL)
	@JoinColumn (name="idauto", referencedColumnName="id")
	Auto auto;
	public String getCodfisc() {
		return codfisc;
	}
	public void setCodfisc(String codfisc) {
		this.codfisc = codfisc;
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
	public Auto getAuto() {
		return auto;
	}
	public void setAuto(Auto auto) {
		this.auto = auto;
	}
	
	

}
