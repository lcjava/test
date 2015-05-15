<?php
if (isset($_POST['code'])) {
	require_once 'http.class.php';
	
	$http = new PPHTTP();
	$array = $_POST;
	$result = $http->httppost('http://127.0.0.1/oauth/token.php',$array);
	
	$assoc = json_decode($result,true);
	Header ('Location:auth2.php?token='.$assoc['access_token']);
	exit();
}

if (!isset($_GET['code']) || empty($_GET['code'])) {
	exit('非法的请求！');
}else{
	$code = $_GET['code'];
}
?>
<!DOCTYPE HTML>
<html>
<head>
<title>请求TOKEN -这是应用</title>
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
					<p class="para">我已经获得了CODE，现在要请求TOKEN吗？</p>
					<p class="para">附：CODE为：<?php echo $code ?></p>
					<form name="form1" method="post" action=''>
					<p><input class="btn" type="submit" value="请求TOKEN"></p>
					<input type="hidden" name="grant_type" id="grant_type" value="authorization_code">
					<input type="hidden" name="code" id="code" value="<?php echo $code ?>">
					<input type="hidden" name="redirect_uri" id="redirect_uri" value="http://127.0.0.1/auth.php">
					<input type="hidden" name="client_id" id="client_id" value="1000000012">
					<input type="hidden" name="client_secret" id="client_secret" value="doeLO2vJOlnwhA4Cv08YAUoxzJRtWisK">
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