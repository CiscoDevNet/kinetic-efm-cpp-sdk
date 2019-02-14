CFLAGS = -std=c++11 -Wall -Wextra -I ../../include -g -O2 -D_FORTIFY_SOURCE=2 -fPIE -fstack-protector
LDFLAGS = -L ../../lib -pie -Wl,-z,now
LIBS = -lboost_log -lboost_date_time -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lssl -lcrypto -ldl -pthread

.PHONY: all clean
all: simple-responder simple-responder-shared

DEPS = error_code.h
OBJ = error_code.o main.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

simple-responder: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp-static $(LIBS)

simple-responder-shared: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp $(LIBS)

run: simple-responder
	./simple-responder

run-shared: simple-responder-shared create-libcrypto-symlink
	LD_LIBRARY_PATH=../../lib ./responder-shared

clean:
	$(RM) simple-responder simple-responder-shared $(OBJ)

