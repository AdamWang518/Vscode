import torch
import torch.nn as nn
import torchvision
import matplotlib.pyplot as plt
import torch.utils.data as Data
from torch.autograd import Variable
from model import discriminator
from torch.utils.data import DataLoader
import numpy as np
device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
testset = torchvision.datasets.CIFAR10(
    root='./',
    train=False,
    transform=torchvision.transforms.ToTensor(),
    download=True  # 如果你还没有下载数据集，把这里设为True
)
testloader = DataLoader(dataset=testset, batch_size=64, shuffle=True)

test_img, test_label = next(iter(testloader))
# CIFAR10的图片是32x32的，所以我把subplot的数量减少了
fig, axes = plt.subplots(8, 8, figsize=(8, 8))
axes_list = []
# 输入到网络的图像
for i in range(axes.shape[0]):
    for j in range(axes.shape[1]):
        # 因为CIFAR10是彩色图像，所以我们要做转置处理
        axes[i, j].imshow(np.transpose(test_img[i*8+j], (1, 2, 0)))
        axes[i, j].axis("off")
# 预测我拿出来的那一批数据进行展示
cnn = discriminator().to(device=device, dtype=torch.float32)
# 导入我们之前已经训练好的模型
cnn = torch.load("D:\\Vscode\\Python\\AI\\CIFAR\\Discriminator_epoch_49.pth")
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
