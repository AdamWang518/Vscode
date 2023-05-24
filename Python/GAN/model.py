import torch.nn as nn
class Discriminator(nn.Module):
    def __init__(self):
        self.main = nn.Sequential(
            nn.Linear(784,386),
            nn.ReLU(),
            nn.Linear(386,193),
            nn.ReLU(),
            nn.Linear(193,128),
            nn.ReLU(),
            nn.Linear(128,64),
            nn.ReLU(),
            nn.Linear(64,32),
            nn.ReLU(),
            nn.Linear(32,1),
            nn.Sigmoid()
        )
        return
    def forward(self, x):
        return self.main(input)
class Generator(nn.Module):
    def __init__(self):
        self.main = nn.Sequential(
            nn.Linear(128,1024),
            nn.ReLU(),
            nn.Linear(1024,1024),
            nn.ReLU(),
            nn.Linear(1024,784),
            nn.Tanh()
        )
    def forward(self, x):
        return self.main(input)
