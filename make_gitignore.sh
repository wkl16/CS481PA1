#!/usr/bin/bash
ROOT=$(pwd)
FILES=$(find ./* -name "Makefile")
GITIGNORE=$ROOT/.gitignore

rm -f $GITIGNORE
for FILE in $FILES; do
    DIR=$(dirname $FILE)
    cd $DIR
    GITIGNORE=$GITIGNORE make $GITIGNORE
    cd $ROOT
done
