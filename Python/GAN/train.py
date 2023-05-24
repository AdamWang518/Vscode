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

def show_images(images):
    sqrtn = int(np.ceil(np.sqrt(images.shape[0])))

    for index, image in enumerate(images):
        plt.subplot(sqrtn, sqrtn, index+1)
        plt.imshow(image.reshape(28, 28),cmap='gray')
def g_loss(input):
    targets=torch.tensor()
    return nn.BCELoss(input,target)
    #計算Loss後要想辦法貼近目標，而generator的目標是輸出盡可能真實的產品
    #因此g_loss內部的target為1
def d_loss(input,target):

    return nn.BCELoss(input,target)
    #計算Loss後要想辦法貼近目標，而discriminator的目標與輸入的真偽一致
    #因此d_loss的建構子會有target