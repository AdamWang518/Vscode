import torch.nn as nn
import torch


class discriminator(nn.Module):
    def __init__(self):
        super(discriminator, self).__init__()
        self.model = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=3, stride=1, padding=1),
            # 32*32
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
            # 16*16
            nn.BatchNorm2d(16),  # 批量正規化層
            nn.Conv2d(16, 32, kernel_size=3, stride=1, padding=1),
            # 16*16
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
            # 8*8
            nn.BatchNorm2d(32),  # 批量正規化層
            nn.Flatten(),
            nn.Linear(32 * 8 * 8, 256),
            nn.ReLU(),
            nn.BatchNorm1d(256),  # 批量正規化層
            nn.Linear(256, 10)  # 輸出10個類別
        )

    def forward(self, x):
        return self.model(x)
