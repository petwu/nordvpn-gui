FROM ubuntu:20.10

# install  compiler, build tools, git, ...
RUN apt-get -qq update && \
    apt-get -qq install -y --no-install-recommends \
        build-essential \
        gcc-10 \
        libclang-11-dev \
        llvm-11-dev \
        clang-format \
        clang-tidy \
        cmake \
        ninja-build \
        qt5-default \
        qtdeclarative5-dev \
        qttools5-dev-tools \
        git \
        python3-pip \
        doxygen \
        openjdk-15-jre-headless \
        graphviz \
        librsvg2-bin \
        file \
        dpkg-dev \
        rpm \
        cloc && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# install conan, a C/C++ package manager
RUN pip3 install conan==1.32.0

# install include-what-you-use
# (iwyu does not provide prebuilt binaries and hence must be built from source)
RUN git clone https://github.com/include-what-you-use/include-what-you-use.git && \
    cd include-what-you-use && \
    git checkout clang_11 && \
    mkdir build && \
    cd build && \
    cmake -G Ninja .. && \
    ninja && \
    ninja install && \
    cd ../.. && \
    rm -rf include-what-you-use
