<?php

/**
 * @file
 * Sample client add script.
 *
 * Obviously not production-ready code, just simple and to the point.
 */
header('Content-Type: text/html; charset=utf-8');
require_once "lib/PDOOAuth2.inc.php";

if ($_POST && isset($_POST['appname']) && isset($_POST['rurl'])) {
  	$appname = trim($_POST['appname']);
	$rurl = trim($_POST['rurl']);
	
	if (empty($appname) || empty($rurl)) {
		echo '添加失败，请完整填写应用名及回调地址。';
		exit();
	}
  	
	$oauth = new PDOOAuth2();
  	$client_sec = client_secret_generate();
  	$oauth->addClient($_POST['appname'],$client_sec, $_POST['rurl']);
  	$client_id = $oauth->getClientID($client_sec,$appname);
  	echo '成功地注册了应用</br>您的Client_ID:'.$client_id.'</br>您的Client_Secret:'.$client_sec.'</br><a href="/">点此返回首页</a>';
  	exit();
}else {
	load_view('appreg');
}