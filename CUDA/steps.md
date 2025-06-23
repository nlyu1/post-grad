# Setup

See [installation page](https://github.com/Infatoshi/cuda-course/blob/master/02_Setup/README.md). 

## Issue Resolution

The CUDA toolkit was already properly installed via package manager, but there was a PATH configuration issue.

**Problem**: PATH was pointing to `/usr/local/cuda-12.6/bin` but CUDA was installed in `/usr/local/cuda-12.5/`

**Solution**: Updated `.bashrc` to use `/usr/local/cuda/bin` (which is a symlink to the correct version)

## Status
- ✅ NVIDIA Driver: 555.42.06 (working)
- ✅ GPU: RTX 4060 Ti (detected)  
- ✅ CUDA Runtime: 12.5 (installed)
- ✅ CUDA Toolkit: 12.5 (installed)
- ✅ nvcc: Working (accessible via PATH)

## Verification
```bash
nvidia-smi    # Check GPU and driver
nvcc --version # Check CUDA compiler
```

1. Visit [cuda download](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=24.04&target_type=runfile_local). Download the 
2. 