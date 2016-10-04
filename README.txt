LINUX

UNCOMPRESS
$ cd ~/Documents/PERSO/QSync
$ rsync -avz belzel@linux2:~/Téléchargements/QSync* .
$ unzip QSync\ 20160508.zip

COMPRESS
$ tar -cvzf QSync$(date +"%Y%m%d").tgz *
$ rsync -avz QSync20160510.tgz belzel@linux2:~/Téléchargements/.