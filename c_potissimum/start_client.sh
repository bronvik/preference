wget -P ./  https://ekondrat.web.cern.ch/ekondrat/.my_ip > /dev/null 2> /dev/null
#./a.out `cat ./.my_ip`
./_client `cat ./.my_ip` $1
rm -f .my_ip






