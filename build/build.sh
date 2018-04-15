#!/bin/sh

SCRIPT=$(readlink -f "$0")
SCRIPTDIRNAME=$(dirname "$SCRIPT")

make -f $SCRIPTDIRNAME/../Makefile $1
