#!bin/bash
mkdir solution

cp -r geometry/.  solution/
cp -r graphs/. solution/
cp -r parsers/. solution/
cp -r requests/. solution/
cp -r transport_directory/. solution/
cp -r main.cpp solution/
cd solution
zip send_me.zip *
cp send_me.zip ../
cd ..
rm  -r solution


