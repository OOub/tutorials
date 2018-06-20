![tutorials](banner.png "The Tutorials banner")

Tutorials provides step-by-step guides to learn how to use the [Sepia](https://github.com/neuromorphic-paris/sepia), [Tarsier](https://github.com/neuromorphic-paris/tarsier) and [Chameleon](https://github.com/neuromorphic-paris/chameleon) libraries.

The tutorials are held in the [wiki](https://github.com/neuromorphic-paris/tutorials/wiki). This repository contains the result files produced during the tutorials.

# Install

To download Tutorials, run the command:
```sh
git clone --recursive https://github.com/neuromorphic-paris/tutorials.git
```

#

```sh
for file in basics/*/source/*.cpp; do clang-format -i $file; done;
for file in examples/*/source/*.hpp; do clang-format -i $file; done;
for file in examples/*/source/*.cpp; do clang-format -i $file; done;
```

# License

See the [LICENSE](LICENSE.txt) file for license rights and limitations (GNU GPLv3).
