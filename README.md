[![renderer-application CMake Workflow](https://github.com/lucoiso/renderer-application/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/lucoiso/renderer-application/actions/workflows/cmake-build.yml)

![image](https://github.com/lucoiso/renderer-application/assets/77353979/2ba86177-6e79-4dc6-af48-f48c43bef5f1)

# Dependencies

1. Vcpkg (w/ ENVIRONMENT VARIABLE VCPKG_ROOT set to the vcpkg root directory)
2. CMake v3.28 (Officially supporting C++20 Modules)
3. Vulkan SDK v1.3.261 (Does not need any modules except the SDK core)
4. Git

# Setup

- Run these commands:

```
git clone https://github.com/lucoiso/renderer-application.git
cd renderer-application
git submodule update --init --recursive
```
