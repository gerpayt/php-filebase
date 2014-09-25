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

echo filebase_get('World');

?>
