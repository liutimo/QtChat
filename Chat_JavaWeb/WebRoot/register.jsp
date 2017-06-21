<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
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
<script type="text/javascript">
	function validate()
	{
		var pwd = document.getElementById("pwd").value
		var rpwd = document.getElementById("rpwd").value

		if (pwd == rpwd) {
			document.getElementById("tishi").innerHTML = "";
			document.getElementById("submit").disabled = false;
		} else {
			document.getElementById("tishi").innerHTML = "<font color='red'>两次密码不相同</font>";
			document.getElementById("submit").disabled = true;
		}
		}
	
</script>
</head>
<body>
	<h1>Chat帐号注册</h1>
	<div class="main">
		<div class="login-top left">

			<form action="register" method="post">
				<input type="text" name="username" class="name" placeholder="用户名"/> 
				<input type="password" id="pwd" name="password" class="password" placeholder="密码" />
				<input type="password" id="rpwd" name="repassword" class="password" placeholder="重复密码"
						oninput="validate()"/>
				<span id="tishi"></span>
				<input type="submit" id="submit" value="注册"/>
			</form>
		</div>
		<div class="clear"></div>
	</div>
	<div class="copy-right w3l-agile">
		<p>
			© 2017 Chat Register Form . All Rights Reserved |
			Design by <a href="http://www.liutimo.com">liuzheng</a>
		</p>
	</div>

</body>
</html>
