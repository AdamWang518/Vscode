import torch
import torchvision
vgg16=torchvision.models.vgg16(pretrained=False)
print(vgg16)
torch.save(vgg16,"vgg16_test.pth")
torch.save(vgg16.state_dict(),"vgg16_test.pth")