package pojos;
import javax.persistence.*;


@Entity
@Table (name="password")
public class Pass {
	@Id
	@Column (name="username")
	private String user;
	@Column (name="password")
	private String pass;
	
	public String getUser(){return user;}
	public String getPass(){return pass;}

}
