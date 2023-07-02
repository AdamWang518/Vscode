import torch
import torchvision.transforms as transforms
import torch.nn as nn
from PIL import Image


class Discriminator(nn.Module):
    def __init__(self):
        super(Discriminator, self).__init__()
        self.model = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 128),  # 全連接層，輸入維度為784，輸出維度為128
            nn.ReLU(),            # ReLU 激活函數
            nn.Linear(128, 64),   # 全連接層，輸入維度為128，輸出維度為64
            nn.ReLU(),            # ReLU 激活函數
            # 全連接層，輸入維度為64，輸出維度為10（分類問題通常使用softmax函數作為最後一層）
            nn.Linear(64, 10)
        )

    def forward(self, x):
        return self.model(x)


# 讀取圖片
image_path = 'D:\\Vscode\\Python\\AI\\Practice\\4.png'  # 指定圖片路徑
image = Image.open(image_path).convert('L')  # 讀取圖片並轉換為灰階

# 將圖片進行預處理
transform = transforms.Compose([transforms.Resize((28, 28)),  # 圖片大小調整為28x28，這是因為模型訓練時使用的圖片大小為28x28
                                transforms.ToTensor()])  # 將圖片轉換為Tensor
image = transform(image)

# 增加一個維度以匹配模型輸入
image = image.unsqueeze(0)  # 增加批次維度，使得圖片可以被模型接受

device = torch.device("cuda:0" if torch.cuda.is_available()
                      else "cpu")  # 檢查是否有可用的 GPU，如果無則使用 CPU

# 載入模型
cnn = Discriminator().to(device)
cnn = torch.load("D:\\Vscode\\Python\\AI\\Practice\\Discriminator_epoch_9.pth")  # 載入模型權重

# 預測
image = image.to(device)  # 將圖片送到設備上（可能是GPU或者CPU）
output = cnn(image)  # 將圖片送入模型並獲取輸出
print(output)
prediction = output.argmax(dim=1).item()  # 從輸出中獲取預測結果

print(f'模型預測的結果是：{prediction}')
