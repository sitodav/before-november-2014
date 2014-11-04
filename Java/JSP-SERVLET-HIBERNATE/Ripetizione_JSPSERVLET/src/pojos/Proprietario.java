package pojos;
import javax.persistence.*;

@Entity
@Table (name="proprietario")
public class Proprietario {
	@Id
	@Column (name="cf")
	private String cf;
	@OneToOne (cascade=CascadeType.ALL)
	@JoinColumn (name="casa", referencedColumnName="id")
	Casa casa;
	
	public String getCf() {
		return cf;
	}
	public void setCf(String cf) {
		this.cf = cf;
	}
	public Casa getCasa() {
		return casa;
	}
	public void setCasa(Casa casa) {
		this.casa = casa;
	}
	
}
