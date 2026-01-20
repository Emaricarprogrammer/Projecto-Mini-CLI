print("""=============== Bem-vindo ao seu sistema via terminal ==============
	////////////////				      ///////////////
				////////////////////
				|
				|
				|
				|
				|//////////////////
				|
				|
				|
				|
				|
				////////////////////

|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
""")

import os

print("""
	1-Get Ip
	2-Create File
	3-Create Folder
""")
while True:
	choise = int(input(":"))
	if(choise == 1):
		print(os.system("ip a"))
		print(os.system("clear"))
	elif(choise == 2):
		print("filename:")
		name = str(input(":"))
		print("file type:")
		fileType = str(input(":"))
		print(os.system(f'touch ${name}.${fileType}'))
	else:
		print("invalid choice")

