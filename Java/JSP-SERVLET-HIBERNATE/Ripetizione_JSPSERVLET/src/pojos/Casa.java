package pojos;
import javax.persistence.*;

@Entity
@Table (name="casa")
public class Casa {
	@Id
	private String id;
	@OneToOne (cascade=CascadeType.ALL, mappedBy="casa")
	Proprietario proprietario;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public Proprietario getProprietario() {
		return proprietario;
	}
	public void setProprietario(Proprietario proprietario) {
		this.proprietario = proprietario;
	}
	
	

}
