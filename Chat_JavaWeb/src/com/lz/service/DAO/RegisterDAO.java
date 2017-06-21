package com.lz.service.DAO;

public interface RegisterDAO {
	public String register(String username, String pssword);
	
	public String updateInfo(String[] info);
}
