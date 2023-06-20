import torchvision
dataset = torchvision.datasets.CIFAR10("D:\Vscode\data",train=False,download=False)
img,target = dataset[0]
print(dataset.classes[target])
img.show()
