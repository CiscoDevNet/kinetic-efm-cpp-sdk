CFLAGS = -std=c++11 -Wall -Wextra -I ../../include -g -O2 -D_FORTIFY_SOURCE=2 -fPIE -fstack-protector
LDFLAGS = -L ../../lib -pie -Wl,-z,now
# -latomic is only required on some platforms
LIBS = -lboost_log -lboost_date_time -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lssl -lcrypto -ldl -lpthread -latomic

LINK_NAME=requester
VERSION=1.2.3
DESCRIPTION="Requester Link Example"

.PHONY: all clean
all: requester requester-shared

DEPS = requester_link.h
OBJ = main.o

%.o: %.cpp $(DEPS)
	$(CXX) -DLINK_VERSION="\"$(VERSION)\"" -c -o $@ $< $(CFLAGS)

requester: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp-static $(LIBS)

requester-shared: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp $(LIBS)

run: requester
	./requester

run-shared: requester-shared
	LD_LIBRARY_PATH=../../lib ./requester-shared

package: requester
	../../packaging/package_link.sh $(LINK_NAME) ./requester $(VERSION) $(DESCRIPTION)

package-shared: requester-shared
	../../packaging/package_link.sh $(LINK_NAME) ./requester-shared $(VERSION) $(DESCRIPTION) ../../lib/*.so.*

clean:
	$(RM) requester requester-shared $(OBJ) *.zip
