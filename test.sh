#!/bin/sh

for example in ./tests/*
do
	echo "============================================================"
	echo "Testing $example"
	echo "============================================================"
	./build/RPNlang "$example"
	if [ $? -ne 0 ]
	then
		echo "Test $example failed"
	fi
	echo
done

echo "Done"