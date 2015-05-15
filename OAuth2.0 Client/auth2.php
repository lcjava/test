<?php
if (isset($_POST['oauth_token'])) {
	require_once 'http.class.php';
	
	$http = new PPHTTP();
	$array = $_POST;
	$result = $http->httppost('http://127.0.0.1/oauth/userinfo_api.php',$array);
	$pjson = base64_encode($result);
	Header ('Location:auth3.php?result='.$pjson);
	exit();
}

if (!isset($_GET['token']) || empty($_GET['token'])) {
	exit('非法的请求！');
}else{
	$token = $_GET['token'];
}
?>
<!DOCTYPE HTML>
<html>
<head>
<title>请求用户资料 -这是应用</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
<link href="style.css" rel="stylesheet" type="text/css" media="all" />
</head>
<body>
<!-- start header -->
<div class="footer_bg">
  <div class="wrap">
    <div class="wrapper1">
      <div class="menu">
        <ul class="top-nav">
          <li  class="active"><a href="index.htm">首页</a></li>
          <li><a href="oauth/statics/about.htm" class="header_bg">关于</a></li>
          <div class="clear"></div>
        </ul>
        <div class="clear"></div>
      </div>
    </div>
  </div>
</div>
<!-- start main -->
<div class="main_bg">
<div class="wrap">
<div class="wrapper">	
	<div class="main">
		<div class="content">
			<a href="index.htm">
			<h2>这是应用</h2></a>
		</div>
		<div class="about-us">
			<div class="about-header">
				<h2 class="style">&nbsp;</h2>
		  </div>
					<div class="about-info">
					<p class="para">我已经获得了TOKEN，现在要请求用户资料吗？</p>
					<p class="para">附：TOKEN为：<?php echo $token ?></p>
					<form name="form1" method="post" action=''>
					<p><input class="btn" type="submit" value="请求用户资料"></p>
					<input type="hidden" name="oauth_token" id="oauth_token" value="<?php echo $token ?>">
					</form>
					</div>
			<!---End-about---->
		 </div>
	</div>
	<div class="clear"></div>
</div>
</div>
<!-- start footer -->
<div class="footer_bg">
<div class="wrap">
<div class="wrapper">
	<div class="footer">
	  <div class="clear"></div>
		<div class="copy">
		  <p class="w3-link">&copy; Copyright &copy; 2014.Thjx<a href="http://weibo.com/lzwzq68"> @lzwzq68</a> All rights reserved.</p>
		</div>
	</div>
</div>
<div class="clear"></div>
</div>
</div>
</body>
</html>