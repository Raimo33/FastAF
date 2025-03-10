#https://github.com/clearlinux/clr-bundles
FROM clearlinux:latest 

RUN swupd update
RUN swupd bundle-add devpkg-openssl devpkg-zlib devpkg-yyjson devpkg-xxhash valgrind
#TODO xxhash (Xxhash-os-clr-on-clr-dev)

COPY ./build /build

RUN useradd -s /bin/bash fastaf
RUN chown -R fastaf:fastaf /build

WORKDIR /build
RUN cmake . && cmake --build . --parallel $(nproc)

# TODO:
# disable_interrupt_coalescing
# disable_gro_lro
# disable RSS
# Set Low-Latency IRQ Affinity

CMD ["./fastaf"]