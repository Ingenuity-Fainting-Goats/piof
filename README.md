# PIOF - PHP Instrumentation Open Framework
![PIOF - PHP Instrumentation Open Framework](https://github.com/ingenuity-ninja/piof/raw/master/images/headerlogo.png)

# What
PIOF - Is a dynamic and modulable instrumentation framework for PHP language.

# Why
It could be useful for Developers, Reverse Engineers, Malware Analyst, Vulnerability Researcher.
With PIOF you should be able to perform:

- Debugging, Tracing and Performance analysis
- Instrumentate code without touching the PHP code
- Instrumentate obfuscated PHP code
- Virtual Patching 
- Prototype IAST and RASP technology

# Where
Tested on:
```
PHP 7.2.10-0ubuntu0.18.04.1 (cli) (built: Sep 13 2018 13:45:02) ( NTS )
Copyright (c) 1997-2018 The PHP Group
Zend Engine v3.2.0, Copyright (c) 1998-2018 Zend Technologies
    with Zend OPcache v7.2.10-0ubuntu0.18.04.1, Copyright (c) 1999-2018, by Zend Technologies
```
# Who
Martino - twitter.com/martinolessio - mlessio6 [ $placeholder_at ] gmail [ $placeholder_dot ] com

Alessandro - twitter.com/rhpco - rakkapriccio [ $placeholder_at ] gmail [ $placeholder_dot ] com

# Quick Start
```
git clone https://github.com/ingenuity-ninja/piof.git
cd piof
./run.sh
```
## Docker
A Docker is used for building and testing process, use the opened shell to interact with the docker machine
```
root@4e7b7dd9f362:/opt/piof/modules#
```
## Extension
The generated extension is available in the following directory
```
root@4e7b7dd9f362:/opt/piof/modules# ls -lah
total 100K
drwxr-xr-x 2 root root 4.0K Feb 19 20:55 .
drwxr-xr-x 1 root root 4.0K Feb 19 20:55 ..
-rwxr-xr-x 1 root root  91K Feb 19 20:55 piof.so
```
It could be included in php.ini or used in command line execution.

## Command line run
The extension is enabled in the `php.ini` configuration file, so it is possible run a command like `php -r "system("ls");"`:
```
root@4e7b7dd9f362:/etc# php -r "system("ls");"

adduser.conf
aliases
alternatives
apache2
apt
bash.bashrc
bindresvport.blacklist
ca-certificates
ca-certificates.conf
cron.d
[...]
```
For explicit extension usage it could be used following command with same output.
```
php -dextension=/opt/piof/modules/piof.so -r "system("ls");"

```

## Logger
All information are logged under following directory `/var/log/piof*`

### Informational
The hooking modules logs information about sink execution in logs file as shown below:
```
root@4e7b7dd9f362:/etc# tail -f /var/log/piof.info.log

Feb 19 21:05:15 4e7b7dd9f362 piof - system - hook[155]: Arguments 1
Feb 19 21:05:15 4e7b7dd9f362 piof - system - hook[155]: Parameter ls
Feb 19 21:12:17 4e7b7dd9f362 piof - md5 - hook[160]: Arguments 1
Feb 19 21:12:17 4e7b7dd9f362 piof - md5 - hook[160]: Parameter admin
```
### Error
All PIOF extension internal error are logged in following file `/var/log/piof.error.log` 

## Hooks Modules
All hooks modules are in following directory `/opt/piof/hooks`. These modules are builded and loaded by the PIOF core extension so they need to be always available in pair with the `piof.so` extension.
The modules are organized in directory, the directory name is associated to the sink name, every modules contains `hook.c` and a `hook.h` files.

```
root@42e0da0df931:/opt/piof/hooks# ls -lah
total 28K
drwxrwxr-x 1 root root 4.0K Feb 19 20:55 .
drwxr-xr-x 1 root root 4.0K Feb 19 20:55 ..
drwxr-xr-x 2 root root 4.0K Feb 19 20:55 build
drwxrwxr-x 2 root root 4.0K Feb 19 20:24 eval
drwxrwxr-x 2 root root 4.0K Feb 19 20:24 include
drwxrwxr-x 2 root root 4.0K Feb 19 20:24 md5
drwxrwxr-x 2 root root 4.0K Feb 19 20:24 system
```
and
```
root@42e0da0df931:/opt/piof/hooks# ls -lah include/
total 16K
drwxrwxr-x 2 root root 4.0K Feb 19 20:24 .
drwxrwxr-x 1 root root 4.0K Feb 19 20:55 ..
-rw-rw-r-- 1 root root  943 Feb 19 20:24 hook.c
-rw-rw-r-- 1 root root  356 Feb 19 20:24 hook.h
```
The building stage compile all modules and move the `.so` compiled file in following directory `/opt/piof/hooks/build`

```
root@42e0da0df931:/opt/piof/hooks/build# ls -lah
total 184K
drwxr-xr-x 2 root root 4.0K Feb 19 20:55 .
drwxrwxr-x 1 root root 4.0K Feb 19 20:55 ..
-rwxr-xr-x 1 root root  43K Feb 19 20:55 eval.so
-rwxr-xr-x 1 root root  43K Feb 19 20:55 include.so
-rwxr-xr-x 1 root root  44K Feb 19 20:55 md5.so
-rwxr-xr-x 1 root root  44K Feb 19 20:55 system.so
```


## Manual Extension Building
```
make clean
phpize
./configure
make
```
## Manuale Modules Building
```
cd /opt/piof/hooks/eval/
gcc -shared -o eval.so -fPIC hook.c -g -Wall -I -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib -I ../../
mv eval.so ../build
```

## Manual Testing

### Hooked function `md5`
```
php -dextension=/opt/piof/modules/piof.so -r "echo md5("admin");"

```

### Hooked function `system`
```
php -dextension=/opt/piof/modules/piof.so -r "system("ls");"

```

# License
3-clause BSD license (BSD-3-Clause)
