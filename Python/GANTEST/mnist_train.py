# -*- coding: utf-8 -*-
import time
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import transforms
from model import discriminator, generator
import numpy as np
import matplotlib.pyplot as plt

start_time = time.time()
plt.rcParams['image.cmap'] = 'gray'



def show_images(images):
    sqrtn = int(np.ceil(np.sqrt(images.shape[0])))

    for index, image in enumerate(images):
        plt.subplot(sqrtn, sqrtn, index+1)
        plt.imshow(image.reshape(28, 28),cmap='gray')
# Discriminator Loss => BCELoss
def d_loss_function(inputs, targets):
    return nn.BCELoss()(inputs, targets)


def g_loss_function(inputs):
    
    targets = torch.ones([inputs.shape[0], 1])
    targets = targets.to(device)
    return nn.BCELoss()(inputs, targets)


# GPU
device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
print('GPU State:', device)

# Model
G = generator().to(device)
D = discriminator().to(device)
print(G)
print(D)

# Settings
epochs = 200
lr = 0.0002
batch_size = 64
g_optimizer = optim.Adam(G.parameters(), lr=lr, betas=(0.5, 0.999))
d_optimizer = optim.Adam(D.parameters(), lr=lr, betas=(0.5, 0.999))


# Transform
transform = transforms.Compose([transforms.ToTensor(),
                                transforms.Normalize((0.5,), (0.5,))])


# Load data
train_set = datasets.MNIST('D:\Vscode\data', train=True, download=True, transform=transform)
test_set = datasets.MNIST('D:\Vscode\data', train=False, download=True, transform=transform)
train_loader = DataLoader(train_set, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_set, batch_size=batch_size, shuffle=False)


# Train
for epoch in range(epochs):
    epoch += 1

    for times, data in enumerate(train_loader):
        times += 1
        real_inputs = data[0].to(device)
        test = 255 * (0.5 * real_inputs[0] + 0.5)
        # (0.5 * real_inputs[0] + 0.5) 的作用是將像素值範圍從 [-1,1] 映射到 [0,1]，因為GAN在生成圖像時，像素值的範圍是-1到1。
        # 所以，將像素值範圍從 [-1,1] 映射到 [0,1] 是為了方便後續將其映射到[0,255]，也就是一般圖像的像素值範圍。
        # 接下來，將 (0.5 * real_inputs[0] + 0.5) 所得到的數值乘上255，即可將像素值範圍從 [0,1] 映射到 [0,255]。
        # 因為255是圖像像素值的最大值，所以將其乘以255可以使像素值的範圍從 [0,1] 映射到 [0,255]，這是常見的圖像像素值範圍。
        # 最終，test 是一個 28x28 的圖像，其像素值範圍在 [0,255]。
        real_inputs = real_inputs.view(-1, 784)
        # 這行程式碼是將 real_inputs 張量中每個28x28的圖片張量展平為一個784維的向量。
        # 具體來說，real_inputs 是一個形狀為 (batch_size, 1, 28, 28) 的4D張量，其中 batch_size 是該批次中圖片的數量。
        # 使用 .view(-1, 784) 函式，它會將 real_inputs 重新調整形狀為 (batch_size, 784) 的2D張量。
        # 其中的 -1 參數表示自動計算，以確保該張量的大小不變。這樣做的目的是將2D張量傳遞到判別器(Discriminator)中。
        real_outputs = D(real_inputs)
        real_label = torch.ones(real_inputs.shape[0], 1).to(device)
        
        noise = (torch.rand(real_inputs.shape[0], 128) - 0.5) / 0.5
        noise = noise.to(device)
        fake_inputs = G(noise)
        fake_outputs = D(fake_inputs)
        fake_label = torch.zeros(fake_inputs.shape[0], 1).to(device)

        outputs = torch.cat((real_outputs, fake_outputs), 0)
        targets = torch.cat((real_label, fake_label), 0)

        # Zero the parameter gradients
        d_optimizer.zero_grad()

        # Backward propagation
        d_loss = d_loss_function(outputs, targets)
        d_loss.backward()
        d_optimizer.step()

        # Generator
        noise = (torch.rand(real_inputs.shape[0], 128)-0.5)/0.5
        noise = noise.to(device)

        fake_inputs = G(noise)
        fake_outputs = D(fake_inputs)

        g_loss = g_loss_function(fake_outputs)
        g_optimizer.zero_grad()
        g_loss.backward()
        # backeward負責算出哪個神經元的權重調整多少，而step負責實行
        g_optimizer.step()

        if times % 100 == 0 or times == len(train_loader):
            print('[{}/{}, {}/{}] D_loss: {:.3f} G_loss: {:.3f}'.format(epoch, epochs, times, len(train_loader), d_loss.item(), g_loss.item()))

    imgs_numpy = (fake_inputs.data.cpu().numpy()+1.0)/2.0
    # show_images(imgs_numpy[:16])
    # plt.show()

    if epoch % 50 == 0:
        torch.save(G, 'Generator_epoch_{}.pth'.format(epoch))
        print('Model saved.')


print('Training Finished.')
print('Cost Time: {}s'.format(time.time()-start_time))