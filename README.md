# 하모니카 런처

하모니카에서 사용하는 프로그램 런처

단축키 Ctrl+Space 키를 눌러 PC에 설치된 프로그램과 웹에서 바로 검색할 수 있습니다.

![screenshot](./albert.png)

* upstream : https://github.com/albertlauncher/albert.git


# Install

## Ubuntu, Linuxmint, Debian ...
```
wget -qO- https://pkg.hamonikr.org/add-hamonikr.apt | sudo -E bash -

sudo apt install albert
```

# Build from source
```
git clone --recursive https://github.com/hamonikr/hamonikr-albert.git
cmake -B build -S hamonikr-albert -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cmake --install build
```

# Support

* [HamoniKR Community](https://hamonikr.org/)

