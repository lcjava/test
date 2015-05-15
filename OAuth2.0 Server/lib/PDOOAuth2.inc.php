<?php

/**
 * @file
 * Sample OAuth2 Library PDO DB Implementation.
 */

require_once 'OAuth2.inc.php';
require_once 'conn.inc.php';
require_once 'func.inc.php';

/**
 * OAuth2 Library PDO DB Implementation.
 */
class PDOOAuth2 extends OAuth2 {

  private $db;

  /**
   * Overrides OAuth2::__construct().
   */
  public function __construct() {
    parent::__construct();

    try {
      $this->db = new PDO(PDO_DSN, PDO_USER, PDO_PASS);
    } catch (PDOException $e) {
      die('Connection failed: ' . $e->getMessage());
    }
  }

  /**
   * Release DB connection during destruct.
   */
  function __destruct() {
    $this->db = NULL; // Release db connection
  }

  /**
   * Handle PDO exceptional cases.
   */
  private function handleException($e) {
    echo "Database error: " . $e->getMessage();
    exit;
  }

  /**
   * Little helper function to add a new client to the database.
   *
   * Do NOT use this in production! This sample code stores the secret
   * in plaintext!
   *
   * @param $client_id
   *   Client identifier to be stored.
   * @param $client_secret
   *   Client secret to be stored.
   * @param $redirect_uri
   *   Redirect URI to be stored.
   */
  public function addClient($appname, $client_secret, $redirect_uri) {
    try {
      $sql = "INSERT INTO clients (appname, client_secret, redirect_uri) VALUES (:appname, :client_secret, :redirect_uri)";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":appname", $appname, PDO::PARAM_STR);
      $stmt->bindParam(":client_secret", $client_secret, PDO::PARAM_STR);
      $stmt->bindParam(":redirect_uri", $redirect_uri, PDO::PARAM_STR);
      $stmt->execute();
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Implements OAuth2::checkClientCredentials().
   *
   * Do NOT use this in production! This sample code stores the secret
   * in plaintext!
   */
  protected function checkClientCredentials($client_id, $client_secret = NULL) {
    try {
      $sql = "SELECT client_secret FROM clients WHERE client_id = :client_id";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":client_id", $client_id, PDO::PARAM_STR);
      $stmt->execute();

      $result = $stmt->fetch(PDO::FETCH_ASSOC);

      if ($client_secret === NULL)
          return $result !== FALSE;

      return $result["client_secret"] == $client_secret;
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Implements OAuth2::getRedirectUri().
   */
  protected function getRedirectUri($client_id) {
    try {
      $sql = "SELECT redirect_uri FROM clients WHERE client_id = :client_id";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":client_id", $client_id, PDO::PARAM_STR);
      $stmt->execute();

      $result = $stmt->fetch(PDO::FETCH_ASSOC);

      if ($result === FALSE)
          return FALSE;

      return isset($result["redirect_uri"]) && $result["redirect_uri"] ? $result["redirect_uri"] : NULL;
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }
  
	public function getClientID($client_sec,$appname) {
    try {
      $sql = "SELECT `client_id` FROM `clients` WHERE `client_secret` = :client_sec AND `appname` = :appname ";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(':client_sec', $client_sec, PDO::PARAM_STR);
      $stmt->bindParam(':appname', $appname, PDO::PARAM_STR);
      $stmt->execute();

      $result = $stmt->fetch(PDO::FETCH_ASSOC);

      if ($result === FALSE)
          return FALSE;

      return isset($result['client_id']) && $result['client_id'] ? $result['client_id'] : NULL;
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Implements OAuth2::getAccessToken().
   */
  protected function getAccessToken($oauth_token) {
    try {
      $sql = "SELECT client_id, expires, scope FROM tokens WHERE oauth_token = :oauth_token";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":oauth_token", $oauth_token, PDO::PARAM_STR);
      $stmt->execute();

      $result = $stmt->fetch(PDO::FETCH_ASSOC);

      return $result !== FALSE ? $result : NULL;
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Implements OAuth2::setAccessToken().
   */
  protected function setAccessToken($oauth_token, $client_id, $expires, $uid, $scope = NULL) {
    try {
      $sql = "INSERT INTO tokens (oauth_token, client_id, expires, uid, scope) VALUES (:oauth_token, :client_id, :expires, :uid, :scope)";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":oauth_token", $oauth_token, PDO::PARAM_STR);
      $stmt->bindParam(":client_id", $client_id, PDO::PARAM_STR);
      $stmt->bindParam(":expires", $expires, PDO::PARAM_INT);
      $stmt->bindParam(":uid", $uid, PDO::PARAM_INT);
      $stmt->bindParam(":scope", $scope, PDO::PARAM_STR);

      $stmt->execute();
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Overrides OAuth2::getSupportedGrantTypes().
   */
  protected function getSupportedGrantTypes() {
    return array(
      OAUTH2_GRANT_TYPE_AUTH_CODE,
    );
  }

  /**
   * Overrides OAuth2::getAuthCode().
   */
  protected function getAuthCode($code) {
    try {
      $sql = "SELECT code, client_id, redirect_uri, expires, uid, scope FROM auth_codes WHERE code = :code";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":code", $code, PDO::PARAM_STR);
      $stmt->execute();

      $result = $stmt->fetch(PDO::FETCH_ASSOC);

      return $result !== FALSE ? $result : NULL;
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }

  /**
   * Overrides OAuth2::setAuthCode().
   */
  protected function setAuthCode($code, $client_id, $redirect_uri, $expires, $uid, $scope = NULL) {
    try {
      $sql = "INSERT INTO auth_codes (code, client_id, redirect_uri, expires, uid, scope) VALUES (:code, :client_id, :redirect_uri, :expires, :uid, :scope)";
      $stmt = $this->db->prepare($sql);
      $stmt->bindParam(":code", $code, PDO::PARAM_STR);
      $stmt->bindParam(":client_id", $client_id, PDO::PARAM_STR);
      $stmt->bindParam(":redirect_uri", $redirect_uri, PDO::PARAM_STR);
      $stmt->bindParam(":expires", $expires, PDO::PARAM_INT);
      $stmt->bindParam(":uid", $uid, PDO::PARAM_INT);
      $stmt->bindParam(":scope", $scope, PDO::PARAM_STR);

      $stmt->execute();
    } catch (PDOException $e) {
      $this->handleException($e);
    }
  }
  
	public function ckUser($username,$password) {
		try {
	      	$sql = "SELECT `password`,`uid` FROM `user` WHERE `username` = :username";
			$stmt = $this->db->prepare($sql);
			$stmt->bindParam(":username", $username, PDO::PARAM_STR);
			$stmt->execute();
			
			$result = $stmt->fetch(PDO::FETCH_ASSOC);
			if ($result === FALSE) {
		    	return 1;
		    }
		    
		    if ($password != $result['password']) {
		    	return 2;
		    }
		    return $result;
	    } catch (PDOException $e) {
	      $this->handleException($e);
	    }
	}
	
	public function getUserInfo($oauth_token) {
		try {
	      	$sql = "SELECT `uid` FROM `tokens` WHERE `oauth_token` = :oauth_token";
			$stmt = $this->db->prepare($sql);
			$stmt->bindParam(":oauth_token", $oauth_token, PDO::PARAM_STR);
			$stmt->execute();
			
			$result = $stmt->fetch(PDO::FETCH_ASSOC);
			if ($result === FALSE) {
		    	return 1;
		    }
		    
		    $sql = "SELECT `email`,`phone`,`img` FROM `user` WHERE `uid` = :uid";
			$stmt = $this->db->prepare($sql);
			$stmt->bindParam(":uid", $result['uid'], PDO::PARAM_STR);
			$stmt->execute();
			
			$result = $stmt->fetch(PDO::FETCH_ASSOC);
			$result_json = json_encode($result);
			
		    return $result_json;
	    } catch (PDOException $e) {
	      $this->handleException($e);
	    }
	}
}