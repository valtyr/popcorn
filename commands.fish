function build-buildenv
  docker build buildenv -t popcorn-buildenv
end

function clean-artifacts
  rm -rf build/**/*.o
end

function run-kernel
  qemu-system-i386 -serial stdio -cdrom dist/x86_64/kernel.iso
end

function build-kernel
  docker run --rm -v $PWD:/root/env popcorn-buildenv make build-x86_64
end

function generate-compiledb
  docker run --rm -v $PWD:/root/env popcorn-buildenv make -Bnwk build-x86_64 | compiledb -o- > compile_commands.json
end

function build-and-run-kernel
  build-kernel && run-kernel
end

