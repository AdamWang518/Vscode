# install pycuda windows binary
# https://www.lfd.uci.edu/~gohlke/pythonlibs/?cm_mc_uid=08085305845514542921829&cm_mc_sid_50200000=1456395916#pycuda
# pip install "pycuda-2020.1+cuda101-cp38-cp38-win_amd64.whl"

import pycuda
import pycuda.driver as drv
drv.init()

print(f'偵測 {drv.Device.count()} 個CUDA裝置 \n')

for i in range(drv.Device.count()):
    gpu_device = drv.Device(i)
    print(f'裝置 {i}: {gpu_device.name()}') 
    compute_capability = float(str(gpu_device.compute_capability()[0])+'.'+ \
                        str(gpu_device.compute_capability()[1]))
    print(f'\t 計算能力: {compute_capability}')
    print(f'\t GPU記憶體: {gpu_device.total_memory()//(1024**2)} MB')
    
    # 裝置其他屬性
    device_attributes_tuples = iter(gpu_device.get_attributes().items()) 
    device_attributes = {}
    
    for k, v in device_attributes_tuples:
        device_attributes[str(k)] = v
    
    num_mp = device_attributes['MULTIPROCESSOR_COUNT']
    
    # GPU 核心數
    # http://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#compute-capabilities
    cuda_cores_per_mp = { 5.0 : 128, 5.1 : 128, 5.2 : 128, 6.0 : 64, \
                          6.1 : 128, 6.2 : 128}[compute_capability]
    
    print(f'\t {num_mp} 個處理器, 各有 {cuda_cores_per_mp} 個CUDA核心數, '+ \
            f'共 {num_mp*cuda_cores_per_mp} 個CUDA核心數\n')
    
    device_attributes.pop('MULTIPROCESSOR_COUNT')
    
    for k in list(device_attributes.keys()):
        print(f'\t {k}: {device_attributes[k]}')
