# use monitor 3, monitor 5 etc..., default is 5
idf.py -p /dev/ttyS${1:=5} monitor
