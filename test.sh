#!/bin/sh

for example in ./tests/*
do
	echo "Testing $example"
	./build/RPNlang "$example"
	if [ $? -ne 0 ]
	then
		echo "Test $example failed"
	fi
done

echo "Done"