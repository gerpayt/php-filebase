<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('filebase')) {
	dl('filebase.' . PHP_SHLIB_SUFFIX);
}
$module = 'filebase';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}

$article = filebase_get('article', '12345');
if ($article)
{
    echo $article;
}
else
{
    echo "Error\n";
}

$article = "This is a very long text";
$length = filebase_put('article', '54321', $article);
echo "write len" . $length;

?>
