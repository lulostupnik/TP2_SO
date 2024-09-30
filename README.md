Compiling instructions:

git clone git@github.com:lulostupnik/TP2_SO.git
cd TP2_SO
docker run -v ${PWD}:/root --privileged -ti --name SO agodio/itba-so-multi-platform:3.0 
exit
./run.sh