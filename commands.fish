function build-buildenv
  docker build buildenv -t popcorn-buildenv
end

function run-kernel
  qemu-system-x86_64 -soundhw pcspk -cdrom dist/x86_64/kernel.iso
end

function build-kernel
  docker run --rm -v $PWD:/root/env popcorn-buildenv make build-x86_64
end

function build-and-run-kernel
  build-kernel && run-kernel
end

