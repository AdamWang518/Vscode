import torch
from torchvision import models
from torchvision import transforms

import json
from PIL import Image
resnet = models.resnet101(pretrained=True, progress=True)
preprocess = transforms.Compose([
    transforms.Resize(256),
    transforms.CenterCrop(224),
    transforms.ToTensor(),
    transforms.Normalize(
        mean=[0.485, 0.456, 0.406],
        std=[0.229, 0.224, 0.225]
    )
])

img1 = Image.open("D:\\Vscode\\Python\\Pytorch\\dog.jpg")
img2 = Image.open("D:\\Vscode\\Python\\Pytorch\\cat.jpg")
img3 = Image.open("D:\\Vscode\\Python\\Pytorch\\plane.jpg")
img1 = preprocess(img1)
img2 = preprocess(img2)
img3 = preprocess(img3)
resnet.eval()
inp_batch = torch.stack([img1, img2, img3])
out_batch = resnet(inp_batch)
torch.Size([3, 1000])

with open("D:\\Vscode\\Python\\Pytorch\\imagenet-class.txt", 'r') as f:
    labels = [line.strip() for line in f.readlines()]

_, index = torch.max(out_batch, dim=1)
for idx in index:
    print(labels[idx.item()])