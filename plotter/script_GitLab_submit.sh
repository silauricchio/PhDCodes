#!/bin/bash                                                                                                                                                                              

echo ""
echo "------------------------------------------------"

echo "Commit all the source directory"

git add source

git status 

echo ""
echo "------------------------------------------------"

echo "Type a message for committing:"

read message

git commit -m "$message"

echo ""
echo "------------------------------------------------"

git status

echo ""
echo "------------------------------------------------"


git push origin master
