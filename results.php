<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>SELK - Search results for "<?php print $_POST['search']; ?>"</title>
<link href="./css/layout.css" rel="stylesheet" type="text/css"/>
</head>

<body>
<?php
	mb_internal_encoding("UTF-8");
	include ("./includes/functions.inc");
	
	$searchstr = $_POST["search"];
	$words = returnWords($searchstr);
	
	$sites = findAndSortSites($words);
	
	if (is_array($sites)){
		$results = showResults($sites);
		$index = 1;
		for($i = 0; $i < sizeof($results); $i++)
		{
			$index;
			$url = $results[$i]['url'];
			$title = $results[$i]['title'];
			print "<p>$index. <a href=\"$url\">$title</a></p>";
			$index++;
		}
	}
	else
		print $sites;
?>
</body>
</html>
