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
<title>更新资料</title>
</head>
<body>
	<div class="main">
		<div style="margin:0 auto;">
			<h1 style="text-align: center">完善你的资料</h1>
			<div style="width:100px; height:100px; margin:0 auto; border-radius:50%; overflow:hidden;">
				<img src="http://127.0.0.1/123456.jpg" alt="上海鲜花港 - 郁金香"
					style="width:100%;height:100%;" />
			</div>
			<s:form action="update" enctype="multipart/form-data" cssStyle="margin:0 auto;" method="post">
				<s:hidden name="userid" value="%{#session.userid}"/>
				<s:textfield label="用户名" name="username" value="%{#session.username}"/>
				<s:select list="#{1:'男', 2:'女'}" name="sex" value="男" label="性别" />
				<s:textfield label="生日" name="birthofdate" />
				<s:textfield label="手机" name="mobile" />
				<s:textfield label="邮箱" name="mail" />
				<s:textarea label="个性签名" name="personalizedsignature" />
				<s:file label="更换头像" name="file"/>
				<s:submit value="保存"/>
			</s:form>
		</div>
	</div>
	<div class="copy-right w3l-agile">
		<p>
			© 2017 Chat Register Form . All Rights Reserved | Design by <a
				href="http://www.liutimo.com">liuzheng</a>
		</p>
	</div>

</body>
</html>
