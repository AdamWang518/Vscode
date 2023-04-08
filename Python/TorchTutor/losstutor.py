import torch
import torchvision
from torch import nn
from torch.nn import Conv2d, MaxPool2d, Flatten, Linear, Sequential
from torch.utils.data import DataLoader
# 全局取消证书验证
import ssl
ssl._create_default_https_context = ssl._create_unverified_context

dataset = torchvision.datasets.CIFAR10("D:\Vscode\data",train=False,transform=torchvision.transforms.ToTensor(),
                                       download=True)
dataloader = DataLoader(dataset,batch_size=1) # 为了演示结果 我们把batchsize设置为1

class Network(nn.Module):
    def __init__(self):
        super(Network, self).__init__()
        self.model1 = Sequential(
            Conv2d(3, 32, 5, padding=2),# 卷积层
            MaxPool2d(2), # 池化层
            Conv2d(32, 32, 5, padding=2),
            MaxPool2d(2),
            Conv2d(32, 64, 5, padding=2),
            MaxPool2d(2),
            Flatten(),
            Linear(1024, 64),
            Linear(64, 10),
        )
    def forward(self,x):
        x = self.model1(x)
        return x
# 实例化这个网络
test = Network()
for data in dataloader:
    imgs,targets = data
    outputs = test(imgs) # 输入图片
    print(outputs) # 打印结果
    print(targets)
