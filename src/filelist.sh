#!/bin/bash

ls -l ./filelist/ | awk ' $1 != "total" {print $9}' > ./filelist.txt
