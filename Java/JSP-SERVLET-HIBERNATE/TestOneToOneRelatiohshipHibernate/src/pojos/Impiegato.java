package pojos;
import javax.persistence.*;

@Entity
@Table (name="Employee")
public class Impiegato {
	@Id
	@Column (name="cf")
	private String codfisc;
	
	@Column (name="salary")
	private int stipendio;
	
	@OneToOne(cascade = CascadeType.ALL)
	@JoinColumn(name="id_info", referencedColumnName="info_id")
	Info info;

	public String getCodfisc() {
		return codfisc;
	}

	public void setCodfisc(String codfisc) {
		this.codfisc = codfisc;
	}

	public int getStipendio() {
		return stipendio;
	}

	public void setStipendio(int stipendio) {
		this.stipendio = stipendio;
	}

	public Info getInfo() {
		return info;
	}

	public void setInfo(Info info) {
		this.info = info;
	}
	
	
}
