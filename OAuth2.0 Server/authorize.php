<?php

/**
 * @file
 * Sample authorize endpoint.
 *
 * Obviously not production-ready code, just simple and to the point.
 *
 * In reality, you'd probably use a nifty framework to handle most of the crud for you.
 */
header('Content-Type: text/html; charset=utf-8');
require "lib/PDOOAuth2.inc.php";

if (!isset($_GET) || empty($_GET)) {
	echo '请勿非法提交！';
	exit();
}else{
	if (isset($_POST['username']) && isset($_POST['password'])) {
		$user = trim($_POST['username']);
		$pass = trim($_POST['password']);
		
		if (empty($user) || empty($pass)) {
			echo '请先完整填写用户名及密码。';
			exit();
		}
		
		$oauth = new PDOOAuth2();
		
		$result = $oauth->ckUser($user, $pass);

		if ($result === 1) {
			echo '用户不存在！';
			exit();
		}
		
		if ($result === 2) {
			echo '密码错误！';
			exit();
		}
		
		$oauth->finishClientAuthorization(TRUE, $result['uid'], $_GET);
	}else {
		load_view('auth');
	}
}