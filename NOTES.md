# Building

## Windows

    ryb generate ninja --toolchain=vs@latest

## Mac

Until `ryb` is extended to support Mac.

    # We'll reuse the preprocessor to do depedency resolution...
    rm -r _build
    mkdir -p _build/unity
    find ./src -type f -name "*.cc" | sed 's/\(.*\)/#include "\1"/g' > _build/unity/yeti
    find ./runtime -type f -name "*.cc" | sed 's/\(.*\)/#include "\1"/g' > _build/unity/runtime
    find ./tools -type f -name "*.cc" | sed 's/\(.*\)/#include "\1"/g' > _build/unity/tools

# Running

## Mac

Unfortunately `DYLD_LIBRARY_PATH` is not respected when System Integrety Protection is enabled, as of El Capitan. So you'll need to copy dynamically linked dependencies into `_build/bin` prior to running Yeti et al.
