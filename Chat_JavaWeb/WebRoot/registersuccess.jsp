<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://" + request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
%>

<!DOCTYPE html>
<html>
<head>
<title>Impressive Login & Sign up Forms Flat Responsive Widget
	Template :: w3layouts</title>
<link href="css/style.css" rel='stylesheet' type='text/css' />
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="keywords"
	content="Impressive Login & Sign up Forms Responsive, Login form web template, Sign up Web Templates, Flat Web Templates, Login signup Responsive web template, Smartphone Compatible web template, free webdesigns for Nokia, Samsung, LG, Sony Ericsson, Motorola web design" />
<!-- //end-smoth-scrolling -->
<link
	href='//fonts.googleapis.com/css?family=Open+Sans:300,400,400italic,600,600italic,700,300italic'
	rel='stylesheet' type='text/css'/>
</head>
<body>
	<h1></h1>
	<div style="width:50%; height: 400px; margin: 0 auto;">
		<p style="text-align: center; font-size:16px;color:white;">恭喜你注册成功！！！</p>
		<p style="text-align: center; font-size:16px;color:white;">
		
		 请记住你的Chat帐号：<span style="color:#00FF80; font-size:30px;"><s:property value="#session.userid"/></span>
			该帐号是你使用Chat的唯一标识符
		</p>
		
		<p style="text-align: center; font-size:16px;"><a href="updateinfo.jsp" style="color:#E08031;">现在去完善资料?</a>
		</p>
		
	</div>
	<div class="copy-right w3l-agile">
		<p>
			© 2017 Chat Register Form . All Rights Reserved |
			Design by <a href="http://www.liutimo.com">liuzheng</a>
		</p>
	</div>

</body>
</html>
