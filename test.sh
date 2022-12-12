#!/bin/sh

cd build
for example in ../tests/*.rpn
do
	echo "============================================================"
	echo "Testing $example"
	echo "============================================================"
	./RPNlang "$example"
	if [ $? -ne 0 ]
	then
		echo "Test $example failed"
		exit 1
	fi
	echo
done

echo "Done"