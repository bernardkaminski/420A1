#!/bin/bash

for i in {0..9};
do
	time ./$1 test.png out.png $2;
done
