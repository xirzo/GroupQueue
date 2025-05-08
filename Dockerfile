FROM alpine:3.17.0 AS build

RUN apk update && \
    apk add --no-cache \
        build-base=0.5-r3 \
    	nlohmann-json \
    	zlib-dev \
    	openssl-dev \
    	asio-dev \
	git \
        cmake=3.24.4-r0

RUN git clone https://github.com/SRombauts/SQLiteCpp.git /tmp/SQLiteCpp && \
    	mkdir -p /tmp/SQLiteCpp/build && \
	cd /tmp/SQLiteCpp/build && \
    	cmake -DSQLITECPP_BUILD_EXAMPLES=OFF -DSQLITECPP_BUILD_TESTS=OFF .. && \
    	cmake --build . && \
    	make install

RUN git clone https://github.com/CrowCpp/Crow /tmp/Crow && \
	mkdir -p /tmp/Crow/build && \
	cd /tmp/Crow/build && \
	cmake -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF .. && \
    	cmake --build . && \
	make install

WORKDIR /group-queue
COPY bin/ ./bin/
COPY lib/ ./lib/
COPY *.json ./
COPY *.db3 ./
COPY CMakeLists.txt .

WORKDIR /group-queue/build
RUN cmake .. && \
    cmake --build .

FROM alpine:3.17.0

RUN apk add --no-cache libstdc++ libgcc

RUN addgroup -S group-queue && \
    adduser -S group-queue -G group-queue

USER group-queue

COPY --chown=group-queue:group-queue --from=build \
    ./group-queue/build/bin/group_queue \
    ./app/

COPY --chown=group-queue:group-queue --from=build \
    ./group-queue/*.json \
    ./

COPY --chown=group-queue:group-queue --from=build \
    ./group-queue/*.db3 \
    ./

ENTRYPOINT ["./app/group_queue"]
