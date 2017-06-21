package com.lz.action;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import org.apache.struts2.ServletActionContext;

import com.lz.service.DAO.RegisterDAO;
import com.lz.service.DAOImpl.RegisterDAOImpl;
import com.opensymphony.xwork2.ActionSupport;

public class UpdateAction extends ActionSupport {

	private String username;
	private String userid;
	private int sex;
	private String birthofdate;
	private String mobile;
	private String mail;
	private String personalizedsignature;
	private File file;
	private String fileFileName ;  // 上传文件的名字 ,FileName 固定的写法  
    private String fileContentType ; 
	
	

	@Override
	public String execute() throws Exception {
		// TODO Auto-generated method stub
		
		 String imagepath = "http://127.0.0.1/default.png";
		 
		 if(file != null)
		 {
			 InputStream is = new FileInputStream(file);

			 String[] format = fileFileName.split("\\.");

			 OutputStream os = new FileOutputStream(
					 new File("/var/www/html", userid +"." + format[1]));

			 byte[] buffer = new byte[500];
			 int length = 0;

			 while(-1 != (length = is.read(buffer, 0, buffer.length)))
			 {
				 os.write(buffer);
			 }

			 os.close();
			 is.close();
			 imagepath = "http://127.0.0.1/"+ userid +"." + format[1];
		 }
  
        String[] info = new String[8];
        
        info[0] = username;
        if(sex == 1)
        	info[1] = "男";
        else
        	info[1] = "女";
        info[2] = birthofdate;
        info[3] = mobile;
        info[4] = mail;
        info[5] = personalizedsignature;
        info[6] = imagepath;
        info[7] = userid;
        
        RegisterDAO rd = new RegisterDAOImpl();
        rd.updateInfo(info);
        return SUCCESS;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public void setSex(int sex) {
		this.sex = sex;
	}

	public void setBirthofdate(String birthofdate) {
		this.birthofdate = birthofdate;
	}

	public void setMobile(String mobile) {
		this.mobile = mobile;
	}

	public void setMail(String mail) {
		this.mail = mail;
	}

	public void setPersonalizedsignature(String personalizedsignature) {
		this.personalizedsignature = personalizedsignature;
	}

	public void setFile(File file) {
		this.file = file;
	}

	public void setFileFileName(String fileFileName) {
		this.fileFileName = fileFileName;
	}

	public void setFileContentType(String fileContentType) {
		this.fileContentType = fileContentType;
	}

	public File getFile() {
		return file;
	}

	public String getFileFileName() {
		return fileFileName;
	}

	public String getFileContentType() {
		return fileContentType;
	}

	public String getUserid() {
		return userid;
	}

	public void setUserid(String userid) {
		this.userid = userid;
	}
	
	
	
}
