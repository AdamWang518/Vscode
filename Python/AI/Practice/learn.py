import numpy as np
import torch
import torch.nn as nn
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import time
np.random.seed(1234)
# 这里用的是torchvision已经封装好的MINST数据集
trainset = torchvision.datasets.MNIST(
    root='MNIST',  # root是下载MNIST数据集保存的路径，可以自行修改
    train=True,
    transform=torchvision.transforms.ToTensor(),
    download=True
)

testset = torchvision.datasets.MNIST(
    root='MNIST',
    train=False,
    transform=torchvision.transforms.ToTensor(),
    download=True
)

# DataLoader是一个很好地能够帮助整理数据集的类，可以用来分批次，打乱以及多线程等操作
trainloader = DataLoader(dataset=trainset, batch_size=100, shuffle=True)
testloader = DataLoader(dataset=testset, batch_size=100, shuffle=True)
# 可视化某一批数据
# iter迭代器，可以用来便利trainloader里面每一个数据，这里只迭代一次来进行可视化
train_img, train_label = next(iter(trainloader))
fig, axes = plt.subplots(10, 10, figsize=(10, 10))
axes_list = []
# 输入到网络的图像
for i in range(axes.shape[0]):
    for j in range(axes.shape[1]):
        # 这里画出来的就是我们想输入到网络里训练的图像，与之对应的标签用来进行最后分类结果损失函数的计算
        axes[i, j].imshow(train_img[i*10+j, 0, :, :], cmap="gray")
        axes[i, j].axis("off")
# 对应的标签
print(train_label)
# 卷积模块，由卷积核和激活函数组成


class conv_block(nn.Module):
    def __init__(self, ks, ch_in, ch_out):
        super(conv_block, self).__init__()
        self.conv = nn.Sequential(
            nn.Conv2d(ch_in, ch_out, kernel_size=ks, stride=1,
                      padding=1, bias=True),  # 二维卷积核，用于提取局部的图像信息
            nn.ReLU(inplace=True),  # 这里用ReLU作为激活函数
            nn.Conv2d(ch_out, ch_out, kernel_size=ks,
                      stride=1, padding=1, bias=True),
            nn.ReLU(inplace=True),
        )

    def forward(self, x):
        return self.conv(x)
# 常规CNN模块（由几个卷积模块堆叠而成）


class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()

        # 建立類神經網路各層
        self.flatten = nn.Flatten()  # 轉為一維向量
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),   # 線性轉換
            nn.ReLU(),               # ReLU 轉換
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 10)
        )

    def forward(self, x):
        # 定義資料如何通過類神經網路各層
        x = self.flatten(x)
        logits = self.linear_relu_stack(x)
        return logits


# #网络参数定义
device = torch.device("cuda:0")  # 此处根据电脑配置进行选择，如果没有cuda就用cpu
# #device = torch.device("cpu")
net = CNN().to(device=device, dtype=torch.float32)
epochs = 50  # 训练轮次
optimizer = torch.optim.Adam(
    net.parameters(), lr=1e-4, weight_decay=1e-8)  # 使用Adam优化器
criterion = nn.CrossEntropyLoss()  # 分类任务常用的交叉熵损失函数
train_loss = []
# Begin training
MinTrainLoss = 999
for epoch in range(1, epochs+1):
    total_train_loss = []
    net.train()
    start = time.time()
    for input_img, label in trainloader:
        # 我们同样地，需要将我们取出来的训练集数据进行torch能够运算的格式转换
        input_img = input_img.to(device=device, dtype=torch.float32)
        # 输入和输出的格式都保持一致才能进行运算
        label = label.to(device=device, dtype=torch.float32)
        optimizer.zero_grad()  # 每一次算loss前需要将之前的梯度清零，这样才不会影响后面的更新
        pred_img = net(input_img)
        loss = criterion(pred_img, label.long())
        loss.backward()
        optimizer.step()
        total_train_loss.append(loss.item())
    # 将一个minibatch里面的损失取平均作为这一轮的loss
    train_loss.append(np.mean(total_train_loss))
    end = time.time()
    # 打印当前的loss
    print("epochs[%3d/%3d] current loss: %.5f, time: %.3f" %
          (epoch, epochs, train_loss[-1], (end-start)))  # 打印每一轮训练的结果

    if train_loss[-1] < MinTrainLoss:
        torch.save(net.state_dict(), "./model_min_train.pth")  # 保存loss最小的模型
        MinTrainLoss = train_loss[-1]
# 测试机某一批数据
test_img, test_label = next(iter(testloader))
fig, axes = plt.subplots(10, 10, figsize=(10, 10))
axes_list = []
# 输入到网络的图像
for i in range(axes.shape[0]):
    for j in range(axes.shape[1]):
        axes[i, j].imshow(test_img[i*10+j, 0, :, :], cmap="gray")
        axes[i, j].axis("off")
# 预测我拿出来的那一批数据进行展示
cnn = CNN().to(device=device, dtype=torch.float32)
cnn.load_state_dict(torch.load("./model_min_train.pth",
                    map_location=device))  # 导入我们之前已经训练好的模型
cnn.eval()  # 评估模式

test_img = test_img.to(device=device, dtype=torch.float32)
test_label = test_label.to(device=device, dtype=torch.float32)

pred_test = cnn(test_img)  # 记住，输出的结果是一个长度为10的tensor
# 所以我们需要对其进行最大值对应索引的处理，从而得到我们想要的预测结果
test_pred = np.argmax(pred_test.cpu().data.numpy(), axis=1)

# 预测结果以及标签
print("预测结果")
print(test_pred)
print("标签")
print(test_label.cpu().data.numpy())
