import torch
import torch.nn as nn
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader

# 定義神經網絡結構
class Discriminator(nn.Module):
    def __init__(self):
        super(Discriminator, self).__init__()
        self.model = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 128),  # 全連接層，輸入維度為784，輸出維度為128
            nn.ReLU(),            # ReLU 激活函數
            nn.Linear(128, 64),   # 全連接層，輸入維度為128，輸出維度為64
            nn.ReLU(),            # ReLU 激活函數
            nn.Linear(64, 10)     # 全連接層，輸入維度為64，輸出維度為10（分類問題通常使用softmax函數作為最後一層）
        )

    def forward(self, x):
        return self.model(x)

# 載入MNIST數據集
trainset = torchvision.datasets.MNIST(
    root='./',
    train=True,
    transform=torchvision.transforms.ToTensor(),
    download=False
)
trainloader = DataLoader(dataset=trainset, batch_size=64, shuffle=True)

device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
# 決定運算的位置

# 創建模型實例
model = Discriminator().to(device)

# 定義損失函數和優化器
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=0.1)

# 訓練模型
total_epochs = 10
for epoch in range(total_epochs):
    running_loss = 0.0
    for i, (images, labels) in enumerate(trainloader):
        images = images.to(device)
        labels = labels.to(device)
        
        optimizer.zero_grad()
        
        # 前向傳播
        outputs = model(images)
        loss = criterion(outputs, labels)
        
        # 反向傳播和參數更新
        loss.backward()
        optimizer.step()
        
        running_loss += loss.item()
        
    print(f"Epoch [{epoch+1}/{total_epochs}], Loss: {running_loss/len(trainloader)}")
    if (epoch + 1) % 5 == 0:
        torch.save(model, f'Discriminator_epoch_{epoch}.pth')
        print('Model saved.')