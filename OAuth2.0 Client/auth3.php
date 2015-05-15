<?php
if (!isset($_GET['result']) || empty($_GET['result'])) {
	exit('非法的请求！');
}else{
	$result = base64_decode($_GET['result']);
}
?>
<!DOCTYPE HTML>
<html>
<head>
<title>用户资料 -这是应用</title>
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
					<p class="para">我已经获得了用户资料</p>
					<p class="para">附：用户资料JSON为：</p>
					<p><?php print_r($result) ?></p>
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