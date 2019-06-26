
Windows (cmake-gui)
~~~~~~~~~~~~~~~~~~~

1. Set the source code file to the root directory containing "CMakeLists.txt"
2. Binaries can be built in the directory of your choice, e.g. "OpenGL\visual\``
3. Click the configure button and select which generator you would like to use
4. Click the generate button
5. If your generator is an IDE such as Visual Studio, then open up the newly created .sln file and build ``ALL_BUILD``. After this you might want to set "main" as you StartUp Project.


Linux (Ubuntu)
  # Generate UNIX Makefile (point CMake to CMakeLists.txt)
  cmake ..

  # Execute make command
  make

  # Run executable
  ./bin/main
