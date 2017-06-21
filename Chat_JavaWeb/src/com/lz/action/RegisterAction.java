package com.lz.action;

import java.math.BigInteger;
import java.security.MessageDigest;

import com.lz.service.DAOImpl.RegisterDAOImpl;
import com.opensymphony.xwork2.ActionContext;

public class RegisterAction {

	private String username;
	private String password;

	public String execute() {
		//获取hsa1秘文
		String pwd = getSHA(password);

		RegisterDAOImpl res = new RegisterDAOImpl();
		String userid = res.register(username, pwd);
		//将userid放入seesion中
		ActionContext context = ActionContext.getContext();
		context.getSession().put("userid", userid);
		context.getSession().put("username", username);
		return "success";
	}


	public void setUsername(String username) {
		this.username = username;
	}


	public void setPassword(String password) {
		this.password = password;
	}

	public  String getSHA(String str) {
		String string = null;
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-1");
			md.update(str.getBytes());
			string =  new BigInteger(1, md.digest()).toString(16);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return string;
	}

}
