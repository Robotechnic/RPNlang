#!/bin/sh

cd build
for example in ../tests/*
do
	echo "============================================================"
	echo "Testing $example"
	echo "============================================================"
	./RPNlang "$example"
	if [ $? -ne 0 ]
	then
		echo "Test $example failed"
	fi
	echo
done

echo "Done"