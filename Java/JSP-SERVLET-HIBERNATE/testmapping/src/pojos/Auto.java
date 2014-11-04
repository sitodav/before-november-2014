package pojos;
import java.io.Serializable;

import javax.persistence.*;


@Entity
@Table (name="auto")
public class Auto implements Serializable{
	@Id
	@Column (name="id")
	private int id;
	private int cilindrata;
	@OneToOne (mappedBy="auto", cascade=CascadeType.ALL)
	Persona persona;
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public int getCilindrata() {
		return cilindrata;
	}
	public void setCilindrata(int cilindrata) {
		this.cilindrata = cilindrata;
	}
	public Persona getPersona() {
		return persona;
	}
	public void setPersona(Persona persona) {
		this.persona = persona;
	}
	
}
