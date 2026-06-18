This is a little test project of mine using Vulkan. I'm following vulkan-tutorial.com currently, but I'm planning on extending this into a full engine for personal use.

To build:
```
nix-shell shell.nix
compile_shaders
build main [-release]
```
Defaults to including debug symbols, `-release` swaps to O3 optimisation and disables validation layers.
