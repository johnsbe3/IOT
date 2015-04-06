## BEGIN INIT INFO
# Provides: controller python script 
# Short Description: Starts water monitor python script
# Description:
### END INIT INFO

start on started mysql

#Process name (for display)
NAME=controller

case $1 in 
     start)
	echo "Starting controller" >> /var/log/controller.log
	#run application you want to start
	/bin/su controller && /usr/bin/sudo python /home/controller/meter/meter.py >> /var/log/controller.log &
	;;
     stop)
	echo "Stopping controller" >> /var/log/controller.log
	#kill application you want to stop
	/bin/su controller && /usr/bin/sudo killall python >> /var/log/controller.log 
	;;
     *)
	echo "Usage: /etc/init.d/launcher.sh{start|stop}"
	exit 1
	;;
esac
exit 0	
