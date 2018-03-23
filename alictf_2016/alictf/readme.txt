搭建环境:ubuntu14.04 64bit
sudo apt-get install socat
socat tcp-l:2333,reuseaddr,fork exec:./pwn &

注意：
	为pwn单独创建一个用户，并以该用户执行socat
	删掉kill pkill
	flag文件可变化