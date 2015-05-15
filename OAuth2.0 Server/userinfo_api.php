<?php

/**
 * @file
 * Sample protected resource.
 *
 * Obviously not production-ready code, just simple and to the point.
 *
 * In reality, you'd probably use a nifty framework to handle most of the crud for you.
 */

require 'lib/PDOOAuth2.inc.php';

$oauth = new PDOOAuth2();
$status = $oauth->verifyAccessToken();

if ($status === TRUE) {
	$result = $oauth->getUserInfo($_POST['oauth_token']);
	echo $result;
}

// With a particular scope, you'd do:
// $oauth->verifyAccessToken("scope_name");

?>