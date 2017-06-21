package com.lz.service.DAOImpl;

import java.io.File;

import org.hibernate.SQLQuery;
import org.hibernate.Session;

import com.lz.Hibernate.HibernateSessionFactory;
import com.lz.service.DAO.RegisterDAO;

public class RegisterDAOImpl implements RegisterDAO {

	@Override
	public String register(String username, String password) {

		Session session = HibernateSessionFactory.getSession();
		session.beginTransaction();
		//获取当前最大的用户id
		String sql_getmaxuserid = "select userid from user order by userid desc limit 1;";
		SQLQuery query_getmaxuserid = session.createSQLQuery(sql_getmaxuserid);
		int maxuserid = Integer.parseInt(query_getmaxuserid.list().get(0).toString());

		//插入一个用户
		Integer userid = maxuserid + 1;//当前用户id
		String sql_insert_user = "insert into user(userid, password) value(?, ?);";
		SQLQuery query_insert_user = session.createSQLQuery(sql_insert_user);
		query_insert_user.setString(0, userid.toString());
		query_insert_user.setString(1, password);
		query_insert_user.executeUpdate();

		//创建默认用户信息
		String sql_insert_userinfo = "insert into userinfo(userid, username) value(?, ?);";
		SQLQuery query_insert_userinfo = session.createSQLQuery(sql_insert_userinfo);
		query_insert_userinfo.setString(0, userid.toString());
		query_insert_userinfo.setString(1, username);
		query_insert_userinfo.executeUpdate();

		session.getTransaction().commit();

		HibernateSessionFactory.closeSession();
		return userid.toString();
	}

	@Override
	public String updateInfo(String[] info) {
		// TODO Auto-generated method stub
		Session session = HibernateSessionFactory.getSession();
		session.beginTransaction();
		String sql_update = "update userinfo set username=?, sex=?, birthofdate=?, mobile=?, mail=?,personalizedsignature=?"
				+ ",imagepath=?"
				+ "where userid=?";
		SQLQuery query_update = session.createSQLQuery(sql_update);
		query_update.setString(0, info[0]);
		query_update.setString(1, info[1]);
		query_update.setString(2, info[2]);
		query_update.setString(3, info[3]);
		query_update.setString(4, info[4]);
		query_update.setString(5, info[5]);
		query_update.setString(6, info[6]);
		query_update.setString(7, info[7]);
		query_update.executeUpdate();

		session.getTransaction().commit();

		HibernateSessionFactory.closeSession();
		
		return null;
	}

}
