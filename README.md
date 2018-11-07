## Tricks

### Linking error on OSX: `undefined reference to _aligned_alloc`

You need to patch the `vulkan.c` file from `dependencies/glfw/src/`:

```diff
  GLFWbool _glfwInitVulkan(void)
  {
      VkResult err;
      VkExtensionProperties* ep;
      uint32_t i, count;

  #if !defined(_GLFW_VULKAN_STATIC)
  #if defined(_GLFW_WIN32)
      const char* name = "vulkan-1.dll";
+ #elif defined(_GLFW_COCOA)
+     const char* name = "libvulkan.dylib";
  #else
      const char* name = "libvulkan.so.1";
  #endif
```

### Runtime error when running OSX bundle: `Library not loaded: @rpath/libvulkan.1.dylib`

You need to perform a few steps to solve this:

1. check the definition for the `libvulkan.1.dylib` and `libvulkan.1.1.85.dylib` (typically located in `$VULKAN_SDK/lib`): `otool -L $VULKAN_SDK/libvulkan.1.dylib`; the first line shows where OS will look for this file when running an executable;
2. if the path is `@rpath/libvulkan.1.dylib (compatibility version 1.0.0, current version 1.1.85)` then change it to be relative to the executable: `install_name_tool -id "@executable_path/libvulkan.1.dylib" $VULKAN_SDK/lib/libvulkan.1.dylib`
3. copy the modified libraries to your bundle

### Linking error on OSX (when building GLFW): `undefined symbol _aligned_alloc`

Modify the `dependencies/glfw-3.2.1/tests/vulkan.c` file:

```diff
  VKAPI_ATTR void *VKAPI_CALL myalloc(void *pUserData, size_t size,
                                      size_t alignment,
                                      VkSystemAllocationScope allocationScope) {
  #ifdef _MSC_VER
      return _aligned_malloc(size, alignment);
+ #elif defined(__APPLE__)
+     void *memptr;
+     posix_memalign(&memptr, alignment, size);
+     return memptr;
  #else
      return aligned_alloc(alignment, size);
  #endif
```

### How to bundle an application for OSX?

Create this file tree:

```
vulkan_tutorial.app                                     <--- NAME SHOULD BE SAME AS EXECUTABLE + '.app'
└── Contents
    ├── Frameworks
    │   └── libMoltenVK.dylib
    ├── MacOS
    │   ├── libvulkan.1.1.85.dylib
    │   ├── libvulkan.1.dylib -> libvulkan.1.1.85.dylib  <--- SYMBOLIC LINK
    │   └── vulkan_tutorial                              <--- THIS IS YOUR EXECUTABLE
    └── Resources
        └── vulkan
            └── icd.d
                └── MoltenVK_icd.json
```
