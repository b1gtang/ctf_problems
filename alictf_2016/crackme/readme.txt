Crackme

# mail:root@bigtang.org
# tele:18710892708

考查点:
	1.debug block
	2.花指令
	3.tea算法

其中crackme.exe为测试文件,flag为32位数字加
flag:37c07ba395536352169f4113a9c22283

考虑到反作弊系统，可使每支队伍的flag与其下载的文件不同
见test.py 根据用户名或队伍名其相应的flag，并把flag的校验值（16位），写入文件偏移（0x7030）
并生成test.exe
