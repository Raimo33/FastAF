cmake_minimum_required(VERSION 3.20)
project(FastAF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(producer
  srcs/producer.cpp
  srcs/MarketDataClient.cpp
  srcs/utils.cpp
)

add_executable(consumer
  srcs/consumer.cpp
  srcs/ArbitrageScanner.cpp
  srcs/utils.cpp
)

target_include_directories(producer PRIVATE incs)
target_include_directories(consumer PRIVATE incs)

set(COMPILER_FLAGS
  -Wall -Wextra -Werror -pedantic
  -fomit-frame-pointer -fno-rtti -fno-exceptions -fstrict-aliasing -fno-math-errno -fno-stack-protector
  -march=native -funit-at-a-time -fexpensive-optimizations -fvect-cost-model=dynamic
  -freciprocal-math -fsingle-precision-constant -ffp-contract=fast
  -fno-plt -fuse-linker-plugin -flto -flto=jobserver
)

target_compile_options(producer PRIVATE ${COMPILER_FLAGS})
target_compile_options(consumer PRIVATE ${COMPILER_FLAGS})

# #cleanup
# CXXFLAGS += -ftree-pta -ftree-copy-prop -ftree-forwprop -ftree-phiprop -ftree-scev-cprop
# CXXFLAGS += -ftree-dce -ftree-builtin-call-dce -ftree-ccp -ftree-bit-ccp -ftree-dominator-opts -ftree-ch -ftree-coalesce-vars -ftree-sink -ftree-slsr -ftree-sra -ftree-pre -ftree-partial-pre -ftree-dse -ftree-vrp
# CXXFLAGS += -fgcse -fgcse-lm -fgcse-sm -fgcse-las -fgcse-after-reload -fweb -flive-range-shrinkage -fsplit-wide-types -fsplit-wide-types-early 
# CXXFLAGS += -fdce -fdse -fcse-follow-jumps -fcse-skip-blocks -fdelete-null-pointer-checks -fcrossjumping -fisolate-erroneous-paths-attribute
# #replacements
# CXXFLAGS += -ftree-vectorize -ftree-slp-vectorize -ftree-switch-conversion -ftree-tail-merge -ftree-ter
# CXXFLAGS += -fdevirtualize -fdevirtualize-speculatively -fdevirtualize-at-ltrans -fstdarg-opt
# CXXFLAGS += -fauto-inc-dec -fmerge-constants -fif-conversion -fif-conversion2 -fcompare-elim -fcprop-registers -fforward-propagate -fcombine-stack-adjustments -fssa-backprop -fssa-phiopt
# CXXFLAGS += -foptimize-sibling-calls -foptimize-strlen
# CXXFLAGS += -fipa-profile -fipa-modref -fipa-pure-const -fipa-strict-aliasing -fipa-reference -fipa-reference-addressable -fipa-vrp -fipa-cp -fipa-bit-cp -fipa-icf -fipa-sra -fipa-ra -fipa-pta -fipa-cp-clone
# #loops
# CXXFLAGS += -faggressive-loop-optimizations -ffinite-loops -fversion-loops-for-strides -fivopts -fmove-loop-invariants -fmove-loop-stores -ftree-loop-ivcanon -ftree-loop-linear -floop-nest-optimize -floop-block -floop-strip-mine -floop-interchange -loop-distribution -fsplit-loops -funswitch-loops -fpeel-loops -ftree-loop-if-convert -ftree-loop-im -fsplit-ivs-in-unroller -funroll-loops -floop-unroll-and-jam -ftree-loop-vectorize -fira-loop-pressure -fprefetch-loop-arrays -frerun-cse-after-loop
# CXXFLAGS += -fmodulo-sched -fmodulo-sched-allow-regmoves -freschedule-modulo-scheduled-loops
# #inlining
# CXXFLAGS += -fearly-inlining -findirect-inlining -fpartial-inlining -finline-functions -finline-functions-called-once -finline-small-functions
# #instruction reordering
# CXXFLAGS += -fschedule-insns -fschedule-insns2 -fsched-interblock -fsched-spec -fselective-scheduling -fselective-scheduling2 -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops
# CXXFLAGS += -fshrink-wrap -fshrink-wrap-separate
# CXXFLAGS += -fpeephole2 -flra-remat -fallow-store-data-races -fstore-merging -fthread-jumps -fpredictive-commoning -fsplit-paths
# CXXFLAGS += -freorder-blocks -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -fcode-hoisting -fhoist-adjacent-loads -fira-hoist-pressure
# CXXFLAGS += -falign-functions -falign-jumps -falign-labels -falign-loops
# CXXFLAGS += -fcaller-saves -fdefer-pop -fguess-branch-probability

target_link_options(producer PRIVATE -static)
target_link_options(consumer PRIVATE -static)

set(LIBRARIES
  ssl
  dl
  resolv
  crypto
  boost_system
  jemalloc
)

target_link_libraries(producer PRIVATE ${LIBRARIES})
# TODO link consumer libraries
