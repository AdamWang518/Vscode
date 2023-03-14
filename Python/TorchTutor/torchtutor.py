import torch
import torchvision
from torch import nn
from tensorboardX import SummaryWriter
from torch.nn import Conv2d
from torch.nn import MaxPool2d
from torch.utils.data import DataLoader
# 准备数据集
dataset = torchvision.datasets.CIFAR10("D:\Vscode\data",train=False,transform=torchvision.transforms.ToTensor(),download=False)
# 设置dataloader
dataloader = DataLoader(dataset,batch_size=64)

# 定义神经网络
class network(nn.Module):
    def __init__(self):
        super(network, self).__init__()
        self.conv1=Conv2d(in_channels=3,out_channels=3,kernel_size=3,stride=1,padding=0)
        self.maxpool = MaxPool2d(kernel_size=3) # 池化操作
    def forward(self,x):
        x = self.conv1(x)
        x = self.maxpool(x) # 执行池化操作
        return x

test_network = network() # 实例化
writer = SummaryWriter("D:\Vscode\logs")
step = 0
for data in dataloader:
    imgs,targets = data
    writer.add_images("input",imgs,global_step=step) # 显示下输入数据
    output = test_network(imgs) # 把图片输入进神经网络
    writer.add_images("output", output, global_step=step)  # 展示一下输出数据
    step =step + 1
