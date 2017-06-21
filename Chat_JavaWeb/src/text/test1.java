package text;

import org.junit.Test;

import com.lz.service.DAOImpl.RegisterDAOImpl;

public class test1 {

	@Test
	public void testregister()
	{
//		RegisterDAOImpl r = new RegisterDAOImpl();
		
//		r.register("woshishui", "123456");
	
		String string  = "123456.jpg";
		
		String[] s = string.split("\\.");
		
		System.out.println(s[0] + "s");
	}
}
