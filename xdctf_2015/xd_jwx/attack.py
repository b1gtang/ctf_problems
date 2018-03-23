
import requests

def submit(flag):
	url = "http://172.16.0.110/index.php/wargame/submit"

	_data = {"key":flag}
	_cookie = {
		"PHPSESSID":"euc87h0n28s4id0grv36sluvq0",
		"xdgame_username":"Bigetang"
	}

	r = requests.post(url,data=_data,cookies=_cookie)
	if "KEY" in r.content:
		print 1123

def main():
	flag = '12314'
	submit(flag)



if __name__ == '__main__':
	main()


