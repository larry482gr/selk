<?php
	function returnWords ($searchstr){
		mb_internal_encoding("UTF-8");
		//mb_regex_encoding('UTF-8');
		$words = explode(" ", $searchstr, 11);
		// limit search to 10 words
		if (sizeof($words) > 10)
			array_pop($words);
		// don't search for words containing less than three letters
		foreach($words as $key => $word) {
			if (mb_strlen($word, "UTF-8") < 4) {
				unset($words[$key]);
				continue;
			}
		//	if (!mb_ereg_match("/^[a-zA-Z]*$|^~\p{Greek}*$~u/", $words[$key]))
		//		unset($words[$key]);
		}
		$words = array_merge($words);
		foreach($words as $key => $word) {
			$words[$key] = mb_strtolower($word, "UTF-8");
		}
		$words = array_reverse($words);
		return $words;
	}
	
	function findAndSortSites($words)
	{
		include ("./includes/dbconnect.inc");
		$db_connect = @mysql_connect($dbhost, $dbuser, $dbpass)
		or die("Η σύνδεση με τη βάση δεδομένων απέτυχε.");
		@mysql_select_db($dbname,$db_connect)
		or die("Η σύνδεση με τη βάση δεδομένων απέτυχε.");
		mysql_query("SET NAMES 'utf8'");
		mysql_query("SET CHARACTER SET utf8");
		$counter = 0;
		foreach ($words as $word)
		{
			$counter++;
			$query = "SELECT url, freq FROM wordfreq WHERE word LIKE '$word%'";
			$result = mysql_query ($query);
		
			while ($wordfreq = mysql_fetch_row($result))
			{
				if ($counter < 2)
					$sites[] = array('url' => $wordfreq[0], 'freq' => $wordfreq[1], 'nowords' => 1);
				else
				{
					$site_exists = false;
					for($i = 0; $i < sizeof($sites); $i++)
					{
						if ($sites[$i]['url'] == $wordfreq[0])
						{
							//increase frequency based on priority
							$sites[$i]['freq'] += $wordfreq[1] + ($counter*5);
							//increase number of words found on a specific site
							$sites[$i]['nowords'] += 1;
							$site_exists = true;
							break;
						}
					}
				
					if (!$site_exists)
						$sites[] = array('url' => $wordfreq[0], 'freq' => $wordfreq[1], 'nowords' => 1);
				}
			}
		}
		if (isset($sites))
		{
			foreach ($sites as $key => $row) {
    			$freq[$key] = $row['freq'];
				$nowords[$key] = $row['nowords'];
			}
			array_multisort($nowords, SORT_DESC, $freq, SORT_DESC, $sites);
			return $sites;
		}
		else
		{
			$sites = "Sorry. There are no results matching your query";
			return $sites;
		}
	}
	
	function showResults ($sites) {
		for($i = 0; $i < sizeof($sites); $i++)
		{
			$url = $sites[$i]['url'];
			$query = "SELECT title FROM urls WHERE url = '$url'";
			$result = mysql_query ($query);
			$title = mysql_result($result, 0, 0);
			if(empty($title))
			{
				$title=$url;
			}
			$results[] = array('url' => $url, 'title' => $title);
		}
		return $results;
	}
?>
