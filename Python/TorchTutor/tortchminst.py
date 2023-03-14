#javascript:document.body.contentEditable='true';document.designMode='on'; void 0
import torch
from tensorboardX import SummaryWriter
import torchvision
from torch import nn
from torch.nn import Conv2d
from torch.utils.data import DataLoader
dataset=torchvision.datasets.MNIST('D:\Vscode\data',train=False,transform=torchvision.transforms.ToTensor())
dataloader=DataLoader(dataset,batch_size=64)
class network(nn.Module):
    def __init__(self):
        super(network,self).__init__()
        self.conv1=Conv2d(in_channels=1,out_channels=6,kernel_size=3,stride=1,padding=0)
    def forward(self,x):
        x=self.conv1(x)
        return x
test = network() # 实例化一个神经网络


step = 0
writer = SummaryWriter("./logs")
for data in dataloader: # 遍历dataloader 每次64个图像
    imgs,targets = data

    output = test(imgs) # 把数据送入神经网络 处理后给output
    writer.add_images("input",imgs,global_step=step) # 展示一下原始数据
    # 需要注意 我们设置的输出通道数是6 彩色图像通道数是3  这里直接显示会报错 他无法显示6个通道
    # 这里解释一下 本来是64张图片 6个通道 我们给他resize为3通道了 那么他是怎么处理的呢
    # 其实是把6个通道拆开 拆成两个3通道的 可以理解为一个6层的魔方 切开 变成两个三层的魔方（不严谨 理解意思即可）
    # 那么拆开 必然导致图片数量就多了 就不是64个了 这里写-1 代表自动计算 他会自动计算图片个数
    output = torch.reshape(output,(-1,1,26,26))
    writer.add_images("output", output, global_step=step)  # 展示一下输出数据
    step = step +1


