import torch
import pytorchvideo
import torchvision.transforms
# 建立 VideoDataset 對象
dataset = pytorchvideo.datasets.VideoClassificationDataset("tennis.mp4",
                                                           "labels",
                                                           frames_per_clip=32,
                                                           step_between_clips=16,
                                                           frame_size=224,
                                                           transform=torchvision.transforms.Compose([
                                                               torchvision.transforms.ToTensor()
                                                           ]))

# 將數據分割成訓練集和驗證集
train_size = int(0.8 * len(dataset))
val_size = len(dataset) - train_size
train_dataset, val_dataset = torch.utils.data.random_split(
    dataset, [train_size, val_size])

# 建立 DataLoader 對象
train_dataloader = torch.utils.data.DataLoader(
    train_dataset, batch_size=32, shuffle=True)
val_dataloader = torch.utils.data.DataLoader(
    val_dataset, batch_size=32, shuffle=False)

# # 定義模型
# model = MyVideoModel()

# # 定義損失函數和優化器
# loss_fn = torch.nn.CrossEntropyLoss()
# optimizer = torch.optim.SGD(model.parameters(), lr=0.001)

# # 迭代訓練模型
# for epoch in range(10):
#     for data, labels in train_dataloader:
#         # 將模型輸出與實際標籤進行比較，計算損失
#         output = model(data)
#         loss = loss_fn(output, labels)
        
#         # 使用優化器計算梯度並更新模型參數
#         optimizer.zero_grad()
       

