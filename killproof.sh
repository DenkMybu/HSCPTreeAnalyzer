for i in `ps -u | grep proof | awk '{print $2}'` ; do kill -9 $i ; done
