<?php
class azimeAdaFruit 
{
	public $key;
	public $url;
	
	public function __construct($key, $url="http://io.adafruit.com")
	{
		$this->key = $key;
		$this->url = $url;
	}
	
	/**
	 * Returns a feed wrapper
	 * @param string $name
	 * @return AdaFruitIOFeed
	 */
	public function getFeed($name)
	{
		return new AdaFruitIOFeed($this->key, $name, $this->url);
	}
	
	/**
	 * Returns the all the feed names in an array
	 * @return array of string
	 */
	public function getFeedNames()
	{
		$url = $this->url."/api/feeds.json";
		
		$c = curl_init($url);
		
		$headers = array();
		$headers[] = "X-AIO-Key: ".$this->key;
		curl_setopt($c, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
		
		$json = json_decode(curl_exec($c));

		curl_close($c);
		
		$arr = array();
		foreach($json as $j)
			$arr[] = $j->name;
		
		return $arr;
		
	}
}

/**
 * Represents a feed wrapper.
 * Provides methods to push data, and retrieve the last data pushed to the feed.
 */
class AdaFruitIOFeed
{
	public $key;
	public $url;
	public $name;
	
	public function __construct($key, $name, $url)
	{
		$this->key = $key;
		$this->name = $name;
		$this->url = $url;
	}
	
	/**
	 * Pushes a new value to the feed in a json form
	 * 
	 * @param mixed $value
	 * @param string $quoted : indicates if the json data must be wrapped in quotes
	 */
	public function send($value, $quoted = false)
	{
		$req = '{"value":';
		
		if($quoted) $req .= '"';
		
		$req .= $value;
		
		if($quoted) $req .= '"';
		
		$req .= '}';
		
		
		$url = $this->url."/api/feeds/".$this->name."/data/send";
		$res = $this->sendRequest($url, true, $req);
		
		return $res;
	}
	
	/**
	 * Retrieves the last value of this feed
	 * @return mixed
	 */
	public function get()
	{
		$url = $this->url."/api/feeds/".$this->name."/data/last.txt";
		
		return $this->sendRequest($url);
	}
	
	/**
	 * Sends an HTTP request 
	 * @param string $body
	 */
	protected function sendRequest($url, $isPOST = false, $body = "")
	{
				
		$c = curl_init($url);
		
		$headers = array();
		$headers[] = "X-AIO-Key: ".$this->key;
		
		
		if($isPOST)
		{
			curl_setopt($c, CURLOPT_POST, true);
			$headers[] = "Content-Type: application/json";
			curl_setopt($c, CURLOPT_POSTFIELDS, $body);
		}
		
		curl_setopt($c, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($c, CURLOPT_RETURNTRANSFER, true);
				
		$res = curl_exec($c);
				
		curl_close($c);
		
		return $res;
	}
}