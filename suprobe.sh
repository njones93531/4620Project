#!/bin/bash 
  
su $2 -c bash << EOF
$1
echo "Password is $1" > su.txt
kill -9 $3
EOF