import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from torchvision import datasets, transforms

# GPU
device = 'cuda:0' if torch.cuda.is_available() else 'cpu'
print('GPU State:', device)

class discriminator(nn.Module):
    def __init__(self):
        super(discriminator, self).__init__()
        self.main = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 512),
            nn.ReLU(),
            nn.Linear(512, 256),
            nn.ReLU(),
            nn.Linear(256, 128),
            nn.ReLU(),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Linear(64, 10)
        )

    def forward(self, input):
        return self.main(input)

# Transform
transform = transforms.Compose([transforms.ToTensor(),
                                transforms.Normalize((0.5,), (0.5,))])

train_set = datasets.MNIST('./', train=True, download=True, transform=transform)

train_size = int(0.8 * len(train_set))  # 80% training
test_size = len(train_set) - train_size  # 20% validation
train_dataset, test_dataset = torch.utils.data.random_split(train_set, [train_size, test_size])

batch_size = 64

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

D = discriminator().to(device)

optimizer = optim.Adam(D.parameters(), lr=0.001)
criterion = nn.CrossEntropyLoss()

epochs = 20

for epoch in range(epochs):
    print("epoch:{}".format(epoch))
    running_loss = 0.0
    for i, data in enumerate(train_loader):
        inputs, labels = data
        inputs, labels = inputs.to(device), labels.to(device)  # Move input and label tensors to the GPU
        
        optimizer.zero_grad()

        outputs = D(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        running_loss += loss.item()

    print(f"Epoch: {epoch + 1}, Loss: {running_loss / len(train_loader)}")

    if (epoch + 1) % 5 == 0:
        torch.save(D, f'Discriminator_epoch_{epoch}.pth')
        print('Model saved.')
