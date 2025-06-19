#!/usr/bin/env python3

import os
import argparse
from pandare import blocking, Panda
from pathlib import Path
import subprocess
import shutil


def prepare_cdrom_iso(build_target):
    thisdir = Path(__file__).parent    
    cdrom_dir = thisdir / "cdrom"
    taint_unit_test_dir = thisdir.parent.parent / "taint_unit_test"

    # Create the cdrom directory
    cdrom_dir.mkdir(parents=True, exist_ok=True)

    # Build the target in taint_unit_test
    subprocess.run(
        ["make"],
        env={"TARGET": build_target, **os.environ},
        cwd=taint_unit_test_dir,
        check=True
    )

    # Copy binaries from taint_unit_test/bin to cdrom
    bin_dir = taint_unit_test_dir / "bin"
    for binary in bin_dir.iterdir():
        if binary.is_file():
            shutil.copy(binary, cdrom_dir)

    # Copy the run_all_tests.sh script to cdrom
    run_all_tests_script = thisdir / "run_all_tests.sh"
    shutil.copy(run_all_tests_script, cdrom_dir)


@blocking
def run_in_guest_record():
    panda.record_cmd("./cdrom/run_all_tests.sh",
        copy_directory=os.path.join(Path(__file__).parent, "cdrom"),
        recording_name="taint2_tests",
        snap_name="root"
    )
    panda.stop_run()

if __name__ == "__main__":
    arg_parser = argparse.ArgumentParser(description="Create or replay a recording for taint2 unit tests")
    arg_parser.add_argument(
            '--arch',
            type=str,
            choices=["i386", "x86_64", "arm", "aarch64"],
            help="Architecture to test, one of: i386, x86_64, arm, aarch64",
            default="i386",
    )
    arg_parser.add_argument(
            '--mode',
            type=str,
            choices=["record", "replay"],
            help="Mode, one of: record, replay",
            default="record",
    )
    args = arg_parser.parse_args()

    # Arch selection
    if args.arch == "i386":
        build_target = "TARGET_I386"
    elif args.arch == "x86_64":
        build_target = "TARGET_X86_64"
    elif args.arch == "arm":
        build_target = "TARGET_ARM"
    elif args.arch == "aarch64":
        build_target = "TARGET_AARCH64"
    
    panda = Panda(generic=args.arch)

    # Mode selection
    if args.mode == "record":
        prepare_cdrom_iso(build_target)
        panda.queue_async(run_in_guest_record)
        panda.run()
    elif args.mode == "replay":
        panda.load_plugin("taint2", 
        args={
            "enable_hypercalls": True,
            "debug" : True
        })
        panda.run_replay("taint2_tests")
