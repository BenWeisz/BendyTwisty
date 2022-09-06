# ModelEngine

Basic OpenGL Engine for loading up models

## Demos

https://user-images.githubusercontent.com/15256744/186548807-a67006e7-4386-43ac-b1b4-cf9537abb265.mov

https://user-images.githubusercontent.com/15256744/187484764-42a4a681-e387-4140-a74c-5a9f1ad9d7ef.mov

## M1 Mac Setup

1. Download, build, install glfw3 using [Guide](http://carette.xyz/posts/opengl_and_cpp_on_m1_mac/)
2. Make a build directory in the top level folder
3. Run `cmake ..` in build directory
4. Run `make`
5. Voila!

For M1 Macs you may need to run `cmake -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 ..` instead
