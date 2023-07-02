from PIL import Image
import torch
import torch.nn as nn
from torchvision import datasets, transforms

# 卷积模块，由卷积核和激活函数组成


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


device = 'cuda:0' if torch.cuda.is_available() else 'cpu'

# Transform
transform = transforms.Compose([transforms.ToTensor(),
                                transforms.Normalize((0.5,), (0.5,))])


def predict_image(model_path, image_path):
    # 初始化模型
    model = CNN().to(device)

    # 讀取模型參數
    model = torch.load(model_path)
    #model = model.to(device)
    model.eval()

    # 讀取圖像
    img = Image.open(image_path).convert('L')  # 轉換成灰度圖像
    img = img.resize((28, 28))  # 改變圖像大小以匹配模型的輸入大小

    # 圖像前處理
    img = transform(img)  # 使用前面定義的 torchvision 轉換
    img = img.unsqueeze(0)  # 因為我們使用的是 CNN，所以需要添加一個 batch 維度
    img = img.to(device)  # 將圖像數據移至 GPU

    # 進行預測
    output = model(img)
    _, predicted = torch.max(output.data, 1)  # 取得預測結果
    return predicted.item()


# 使用方法：
model_path = 'D:\\Vscode\\Python\\AI\\Practice\\model_min_train.pth'
image_path = 'D:\\Vscode\\Python\\AI\\Practice\\6.png'  # 假設 'test.png' 是你要預測的圖片
print(predict_image(model_path, image_path))  # 這會打印出模型預測的數字
