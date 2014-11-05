php-filebase
============

a php extension to manage a large number of text files

introduction
------------

include 3 functions
### integer filebase_put(string bucket_name, string path_name, string content);
Put a text file into filebase, you need to provide bucket name, path name and text file content, the extension automatically create a 2-level directory to store the text file, ex. filebase_put('test', '12345', 'content'); will store 'content' in 'test/45/12345' where '45' is the last 2 characters in path_name. The path is based on 'filebase.root' variable in ini file.
### string filebase_get(string bucket_name, string path_name);
Get text file from filebase, you need to provide bucket name and path name, the extension return file content if the file existes else return false.
### string filebase_del(string bucket_name, string path_name);
Delete text file from filebase, provide bucket name and path name, return true if delete success else return false.

installation
------------

Download the package, unzip it and cd to the directory.
````sh
phpize
./configure
make
make install
````
Add configuration to php.ini
````ini
extension=filebase.so
filebase.root="/usr/local/filebase"
filebase.debug="0"
````
You can also set params in php file by using ini_set() function.

Restart apache server, view phpinfo page, you can find filebase information.

