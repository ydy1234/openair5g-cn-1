#!/bin/sh
pgname="fake_gNB"


pgid=$(ps -ef | grep $pgname | grep -v grep | awk '{print $2}')
if [ "$1" = "request" ];then
     echo  "ng setup request......"
     sleep 1
     kill -s USR1 $pgid

elif [ "$1" = "initial" ];then
     echo  "initial ue message......"
     sleep 1
     kill -s USR2 $pgid
else 
     echo "usage:"
     echo "   ./fire.sh request, input setup request info"
     echo "   ./fire.sh initial, input initial ue message info"
fi
