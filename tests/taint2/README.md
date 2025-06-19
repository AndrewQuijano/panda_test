# Taint2 Testing

Ideally run the following lines of code to download the generic QCows before running the script
```bash
python3 -c "from pandare.qcows_internal import Qcows;Qcows.get_qcow('x86_64')"
python3 -c "from pandare.qcows_internal import Qcows;Qcows.get_qcow('i386')"
```

The best way to debug this locally would be to install Panda locally as follows, from the root of the PANDA repository:
```bash
mkdir build
cd build
../configure \
    --target-list="x86_64-softmmu,i386-softmmu" \
    --enable-debug-info \
    --enable-llvm
cd ..
make -C ./build -j "$(nproc)"
```

Then just run `python3 taint2_multi_arch_record_or_reply.py --arch <arch> --mode <mode>`. You can run with GDB as the debug symbols are enabled on the binaries in the `panda/build` directory.

To clean up everything in taint2 folder, run the following:
```bash
rm -rf cdrom* taint2_tests*
```