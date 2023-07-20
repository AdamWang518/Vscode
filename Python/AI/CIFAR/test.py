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

# loading the CIFAR10 dataset
testset = torchvision.datasets.CIFAR10(
    root='./',
    train=False,
    transform=torchvision.transforms.ToTensor(),
    download=True  # if you have not downloaded the dataset yet, set this to True
)
testloader = DataLoader(dataset=testset, batch_size=64, shuffle=True)

test_img, test_label = next(iter(testloader))
cnn = discriminator().to(device=device, dtype=torch.float32)
cnn = torch.load("D:\\Vscode\\Python\\AI\\CIFAR\\Discriminator_epoch_49.pth")
cnn.eval()  # evaluation mode

test_img = test_img.to(device=device, dtype=torch.float32)
test_label = test_label.to(device=device, dtype=torch.float32)

pred_test = cnn(test_img)  # remember, the output is a tensor of length 10
test_pred = np.argmax(pred_test.cpu().data.numpy(), axis=1)

fig, axes = plt.subplots(8, 8, figsize=(8, 8))

# the list of classes in the CIFAR10 dataset
classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

# input image to the network
for i in range(axes.shape[0]):
    for j in range(axes.shape[1]):
        # because CIFAR10 is a color image, we have to transpose it
        axes[i, j].imshow(np.transpose(test_img[i*8+j].cpu(), (1, 2, 0)))
        axes[i, j].set_title("Pred: {}\nTrue: {}".format(classes[test_pred[i*8+j]], classes[int(test_label[i*8+j])]))
        axes[i, j].axis("off")

plt.tight_layout()
plt.show()

# print the prediction result and the label
print("Prediction Result")
print(test_pred)
print("Label")
print(test_label.cpu().data.numpy())
