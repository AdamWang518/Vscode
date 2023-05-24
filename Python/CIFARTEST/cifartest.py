import torch
import netstruct
import torchvision
from torch.utils.data import random_split
from torch.utils.data import DataLoader
from torch.nn import CrossEntropyLoss
dataset = torchvision.datasets.CIFAR10("D:\Vscode\data",train=False,transform=torchvision.transforms.ToTensor(),download=False)
num_samples = len(dataset)
train_ratio = 0.8  # 训练集比例
val_ratio = 0.1    # 验证集比例
test_ratio = 0.1   # 测试集比例

train_size = int(num_samples * train_ratio)
val_size = int(num_samples * val_ratio)
test_size = num_samples - train_size - val_size
train_dataset, val_dataset, test_dataset = random_split(dataset, [train_size, val_size, test_size])
train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
val_loader = DataLoader(val_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=64, shuffle=True)
network=netstruct.Network()
# 实例化这个网络
loss = CrossEntropyLoss() # 定义loss函数
# 构造优化器
# 这里我们选择的优化器是SGD 传入两个参数 第一个是个模型test的参数 第二个是学习率
optim = torch.optim.SGD(network.parameters(),lr=0.001)
running_loss = 0.0
for data in train_loader:
    imgs,targets = data
    outputs = network(imgs) # 输入图片
    result_loss = loss(outputs,targets) # 计算loss
    optim.zero_grad() #因为这是在循环里面 所以每次开始优化之前要把梯度置为0 防止上一次的结果影响这一次
    result_loss.backward() # 反向传播 求得梯度
    optim.step() # 对参数进行调优
    running_loss = running_loss + result_loss # 记录下这一轮中每个loss的值之和
print(running_loss) # 打印每一轮的loss值之和————————————————
print('done')