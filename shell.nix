{ pkgs ? import <nixpkgs> {} }:
let
  mingw = pkgs.pkgsCross.mingwW64;
  mcfgthread = mingw.windows.mcfgthreads;
  sdl3-mingw = pkgs.fetchzip {
    url = "https://github.com/libsdl-org/SDL/releases/download/release-3.4.8/SDL3-devel-3.4.8-mingw.tar.gz";
    sha256 = "sha256-kIG/0jpYC19zzj//lGP/KMFJh37kX7X+yTgapPcqoh8=";
  };
  volk = pkgs.fetchFromGitHub {
    owner = "zeux";
    repo = "volk";
    rev = "1.4.350";
    sha256 = "sha256:1zx1rxs0rmfdzb9l5a1dkxh61pj2qxwq5mf4gs4ig7qk2dd0x6zc";
  };
  gccVersion = pkgs.lib.getVersion pkgs.gcc.cc;
  libstdcxxInc = "${pkgs.gcc.cc}/include/c++/${gccVersion}";
in pkgs.mkShell {
  packages = [
    mingw.buildPackages.gcc
    mingw.buildPackages.binutils
    pkgs.cmake
    pkgs.ninja
    pkgs.vulkan-headers
    pkgs.vulkan-validation-layers
    pkgs.glslang
    pkgs.sdl3
    pkgs.glm
  ];
  shellHook = ''
    export CC="x86_64-w64-mingw32-gcc"
    export CXX="x86_64-w64-mingw32-g++"
    export VULKAN_HEADERS=$(nix-build '<nixpkgs>' -A vulkan-headers --no-out-link)/include
    export SDL3_PREFIX="${sdl3-mingw}/x86_64-w64-mingw32"
    export VOLK_PREFIX="${volk}"
    export MCFGTHREAD_PREFIX="${mcfgthread}"
    export GLM_PREFIX="${pkgs.glm}"

    SPECS_FILE=$(mktemp)
    $CXX -dumpspecs | sed 's/-lmcfgthread/-Bstatic -lmcfgthread -Bdynamic/' > "$SPECS_FILE"

    function build() {
      local file="$1"
      local opt_flags="-O0"
      if [[ "$2" == "-release" ]]; then
        opt_flags="-O3 -DNDEBUG"
      fi

      mkdir -p bin
      $CXX *.cpp -o "bin/$file.exe" -std=c++23 \
        $opt_flags \
        -specs="$SPECS_FILE" \
        -DVK_USE_PLATFORM_WIN32_KHR \
        -DVK_NO_PROTOTYPES \
        -I$VULKAN_HEADERS \
        -I$SDL3_PREFIX/include \
        -I$VOLK_PREFIX \
        -L$SDL3_PREFIX/lib \
        -L$MCFGTHREAD_PREFIX/lib \
        -I$GLM_PREFIX/include \
        -lSDL3.dll \
        -static-libgcc -static-libstdc++
    } 
    export -f build

    function compile_shaders() {
      rm bin/shaders/*
      for shader in shaders/*; do
        if [ -f "$shader" ]; then
          filename=$(basename "$shader")
          ext=''${filename##*.}
          glslang -V "$shader" -o "bin/shaders/$ext.spv"
        fi
      done
    }
    export -f compile_shaders

    cat > compile_flags.txt << EOF

-I${pkgs.sdl3.dev}/include
-I$GLM_PREFIX/include
-I$VOLK_PREFIX
-std=c++23
-isystem${libstdcxxInc}
-isystem${libstdcxxInc}/x86_64-unknown-linux-gnu
EOF

    cat > .clangd << EOF
Diagnostics:
  UnusedIncludes: None
  MissingIncludes: None
EOF

    rm -rf ~/.cache/clangd

    echo "Cross compiler ready: $CXX"
    echo "compile_flags.txt written"
  '';
}
