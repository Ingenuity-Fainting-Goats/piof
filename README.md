[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![star this repo](http://githubbadges.com/star.svg?user=boennemann&repo=badges&style=flat)](https://github.com/boennemann/badges)
[![fork this repo](http://githubbadges.com/fork.svg?user=boennemann&repo=badges&style=flat)](https://github.com/boennemann/badges/fork)

# PIOF - PHP Instrumentation Open Framework
![PIOF - PHP Instrumentation Open Framework](https://github.com/ingenuity-ninja/piof/raw/master/images/headerlogo.png)

# What
PIOF - Is a dynamic and modular instrumentation framework for PHP.

# Why
It is useful for Developers, Reverse Engineers, Malware Analyst and Vulnerability Researcher.
With PIOF you can:

- Perform debugging, tracing and performance analysis
- Instrument PHP code on the interpreter layer
- Instrument obfuscated PHP code
- Virtual Patching 
- Prototype IAST and RASP technologies

# Where
Tested on:
```
PHP 7.2.10-0ubuntu0.18.04.1 (cli) (built: Sep 13 2018 13:45:02) ( NTS )
Copyright (c) 1997-2018 The PHP Group
Zend Engine v3.2.0, Copyright (c) 1998-2018 Zend Technologies
    with Zend OPcache v7.2.10-0ubuntu0.18.04.1, Copyright (c) 1999-2018, by Zend Technologies
```
# Who
Martino - twitter.com/martinolessio

Alessandro - twitter.com/rhpco

# Quick Start
```
git clone https://github.com/ingenuity-ninja/piof.git
cd piof
./run.sh
```
## Docker
A Docker container is used for the building and testing stages. Use the container shell to interact with the PIOF environment:
```
root@4e7b7dd9f362:/opt/piof/modules#
```
## Extension
The generated PHP extension is available in `/opt/piof/modules`
```
root@4e7b7dd9f362:/opt/piof/modules# ls -lah
total 100K
drwxr-xr-x 2 root root 4.0K Feb 19 20:55 .
drwxr-xr-x 1 root root 4.0K Feb 19 20:55 ..
-rwxr-xr-x 1 root root  91K Feb 19 20:55 piof.so
```
You can either include it in your php.ini or use it from the command line.

## Command line run
When the extension is enabled in `php.ini` you can execute php as usual, for example:
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
To use the module at runtime you can use the following command:
```
php -dextension=/opt/piof/modules/piof.so -r "system("ls");"

```

## Logger
PIOF logs in `/var/log/piof*`

### Informational
The hooking modules logs information about sink execution:
```
root@4e7b7dd9f362:/etc# tail -f /var/log/piof.info.log

Feb 19 21:05:15 4e7b7dd9f362 piof - system - hook[155]: Arguments 1
Feb 19 21:05:15 4e7b7dd9f362 piof - system - hook[155]: Parameter ls
Feb 19 21:12:17 4e7b7dd9f362 piof - md5 - hook[160]: Arguments 1
Feb 19 21:12:17 4e7b7dd9f362 piof - md5 - hook[160]: Parameter admin
```
### Error
All internal error are logged in `/var/log/piof.error.log` 

## Hooks Modules
Hooking modules are in located in `/opt/piof/hooks`. These modules are dynamically built and loaded by the PIOF core extension so they need to be always available with `piof.so`.
The modules are organized hierarchically in directories: each directory is associated to a sink, and each one contains a `hook.c` and a `hook.h` file.

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
During the building stage PIOF compiles all modules and move the generated `.so` files in `/opt/piof/hooks/build`

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
