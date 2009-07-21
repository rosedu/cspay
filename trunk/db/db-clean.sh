#!/bin/bash

mysql -h koala.cs.pub.ro -u rosedu_cspay -phuashaquou rosedu_cspay < mysql-cspay-init.sql
mysql -h koala.cs.pub.ro -u rosedu_cspay -phuashaquou rosedu_cspay < mysql-cspay-fill.sql

exit 0
