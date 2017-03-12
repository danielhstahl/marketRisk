INCLUDES=-I../GaussNewton  -I../BinomialTree -I../rapidjson -I../AutoDiff  -I../MonteCarlo -I../HullWhite -I../FunctionalUtilities -I../rapidjson/include/rapidjson

marketRisk: main.o
	g++ -std=c++14 -O3 $(STATIC) -w -fPIC main.o  $(LDFLAGS) $(INCLUDES) -o marketRisk -fopenmp

main.o: main.cpp 
	g++ -std=c++14 -O3 $(STATIC) -w -c -fPIC main.cpp $(LDFLAGS) $(INCLUDES) -fopenmp

clean:
	-rm *.o marketRisk
