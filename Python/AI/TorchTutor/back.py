import torch
import torchvision
from torch import nn
from torch.nn import Conv2d, MaxPool2d, Flatten, Linear, Sequential
from torch.utils.data import DataLoader
from torch.nn import CrossEntropyLoss
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
loss = CrossEntropyLoss() # 定义loss函数
# 实例化这个网络
test = Network()
loss = CrossEntropyLoss() # 定义loss函数
# 构造优化器
# 这里我们选择的优化器是SGD 传入两个参数 第一个是个模型test的参数 第二个是学习率
optim = torch.optim.SGD(test.parameters(),lr=0.001)
for epoch in range(20):
    running_loss = 0.0
    for data in dataloader:
        imgs,targets = data
        outputs = test(imgs) # 输入图片
        result_loss = loss(outputs,targets) # 计算loss
        optim.zero_grad() #因为这是在循环里面 所以每次开始优化之前要把梯度置为0 防止上一次的结果影响这一次
        result_loss.backward() # 反向传播 求得梯度
        optim.step() # 对参数进行调优
        running_loss = running_loss + result_loss # 记录下这一轮中每个loss的值之和
    print(running_loss) # 打印每一轮的loss值之和————————————————
