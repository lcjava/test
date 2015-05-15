<?php
/**
 * @file
 * OAuth2.0 Lib Func.
 */

function client_secret_generate () {
	$i=0;
	$k1 = '';
	while($i<32) {
		$rnd = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'; 
		$k1 = $k1.$rnd[rand(0,61)];
		$i++;
	}
		
	return($k1);
}

function load_view($view='',$array=array()) {
	if(	file_exists(OAuth_DIR.'/tpl/main_'.$view.'.htm') == true) {
		$$view = $array;
		require_once OAuth_DIR.'/tpl/main_'.$view.'.htm';
	}else{
		exit('系统内部错误，请联系管理员解决此问题！<a href="/">点此返回首页</a>');
	}
}