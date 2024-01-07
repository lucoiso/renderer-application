[![renderer-application CMake Workflow](https://github.com/lucoiso/renderer-application/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/lucoiso/renderer-application/actions/workflows/cmake-build.yml)

![image](https://github.com/lucoiso/renderer-application/assets/77353979/95fc9824-0630-4f06-beb2-94a6f80b9afe)

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
