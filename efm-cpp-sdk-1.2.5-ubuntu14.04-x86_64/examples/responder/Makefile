CFLAGS = -std=c++11 -Wall -Wextra -I ../../include -g -O2 -D_FORTIFY_SOURCE=2 -fPIE -fstack-protector
LDFLAGS = -L ../../lib -pie -Wl,-z,now
# -latomic is only required on some platforms
LIBS = -lboost_log -lboost_date_time -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lssl -lcrypto -ldl -lpthread -latomic

LINK_NAME=responder
VERSION=1.2.5
DESCRIPTION="Responder Link Example"

.PHONY: all clean
all: responder responder-shared

DEPS = error_code.h responder_link.h
OBJ = error_code.o main.o

%.o: %.cpp $(DEPS)
	$(CXX) -DLINK_VERSION="\"$(VERSION)\"" -c -o $@ $< $(CFLAGS)

responder: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp-static $(LIBS)

responder-shared: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp $(LIBS)

run: responder
	./responder

run-shared: responder-shared
	LD_LIBRARY_PATH=../../lib ./responder-shared

package: responder
	../../packaging/package_link.sh $(LINK_NAME) ./responder $(VERSION) $(DESCRIPTION)

package-shared: responder-shared
	../../packaging/package_link.sh $(LINK_NAME) ./responder-shared $(VERSION) $(DESCRIPTION) ../../lib/*.so.*

clean:
	$(RM) responder responder-shared $(OBJ) *.zip

