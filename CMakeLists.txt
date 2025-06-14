cmake_minimum_required(VERSION 3.20)
project(FastAF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(FastAF
  srcs/main.cpp
  srcs/MarketDataClient.cpp
  srcs/ArbitrageScanner.cpp
  srcs/utils.cpp
)

target_include_directories(FastAF PRIVATE incs)

set(COMPILER_FLAGS
  -Wall -Wextra -Werror -pedantic -fomit-frame-pointer -fno-rtti -fno-exceptions -fstrict-aliasing -fno-math-errno -fno-stack-protector
  -march=native -funit-at-a-time -fexpensive-optimizations -fvect-cost-model=dynamic
  -freciprocal-math -fsingle-precision-constant -ffp-contract=fast
  -fversion-loops-for-strides -fivopts -fmove-loop-invariants -fmove-loop-stores -ftree-loop-ivcanon -ftree-loop-linear -floop-nest-optimize -floop-block -floop-strip-mine -floop-interchange -loop-distribution -funswitch-loops -fpeel-loops -ftree-loop-if-convert -ftree-loop-im -fsplit-ivs-in-unroller -ftree-loop-vectorize -fira-loop-pressure -fprefetch-loop-arrays -frerun-cse-after-loop #-faggressive-loop-optimizations -fsplit-loops -ffinite-loops -funroll-loops -floop-unroll-and-jam
  -fmodulo-sched -fmodulo-sched-allow-regmoves -freschedule-modulo-scheduled-loops
  -ftree-pta -ftree-copy-prop -ftree-forwprop -ftree-phiprop -ftree-scev-cprop
  -ftree-dce -ftree-builtin-call-dce -ftree-ccp -ftree-bit-ccp -ftree-dominator-opts -ftree-ch -ftree-coalesce-vars -ftree-sink -ftree-slsr -ftree-sra -ftree-pre -ftree-partial-pre -ftree-dse -ftree-vrp
  -fdce -fdse -fcse-follow-jumps -fcse-skip-blocks -fdelete-null-pointer-checks -fcrossjumping -fisolate-erroneous-paths-attribute
  -fgcse -fgcse-lm -fgcse-sm -fgcse-las -fgcse-after-reload -fweb -flive-range-shrinkage -fsplit-wide-types -fsplit-wide-types-early 
  -ftree-vectorize -ftree-slp-vectorize -ftree-switch-conversion -ftree-tail-merge -ftree-ter
  -fdevirtualize -fdevirtualize-speculatively -fdevirtualize-at-ltrans -fstdarg-opt
  -fauto-inc-dec -fmerge-constants -fif-conversion -fif-conversion2 -fcompare-elim -fcprop-registers -fforward-propagate -fcombine-stack-adjustments -fssa-backprop -fssa-phiopt
  -foptimize-sibling-calls -foptimize-strlen
  -fipa-profile -fipa-modref -fipa-pure-const -fipa-strict-aliasing -fipa-reference -fipa-reference-addressable -fipa-vrp -fipa-cp -fipa-bit-cp -fipa-sra -fipa-ra -fipa-pta -fipa-cp-clone # -fipa-icf
  -fschedule-insns -fschedule-insns2 -fsched-interblock -fsched-spec -fselective-scheduling -fselective-scheduling2 -fsel-sched-pipelining -fsel-sched-pipelining-outer-loops
  -fshrink-wrap -fshrink-wrap-separate
  -fpeephole2 -flra-remat -fallow-store-data-races -fstore-merging -fthread-jumps -fpredictive-commoning -fsplit-paths
  -freorder-blocks -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -fcode-hoisting -fhoist-adjacent-loads -fira-hoist-pressure
  -fearly-inlining -findirect-inlining -fpartial-inlining -finline-functions -finline-functions-called-once -finline-small-functions
  -falign-functions -falign-jumps -falign-labels -falign-loops
  -fcaller-saves -fdefer-pop -fguess-branch-probability
  -fno-plt -fuse-linker-plugin -flto -flto=jobserver
  -ffunction-sections -fdata-sections
)

target_compile_options(FastAF PRIVATE ${COMPILER_FLAGS})

target_link_options(FastAF PRIVATE "-Wl,--gc-sections")

set(LIBRARIES
  boost_system
  ssl
  crypto
  dl
  resolv
  jemalloc
)

target_link_libraries(FastAF PRIVATE ${LIBRARIES})