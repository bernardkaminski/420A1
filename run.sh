#!/bin/bash

for i in {0..9};
do
	time ./rectify test.png out.png $1;
done
