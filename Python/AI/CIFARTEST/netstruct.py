from torch import nn
from torch.nn import Conv2d, MaxPool2d, Flatten, Linear, Sequential
class Network(nn.Module):
    def __init__(self):
            super(Network, self).__init__()
            self.model1 = Sequential(
                  Conv2d(3, 32, 5, padding=2),
                  MaxPool2d(2),
                  Conv2d(32, 32, 5, padding=2),
                  MaxPool2d(2),
                  Conv2d(32, 64, 5, padding=2),
                  MaxPool2d(2),
                  Flatten(),
                  Linear(1024, 10),
            )
    def forward(self,x):
        x = self.model1(x)
        return x