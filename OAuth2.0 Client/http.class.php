<?php
 /**
 * LightPass HTTP Get/Post Lib.
 * With CURL
 * Verson 1.5
 * Author:lzwzq68
 */

class PPHTTP {
	
	public function httpget($reurl='',$rearray=array('')) {
		
		if (empty($reurl)) {
			return (false);
		}
		
		$restring = $this->array2string($rearray);
		
		$ch = curl_init() ;
		curl_setopt($ch, CURLOPT_URL, $reurl.$restring);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true) ; 
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_USERAGENT, 'LightPass/X1 (With CURL)');
		$output = curl_exec($ch);
		if (!$output){
			return (false);
		}else {
			curl_close($ch);
			return $output;
		} 	
	}
	
	public function httppost($reurl='',$rearray=array('')){
		if (empty($reurl) || empty($rearray)) {
			return (false);
		}
		
		$post_data = $rearray;
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $reurl);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_USERAGENT, 'LightPass/X1 (With CURL)'); 
		$output = curl_exec($ch);
		if (!$output){
			return (false);
		}else {
			curl_close($ch);
			return $output;
			
		}
	}
	
	public function array2string($rearray = array('')){
		$pre_string = '';
		foreach ($rearray as $key=>$value) {
			$pre_string = $pre_string.$key.'='.$value.'&';
		}
		$restring = substr($pre_string,0,strlen($pre_string)-1);
		return $restring;
	}
}
